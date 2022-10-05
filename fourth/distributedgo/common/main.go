package common

import (
	"encoding/json"
	"fmt"
	"net"
)

const (
	Create string = "create"
	Vote   string = "vote"
)

type Message struct {
	Source      string      `json:"source"`
	Operation   string      `json:"operation"`
	Transaction Transaction `json:"transaction"`
}

type Transaction struct {
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

func Send(msg Message, ip string, port string) ([]byte, error) {
	con, err := net.Dial("tcp", fmt.Sprintf("%s:%s", ip, port))

	if err != nil {
		return nil, err
	}

	b, err := json.Marshal(msg)

	if err != nil {
		return nil, err
	}

	_, err = con.Write(b)

	if err != nil {
		return nil, err
	}

	reply := make([]byte, 1024)
	_, err = con.Read(reply)

	if err != nil {
		return nil, err
	}

	return reply, nil
}
