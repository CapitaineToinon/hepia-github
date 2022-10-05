package common

import (
	"encoding/json"
	"fmt"
	"net"
)

const (
	Create string = "create"
	Vote   string = "vote"
	Get    string = "get"
)

var (
	uuid            int      = 0
	NeedBroadcast   []string = []string{Create, Vote}
	NeedAcknowledge []string = []string{Vote}
)

type Message struct {
	Source      string      `json:"source"`
	Operation   string      `json:"operation"`
	Transaction Transaction `json:"transaction,omitempty"`
}

type Transaction struct {
	Id       int    `jsong:"id,omitempty"`
	Sender   string `json:"sender,omitempty"`
	Receiver string `json:"receiver,omitempty"`
	Amount   int    `json:"amount,omitempty"`
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

func NextUuid() int {
	uuid++
	return uuid
}

func Contains[K comparable](input []K, value K) bool {
	for _, n := range input {
		if n == value {
			return true
		}
	}

	return false
}

func Map[K any, T any](data []K, f func(K) T) []T {

	mapped := make([]T, len(data))

	for i, e := range data {
		mapped[i] = f(e)
	}

	return mapped
}
