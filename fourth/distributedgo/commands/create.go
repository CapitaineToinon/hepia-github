package commands

import (
	"capitainetoinon/messages"

	"github.com/google/uuid"
)

type CreateCommand struct {
	CommonOptions
	Sender   string `long:"sender" default:"me"`
	Receiver string `long:"receiver" default:"them"`
	Amount   int    `long:"amount" default:"1000"`
}

func (c CreateCommand) Execute([]string) error {
	msg := messages.CommonMessage{
		Source:     "client",
		Operiation: "create",
		Broadcast:  true,
		Payload: messages.CreateMessage{
			Uuid:     uuid.NewString(),
			Sender:   c.Sender,
			Receiver: c.Receiver,
			Amount:   c.Amount,
		},
	}

	return msg.Execute(c.Ip, c.Port)
}
