package messages

import (
	"capitainetoinon/distributed/data"
)

type GetMessage struct {
}

type GetResponse struct {
	Transactions []data.Transaction `json:"transactions"`
}

func (c GetMessage) Reach() CommonResponse {
	return CommonResponse{
		Message:    "ok",
		Operiation: "get",
		Data: GetResponse{
			Transactions: data.Transactions,
		},
	}
}
