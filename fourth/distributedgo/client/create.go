package client

import "capitainetoinon/distributed/common"

func Create(to string, port string, sender string, receiver string, amount int) error {
	return Start(to, port, common.Message{
		Source:    "client",
		Operation: common.Create,
		Transaction: common.Transaction{
			Sender:   sender,
			Receiver: receiver,
			Amount:   amount,
		},
	})
}
