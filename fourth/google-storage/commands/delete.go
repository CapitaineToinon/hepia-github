package commands

import (
	"capitainetoinon/database"
	"capitainetoinon/utils"
	"fmt"
)

type DeleteCommand struct {
	Uuid string `long:"uuid" required:"true"`
}

type DeleteResponse struct {
	Deleted string `json:"deleted"`
}

func (c DeleteCommand) Execute([]string) error {
	db, err := database.Connect()

	if err != nil {
		return err
	}

	deleted, err := db.Delete(c.Uuid)

	if err != nil {
		return err
	}

	response := DeleteResponse{
		Deleted: *deleted,
	}

	resp, err := utils.PrettyStruct(response)

	if err != nil {
		return err
	}

	fmt.Println(resp)

	return nil
}
