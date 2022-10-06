package messages

import (
	"capitainetoinon/distributed/data"
	"encoding/json"
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

func (c FakeMessage) Reach() ([]byte, error) {
	faked, err := data.Fake(data.Transaction{
		Id:       c.Id,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	if err != nil {
		return nil, err
	}

	response := FakeResponse{
		Id:       faked.Id,
		Sender:   faked.Sender,
		Receiver: faked.Receiver,
		Amount:   faked.Amount,
	}

	bytes, err := json.Marshal(response)

	if err != nil {
		return nil, err
	}

	return bytes, nil
}
