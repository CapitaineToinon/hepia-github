package data

import "fmt"

type Transaction struct {
	Id       int    `json:"id,omitempty"`
	Sender   string `json:"sender,omitempty"`
	Receiver string `json:"receiver,omitempty"`
	Amount   int    `json:"amount,omitempty"`
}

type CreateTransaction struct {
	Sender   string `json:"sender,omitempty"`
	Receiver string `json:"receiver,omitempty"`
	Amount   int    `json:"amount,omitempty"`
}

var id = 0
var Transactions []Transaction

func Create(t CreateTransaction) Transaction {
	created := Transaction{
		Id:       id,
		Sender:   t.Sender,
		Receiver: t.Receiver,
		Amount:   t.Amount,
	}

	Transactions = append(Transactions, created)
	id++

	return created
}

func Fake(fake Transaction) (Transaction, error) {
	index := -1

	for i, transaction := range Transactions {
		if transaction.Id == fake.Id {
			index = i
			break
		}
	}

	if index == -1 {
		return Transaction{}, fmt.Errorf("transaction with id %d not found", fake.Id)
	}

	Transactions[index] = fake
	return fake, nil
}
