package commands

import (
	"capitainetoinon/database"
	"capitainetoinon/utils"
	"fmt"

	"github.com/google/uuid"
)

type CreateCommand struct {
	Sender   string `long:"sender" required:"true"`
	Receiver string `long:"receiver" required:"true"`
	Amount   int    `long:"amount" required:"true"`
}

type CreateResponse struct {
	Created database.Transaction `json:"created"`
}

func (c CreateCommand) Execute([]string) error {
	db, err := database.Connect()

	if err != nil {
		return err
	}

	transaction, err := db.Create(database.Transaction{
		Uuid:     uuid.NewString(),
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	if err != nil {
		return err
	}

	response := CreateResponse{
		Created: *transaction,
	}

	resp, err := utils.PrettyStruct(response)

	if err != nil {
		return err
	}

	fmt.Println(resp)

	return nil
}
