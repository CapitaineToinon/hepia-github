package commands

import (
	"capitainetoinon/distributed/messages"
	"fmt"
)

type GetCommand struct {
	CommonOptions
}

func (c GetCommand) Execute([]string) error {
	msg := messages.CommonMessage{
		Source:          "client",
		Operiation:      "get",
		Broadcast:       false,
		WithAcknowledge: false,
		Payload:         nil,
	}

	common, err := msg.Execute(c.Ip, c.Port)

	if err != nil {
		return err
	}

	var resp messages.GetResponse
	messages.UnmarshalData(common.Data, &resp)

	if err != nil {
		return err
	}

	fmt.Println(resp)
	return nil
}
