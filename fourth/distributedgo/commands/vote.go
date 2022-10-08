package commands

import (
	"capitainetoinon/distributed/messages"
	"fmt"
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
		Source:          "client",
		Operiation:      "vote",
		Broadcast:       true,
		WithAcknowledge: true,
		Payload: messages.VoteMessage{
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

	var resp messages.VoteResponse
	messages.UnmarshalData(common.Data, &resp)

	if err != nil {
		return err
	}

	fmt.Printf("good %d\n", resp.Good)
	fmt.Printf("Total %d\n", resp.Total)
	fmt.Printf("transaction found in %d%% of the nodes (%d/%d)\n", resp.Good*100/resp.Total, resp.Good, resp.Total)

	return nil
}
