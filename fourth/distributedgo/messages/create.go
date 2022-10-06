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

func (c CreateMessage) Reach() ([]byte, error) {
	created := data.Create(data.CreateTransaction{
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	response := CreateResponse{
		Id:       created.Id,
		Sender:   created.Sender,
		Receiver: created.Receiver,
		Amount:   created.Amount,
	}

	bytes, err := json.Marshal(response)

	if err != nil {
		return nil, err
	}

	return bytes, nil
}
