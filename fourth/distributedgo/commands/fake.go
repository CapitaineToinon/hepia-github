package commands

import (
	"capitainetoinon/distributed/messages"
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

	common, err := msg.Execute(c.Ip, c.Port)

	if err != nil {
		return err
	}

	var resp messages.FakeResponse
	messages.UnmarshalData(common.Data, &resp)

	if err != nil {
		return err
	}

	fmt.Println(resp)
	return nil
}
