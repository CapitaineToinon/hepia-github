package database

import (
	"encoding/json"
)

type Transaction struct {
	Uuid     string `json:"uuid"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

func (t *Transaction) Marshal() ([]byte, error) {
	return json.Marshal(t)
}
