package commands

import (
	"capitainetoinon/database"
	"capitainetoinon/utils"
	"fmt"
)

type FakeCommand struct {
	Uuid     string `long:"uuid" required:"true"`
	Sender   string `long:"sender" required:"true"`
	Receiver string `long:"receiver" required:"true"`
	Amount   int    `long:"amount" required:"true"`
}

type FakeResponse struct {
	Faked database.Transaction `json:"faked"`
}

func (c FakeCommand) Execute([]string) error {
	db, err := database.Connect()

	if err != nil {
		return err
	}

	transaction, err := db.Fake(database.Transaction{
		Uuid:     c.Uuid,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	if err != nil {
		return err
	}

	response := FakeResponse{
		Faked: *transaction,
	}

	resp, err := utils.PrettyStruct(response)

	if err != nil {
		return err
	}

	fmt.Println(resp)

	return nil
}
