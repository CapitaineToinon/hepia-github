package data

import (
	"fmt"
	"reflect"
)

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
	index := FindIndex(fake)

	if index == -1 {
		return Transaction{}, fmt.Errorf("transaction with id %d not found", fake.Id)
	}

	Transactions[index] = fake
	return fake, nil
}

func FindIndex(transaction Transaction) int {
	for i, candidate := range Transactions {
		if transaction.Id == candidate.Id {
			return i
		}
	}

	return -1
}

func Has(transaction Transaction) bool {
	return FindIndex(transaction) != -1
}

func HasExact(transaction Transaction) bool {
	index := FindIndex(transaction)

	if index == -1 {
		return false
	}

	return reflect.DeepEqual(transaction, Transactions[index])
}
