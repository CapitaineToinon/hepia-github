package messages

import (
	"capitainetoinon/distributed/data"
	"encoding/json"
)

type GetMessage struct {
}

type GetResponse struct {
	Transactions []data.Transaction `json:"transactions"`
}

func (c GetMessage) Reach() ([]byte, error) {
	response := GetResponse{
		Transactions: data.Transactions,
	}

	bytes, err := json.Marshal(response)

	if err != nil {
		return nil, err
	}

	return bytes, nil
}
