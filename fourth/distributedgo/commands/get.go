package commands

import (
	"capitainetoinon/distributed/messages"
)

type GetCommand struct {
	CommonOptions
}

func (c GetCommand) Execute([]string) error {
	msg := messages.CommonMessage{
		Source:     "client",
		Operiation: "get",
		Broadcast:  false,
		Payload:    messages.GetMessage{},
	}

	return msg.Execute(c.Ip, c.Port)
}
