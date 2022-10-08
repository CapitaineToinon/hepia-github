package messages

import (
	"capitainetoinon/distributed/data"
	"encoding/json"
)

type CreateMessage struct {
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type CreateResponse struct {
	Id       int    `json:"id"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

func (c CreateMessage) Reach() CommonResponse {
	created := data.Create(data.CreateTransaction{
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	return CommonResponse{
		Message:    "ok",
		Operiation: "create",
		Data: CreateResponse{
			Id:       created.Id,
			Sender:   created.Sender,
			Receiver: created.Receiver,
			Amount:   created.Amount,
		},
	}
}

func (c CreateResponse) Aggregate(responses [][]byte) CommonResponse {
	for _, b := range responses {
		var common CommonResponse

		if err := json.Unmarshal(b, &common); err != nil {
			return CommonResponse{
				Message:    err.Error(),
				Operiation: "create",
				Data:       nil,
			}
		}

		// if any of the create failed, fail all of them
		if !common.Ok() {
			return CommonResponse{
				Message:    common.Message,
				Operiation: "create",
				Data:       nil,
			}
		}
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "create",
		Data:       c,
	}
}
