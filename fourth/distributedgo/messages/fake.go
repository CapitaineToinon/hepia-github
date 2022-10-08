package messages

import (
	"capitainetoinon/distributed/data"
)

type FakeMessage struct {
	Id       int    `json:"id"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type FakeResponse struct {
	Message  string `json:"message"`
	Id       int    `json:"id"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

func (c FakeMessage) Reach() CommonResponse {
	faked, err := data.Fake(data.Transaction{
		Id:       c.Id,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	if err != nil {
		return CommonResponse{
			Message: err.Error(),
			Data:    nil,
		}
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "fake",
		Data: FakeResponse{
			Id:       faked.Id,
			Sender:   faked.Sender,
			Receiver: faked.Receiver,
			Amount:   faked.Amount,
		},
	}
}
