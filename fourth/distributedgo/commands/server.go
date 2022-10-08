package commands

import (
	"capitainetoinon/distributed/messages"
	"capitainetoinon/distributed/utils"
	"capitainetoinon/distributed/yaml"
	"encoding/json"
	"fmt"
	"log"
	"net"
	"sync"
)

type ServerCommand struct {
	Config string `long:"config" description:"config file to load" required:"true"`
	Port   string `long:"port" description:"the port to use" default:"3000"`
}

type PendingConnection struct {
	Connection net.Conn
	Responses  [][]byte
}

type Server struct {
	Me                *yaml.Node
	Port              string
	ParentConn        net.Conn
	ChildrenResponses [][]byte
	Reach             bool
	Count             int
}

func (s *Server) Reset() {
	s.Reach = false
	s.Count = 0
	s.ParentConn = nil
	s.ChildrenResponses = nil
	log.Println("server reset")
}

func (s *Server) Start() error {
	socket, err := net.Listen("tcp", fmt.Sprintf("%s:%s", s.Me.Address, s.Port))

	if err != nil {
		return err
	}

	defer socket.Close()

	log.SetPrefix(fmt.Sprintf("[%s] ", s.Me.Address))
	log.Println("listening...")

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

					response, err := msg.Reach()

					if err != nil {
						log.Println(err)
						return
					}

					bytes, err := response.Marshal()

					if err != nil {
						log.Println(err)
						return
					}

					if _, err := c.Write(bytes); err != nil {
						log.Println(err)
						return
					}

					if err := c.Close(); err != nil {
						log.Println(err)
						return
					}
				}()

				return
			}

			if msg.Source == "client" {
				func() {
					defer s.Reset()
					s.Reach = true
					log.Println("reached by a client")

					copy := msg
					copy.Source = s.Me.Address
					var responses [][]byte

					var wg sync.WaitGroup
					wg.Add(1)

					go func() {
						responses = s.Broadcast(copy, s.Me.Neighbours)
						wg.Done()
					}()

					response, _ := msg.Reach()

					if msg.WithAcknowledge {
						// some commands need to wait for the responses of neighbours
						// and others don't
						wg.Wait()
						response, _ = response.Aggregate(responses)
					}

					bytes, _ := response.Marshal()
					c.Write(bytes)
					c.Close()
				}()

				return
			}

			s.Count++

			if !s.Reach {
				s.Reach = true
				s.ParentConn = c
				log.Println("reached by a neighbour")

				copy := msg
				copy.Source = s.Me.Address

				to := utils.Filter(s.Me.Neighbours, func(n yaml.Node) bool {
					return n.Address != msg.Source
				})

				s.ChildrenResponses = s.Broadcast(copy, to)
			}

			log.Println("now final send maybe?")
			log.Printf("count %d len %d\n", s.Count, len(s.Me.Neighbours))

			if s.Count >= len(s.Me.Neighbours) && s.ParentConn != nil {
				func() {
					defer s.Reset()

					log.Println("respond to parent")
					response, err := msg.Reach()
					response, _ = response.Aggregate(s.ChildrenResponses)
					bytes, _ := response.Marshal()

					if err != nil {
						log.Println(err)
						return
					}

					if _, err := s.ParentConn.Write(bytes); err != nil {
						log.Println(err)
						return
					}

					if err := s.ParentConn.Close(); err != nil {
						log.Println(err)
						return
					}
				}()

				return
			}
		}()
	}
}

func (s *Server) Broadcast(msg messages.CommonMessage, to []yaml.Node) (ret [][]byte) {
	var wg sync.WaitGroup

	for _, n := range s.Me.Neighbours {
		addr := n.Address
		wg.Add(1)
		go func() {
			defer wg.Done()

			log.Printf("broadcasting to %s\n", addr)
			bytes, err := json.Marshal(msg)

			if err != nil {
				log.Println(err)
				return
			}

			resp, err := utils.Send(addr, s.Port, bytes)

			if err != nil {
				log.Println(err)
				return
			}

			ret = append(ret, resp)
			log.Printf("broadcast response by %s\n", addr)
		}()
	}

	wg.Wait()
	return ret
}

func (cmd ServerCommand) Execute([]string) error {
	me, err := yaml.Parse(cmd.Config)

	if err != nil {
		return err
	}

	server := Server{
		Me:    me,
		Port:  cmd.Port,
		Reach: false,
		Count: 0,
	}

	return server.Start()
}
