package messages

import (
	"encoding/json"
	"fmt"
	"log"
)

type Reachable interface {
	Reach() ([]byte, error)
}

type CommonMessage struct {
	Source          string      `json:"source"`
	Operiation      string      `json:"operation"`
	Broadcast       bool        `json:"broadcast"`
	WithAcknowledge bool        `json:"acknowledge"`
	Payload         interface{} `json:"payload"`
}

func (c CommonMessage) Reach() ([]byte, error) {
	var value map[string]interface{}
	var bytes []byte
	var err error

	if c.Payload != nil {
		value = c.Payload.(map[string]interface{})
		bytes, err = json.Marshal(value)

		if err != nil {
			return nil, err
		}
	}

	var message Reachable

	switch c.Operiation {
	case "create":
		var msg CreateMessage
		err = json.Unmarshal(bytes, &msg)

		if err != nil {
			return nil, err
		}

		message = msg
	case "get":
		message = GetMessage{}
	default:
		return nil, fmt.Errorf("operation not found")
	}

	if err != nil {
		return nil, err
	}

	log.Printf("Operation of type %s send by %s\n", c.Operiation, c.Source)
	return message.Reach()
}
