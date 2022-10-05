package server

import (
	"capitainetoinon/distributed/common"
	yaml "capitainetoinon/distributed/yaml"
	"encoding/json"
	"fmt"

	"log"
	"net"
)

type Global struct {
	Me           *yaml.Node
	Port         string
	ReachConn    net.Conn
	Transactions []common.Transaction
	ReachMsg     *common.Message
	Responses    [][]byte
	Reach        bool
	Count        int
}

var state Global

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

	state = Global{
		Me:           me,
		Port:         port,
		Transactions: nil,
		ReachConn:    nil,
		ReachMsg:     nil,
		Responses:    nil,
		Reach:        false,
		Count:        0,
	}

	log.Println("listening...")

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

			if is_client(msg) {
				reached(msg, c)
				return
			}

			state.Count++
			if state.Count == len(state.Me.Neighbours) {
				go acknowledged()
				return
			}

			if !state.Reach {
				reached(msg, c)
			}
		}()
	}
}

func is_client(message common.Message) bool {
	strings := common.Map(state.Me.Neighbours, func(n yaml.Node) string {
		return n.Address
	})
	return !common.Contains(strings, message.Source)
}

func reached(msg common.Message, conn net.Conn) {
	log.Printf("Message of type %s from %s\n", msg.Operation, msg.Source)

	state.Reach = true
	state.ReachConn = conn
	state.ReachMsg = &msg

	switch state.ReachMsg.Operation {
	case common.Create:
		msg.Transaction.Id = common.NextUuid()
		state.Transactions = append(state.Transactions, msg.Transaction)
		bytes, _ := json.Marshal(msg.Transaction)
		state.ReachConn.Write(bytes)
	case common.Vote:
		// todo
	case common.Get:
		bytes, _ := json.Marshal(state.Transactions)
		state.ReachConn.Write(bytes)
	}

	if common.Contains(common.NeedBroadcast, state.ReachMsg.Operation) {
		copy := msg
		copy.Source = state.Me.Address

		for _, n := range state.Me.Neighbours {
			addr := n.Address
			go func() {
				bytes, _ := common.Send(copy, addr, state.Port)
				state.Responses = append(state.Responses, bytes)
			}()
		}
	}

	if !common.Contains(common.NeedAcknowledge, state.ReachMsg.Operation) {
		state.ReachConn.Close()
		state.ReachConn = nil
	}

}

func acknowledged() {
	switch state.ReachMsg.Operation {
	case common.Create:
		// message doesn't need acknowledgement
		// reponse already sent
	case common.Vote:
		log.Println("vote message")
	}

	if state.ReachConn != nil {
		state.ReachConn.Close()
	}

	// return the node to a reachable state
	state = Global{
		Me:           state.Me,
		Port:         state.Port,
		Transactions: state.Transactions,
		ReachConn:    nil,
		ReachMsg:     nil,
		Responses:    nil,
		Reach:        false,
		Count:        0,
	}
}
