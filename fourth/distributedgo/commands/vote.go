package commands

import (
	"capitainetoinon/distributed/messages"
)

type VoteCommand struct {
	CommonOptions
	Id       int    `long:"id" required:"true"`
	Sender   string `long:"sender" required:"true"`
	Receiver string `long:"receiver" required:"true"`
	Amount   int    `long:"amount" required:"true"`
}

func (c VoteCommand) Execute([]string) error {
	msg := messages.CommonMessage{
		Source:     "client",
		Operiation: "vote",
		Broadcast:  true,
		Payload: messages.VoteMessage{
			Id:       c.Id,
			Sender:   c.Sender,
			Receiver: c.Receiver,
			Amount:   c.Amount,
		},
	}

	return msg.Execute(c.Ip, c.Port)
}
