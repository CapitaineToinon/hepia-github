package commands

import (
	"capitainetoinon/database"
	"capitainetoinon/utils"
	"fmt"
)

type GetCommand struct {
	Uuid string `long:"uuid" required:"true"`
}

type GetResponse struct {
	Transaction database.Transaction `json:"transaction"`
}

func (c GetCommand) Execute([]string) error {
	db, err := database.Connect()

	if err != nil {
		return err
	}

	transaction, err := db.Read(c.Uuid)

	if err != nil {
		return err
	}

	response := GetResponse{
		Transaction: *transaction,
	}

	resp, err := utils.PrettyStruct(response)

	if err != nil {
		return err
	}

	fmt.Println(resp)

	return nil
}
