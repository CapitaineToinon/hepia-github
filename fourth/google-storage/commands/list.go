package commands

import (
	"capitainetoinon/database"
	"capitainetoinon/utils"
	"fmt"
)

type ListCommand struct {
}

type ListResponse struct {
	Transactions []string `json:"transactions"`
}

func (c ListCommand) Execute([]string) error {
	db, err := database.Connect()

	if err != nil {
		return err
	}

	transactions, err := db.List()

	if err != nil {
		return err
	}

	response := ListResponse{
		Transactions: *transactions,
	}

	resp, err := utils.PrettyStruct(response)

	if err != nil {
		return err
	}

	fmt.Println(resp)

	return nil
}
