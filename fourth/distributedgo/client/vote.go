package client

import "capitainetoinon/distributed/common"

func Vote(to string, port string, id int) error {
	return Start(to, port, common.Message{
		Source:    "client",
		Operation: common.Vote,
		Transaction: common.Transaction{
			Id: id,
		},
	})
}
