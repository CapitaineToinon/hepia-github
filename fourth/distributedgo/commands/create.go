package commands

import (
	"capitainetoinon/distributed/messages"
	"capitainetoinon/distributed/utils"
	"encoding/json"
	"fmt"
)

type CreateCommand struct {
	CommonOptions
	Sender   string `long:"sender" default:"me"`
	Receiver string `long:"receiver" default:"them"`
	Amount   int    `long:"amount" default:"1000"`
}

func (c CreateCommand) Execute([]string) error {
	msg := messages.CommonMessage{
		Source:          "client",
		Operiation:      "create",
		Broadcast:       true,
		WithAcknowledge: false,
		Payload: messages.CreateMessage{
			Sender:   c.Sender,
			Receiver: c.Receiver,
			Amount:   c.Amount,
		},
	}

	bytes, err := json.Marshal(msg)

	if err != nil {
		return err
	}

	received, err := utils.Send(c.Ip, c.Port, bytes)

	if err != nil {
		return err
	}

	var resp messages.CreateResponse
	err = json.Unmarshal(received, &resp)

	if err != nil {
		return err
	}

	fmt.Println(resp)
	return nil
}
