package commands

import (
	"capitainetoinon/distributed/messages"
	"capitainetoinon/distributed/utils"
	"capitainetoinon/distributed/yaml"
	"encoding/json"
	"fmt"
	"log"
	"net"
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
	Me               *yaml.Node
	Port             string
	PendingConn      net.Conn
	PendingResponses [][]byte
	Reach            bool
	Count            int
}

func (s *Server) Reset() {
	s.Reach = false
	s.Count = 0
	s.PendingConn = nil
	s.PendingResponses = nil
	log.Println("server reset")
}

func (s *Server) Broadcast(msg messages.CommonMessage) {
	copy := msg
	copy.Source = s.Me.Address

	for _, n := range s.Me.Neighbours {
		addr := n.Address
		go func() {
			bytes, _ := json.Marshal(copy)
			resp, _ := utils.Send(addr, s.Port, bytes)
			s.PendingResponses = append(s.PendingResponses, resp)
		}()
	}
}

func (s *Server) OnReach(c net.Conn, msg messages.CommonMessage) error {
	s.Reach = true

	if !msg.WithAcknowledge {
		bytes, err := msg.Reach()

		if err != nil {
			return err
		}

		if _, err := c.Write(bytes); err != nil {
			return err
		}

		if err := c.Close(); err != nil {
			return err
		}
	} else {
		s.PendingConn = c
		// will answer later
	}

	if msg.Broadcast {
		s.Broadcast(msg)
	} else {
		s.Acknowledged(msg)
	}

	return nil
}

func (s *Server) Acknowledged(msg messages.CommonMessage) {
	s.Reset()
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

			if msg.Source == "client" {
				if err := s.OnReach(c, msg); err != nil {
					log.Panic(err)
				}

				return
			}

			s.Count++
			if s.Count == len(s.Me.Neighbours) {
				go s.Acknowledged(msg)
				return
			}

			if !s.Reach {
				if err := s.OnReach(c, msg); err != nil {
					log.Panic(err)
				}
			}
		}()
	}
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
