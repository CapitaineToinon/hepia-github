package commands

import (
	"capitainetoinon/distributed/messages"
	"capitainetoinon/distributed/utils"
	"encoding/json"
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

	bytes, err := json.Marshal(msg)

	if err != nil {
		return err
	}

	received, err := utils.Send(c.Ip, c.Port, bytes)

	if err != nil {
		return err
	}

	var resp messages.GetResponse
	err = json.Unmarshal(received, &resp)

	if err != nil {
		return err
	}

	fmt.Println(resp)
	return nil
}
