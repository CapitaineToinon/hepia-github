package server

import (
	"capitainetoinon/database"
	"capitainetoinon/messages"
	"capitainetoinon/utils"
	"capitainetoinon/yaml"
	"encoding/json"
	"fmt"
	"log"
	"net"
	"sync"
)

type Server struct {
	config            *yaml.Node
	parentConn        net.Conn
	childrenResponses []messages.CommonResponse
	reach             bool
	count             int
	db                *database.Database
	log               *log.Logger
}

func NewServer(config *yaml.Node) (*Server, error) {
	logger := log.New(log.Default().Writer(), fmt.Sprintf("[%s] ", config.Address), (log.LstdFlags|log.Llongfile)&^(log.Ldate|log.Ltime))

	db, err := database.Connect(fmt.Sprintf("bucket-distributed-%d", config.Id))

	if err != nil {
		return nil, err
	}

	server := &Server{
		config:            config,
		reach:             false,
		count:             0,
		parentConn:        nil,
		childrenResponses: nil,
		db:                db,
		log:               logger,
	}

	logger.Println(fmt.Sprintf("bucket-distributed-%d", config.Id))

	return server, nil
}

func (s *Server) Reset() {
	s.reach = false
	s.count = 0
	s.parentConn = nil
	s.childrenResponses = nil
	s.log.Println("server reset")
}

func (s *Server) Start() error {
	socket, err := net.Listen("tcp", fmt.Sprintf("%s:%s", s.config.Address, s.config.Port))

	if err != nil {
		return err
	}

	defer socket.Close()

	s.log.Println("listening...")

	for {
		c, err := socket.Accept()

		if err != nil {
			continue
		}

		go func() {
			d := json.NewDecoder(c)

			var msg messages.CommonMessage
			err = d.Decode(&msg)

			if err != nil {
				log.Println(err)
				return
			}

			if !msg.Broadcast {
				// simple command that only wants to talk to this
				// specific node.
				go func() {
					defer s.Reset()

					response, err := msg.Reach(s.db)

					if err != nil {
						log.Println(err)
						return
					}

					bytes, err := response.Marshal()

					if err != nil {
						s.log.Println(err)
						return
					}

					if _, err := c.Write(bytes); err != nil {
						s.log.Println(err)
						return
					}

					if err := c.Close(); err != nil {
						s.log.Println(err)
						return
					}
				}()

				return
			}

			if msg.Source == "client" {
				func() {
					defer c.Close()
					defer s.Reset()

					s.reach = true
					s.log.Println("reached by a client")

					copy := msg.CopyAs(s.config.Address)
					responses, err := s.Broadcast(copy, s.config.Neighbours)

					if err != nil {
						s.log.Println(err)
						return
					}

					response, err := msg.Reach(s.db)

					if err != nil {
						s.log.Println(err)
						return
					}

					response, err = response.Aggregate(*responses)

					if err != nil {
						s.log.Println(err)
						return
					}

					bytes, err := response.Marshal()

					if err != nil {
						s.log.Println(err)
						return
					}

					if _, err := c.Write(bytes); err != nil {
						s.log.Println(err)
						return
					}
				}()

				return
			}

			s.count++

			if !s.reach {
				s.reach = true
				s.parentConn = c
				s.log.Println("reached by a neighbour")

				copy := msg
				copy.Source = s.config.Address

				to := utils.Filter(s.config.Neighbours, func(n yaml.Node) bool {
					return n.Address != msg.Source
				})

				responses, err := s.Broadcast(copy, to)

				if err != nil {
					s.log.Println(err)
					return
				}

				s.childrenResponses = *responses
			}

			s.log.Printf("count %d, len neighbours %d\n", s.count, len(s.config.Neighbours)-1)

			if s.count >= len(s.config.Neighbours)-1 && s.parentConn != nil {
				func() {
					defer s.parentConn.Close()
					defer s.Reset()

					s.log.Println("respond to parent")
					response, err := msg.Reach(s.db)

					if err != nil {
						s.log.Println(err)
						return
					}

					response, err = response.Aggregate(s.childrenResponses)

					if err != nil {
						s.log.Println(err)
						return
					}

					bytes, err := response.Marshal()

					if err != nil {
						s.log.Println(err)
						return
					}

					if _, err := s.parentConn.Write(bytes); err != nil {
						s.log.Println(err)
						return
					}
				}()

				return
			}
		}()
	}
}

func (s *Server) Broadcast(msg messages.CommonMessage, to []yaml.Node) (*[]messages.CommonResponse, error) {
	var wg sync.WaitGroup
	var ret []messages.CommonResponse
	var errors []error

	for _, n := range to {
		addr := n.Address
		wg.Add(1)
		go func() {
			defer wg.Done()

			s.log.Printf("broadcasting to %s\n", addr)
			bytes, err := json.Marshal(msg)

			if err != nil {
				errors = append(errors, err)
				return
			}

			bytes, err = utils.Send(addr, s.config.Port, bytes)

			if err != nil {
				errors = append(errors, err)
				return
			}

			var common messages.CommonResponse
			s.log.Println(bytes)
			if err := json.Unmarshal(bytes, &common); err != nil {
				errors = append(errors, err)
				return
			}

			ret = append(ret, common)
			s.log.Printf("broadcast response by %s\n", addr)
		}()
	}

	wg.Wait()

	if len(errors) > 0 {
		return nil, errors[len(errors)-1]
	}

	s.log.Printf("done broadcasting to %d nodes\n", len(to))

	return &ret, nil
}
