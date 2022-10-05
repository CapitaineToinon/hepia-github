package client

import (
	"capitainetoinon/distributed/common"
)

func Start(to string, port string) error {
	msg := common.Message{
		Source:    "client",
		Operation: common.Create,
		Transaction: common.Transaction{
			Sender:   "me",
			Receiver: "him",
			Amount:   1000,
		},
	}

	return common.Send(msg, to, port)
}
