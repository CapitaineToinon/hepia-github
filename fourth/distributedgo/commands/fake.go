package commands

import (
	"capitainetoinon/distributed/messages"
	"capitainetoinon/distributed/utils"
	"encoding/json"
	"fmt"
)

type FakeCommand struct {
	CommonOptions
	Id       int    `long:"id" required:"true"`
	Sender   string `long:"sender" required:"true"`
	Receiver string `long:"receiver" required:"true"`
	Amount   int    `long:"amount" required:"true"`
}

func (c FakeCommand) Execute([]string) error {
	msg := messages.CommonMessage{
		Source:          "client",
		Operiation:      "fake",
		Broadcast:       false,
		WithAcknowledge: false,
		Payload: messages.FakeMessage{
			Id:       c.Id,
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

	var resp messages.FakeResponse
	err = json.Unmarshal(received, &resp)

	if err != nil {
		return err
	}

	fmt.Println(resp)
	return nil
}
