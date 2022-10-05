package server

import (
	"capitainetoinon/distributed/common"
	yaml "capitainetoinon/distributed/yaml"
	"encoding/json"
	"fmt"

	"log"
	"net"
)

var transactions []common.Transaction

func Start(path string, port string) error {
	me, err := yaml.Parse(path)

	if err != nil {
		return err
	}

	log.SetPrefix(fmt.Sprintf("[%s] ", me.Address))

	server, err := net.Listen("tcp", fmt.Sprintf("%s:%s", me.Address, port))

	if err != nil {
		return err
	}

	defer server.Close()

	log.Println("listening...")

	var reachConn net.Conn
	reach := false
	count := 0

	for {
		c, err := server.Accept()

		if err != nil {
			continue
		}

		go func() {
			d := json.NewDecoder(c)

			var msg common.Message
			err = d.Decode(&msg)

			if err != nil {
				return
			}

			copy := msg
			copy.Source = me.Address

			if !is_from_neighbour(me, msg) && !reach {
				reach = true
				reachConn = c
				go process(msg, c)

				log.Println("from client")
				for _, n := range me.Neighbours {
					go common.Send(copy, n.Address, port)
				}

				return
			}

			count++

			if !reach {
				reach = true
				reachConn = c
				go process(msg, c)

				for _, n := range me.Neighbours {
					go common.Send(copy, n.Address, port)
				}
			}

			if count >= len(me.Neighbours) {
				reach = false
				count = 0

				// time to acknoledge
				reachConn.Write([]byte("ok"))
				reachConn.Close()
			}
		}()
	}
}

func is_from_neighbour(me *yaml.Node, message common.Message) bool {
	for _, n := range me.Neighbours {
		if n.Address == message.Source {
			return true
		}
	}

	return false
}

func process(msg common.Message, c net.Conn) {
	switch msg.Operation {
	case common.Create:
		log.Println("creating transaction")
		transactions = append(transactions, msg.Transaction)
	case common.Vote:
		log.Println("vote message")
		// transactions = append(transactions, msg.Transaction)
	}
}
