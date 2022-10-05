package client

import "capitainetoinon/distributed/common"

func Get(to string, port string) error {
	return Start(to, port, common.Message{
		Source:    "client",
		Operation: common.Get,
	})
}
