package messages

import (
	"capitainetoinon/database"
)

type GetMessage struct {
}

type GetResponse struct {
	Transactions []string `json:"transactions"`
}

func (c GetMessage) Reach(db *database.Database) CommonResponse {
	transactions, err := db.List()

	if err != nil {
		return CommonResponse{
			Message:    err.Error(),
			Operiation: "fake",
			Data:       nil,
		}
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "get",
		Data: GetResponse{
			Transactions: *transactions,
		},
	}
}
