package client

import (
	"capitainetoinon/distributed/common"
	"log"
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

	bytes, err := common.Send(msg, to, port)

	if err != nil {
		return err
	}

	log.Println(string(bytes))
	return nil
}
