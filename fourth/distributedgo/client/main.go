package client

import (
	"capitainetoinon/distributed/common"
	"log"
)

func Start(to string, port string, msg common.Message) error {
	bytes, err := common.Send(msg, to, port)

	if err != nil {
		return err
	}

	log.Println(string(bytes))
	return nil
}
