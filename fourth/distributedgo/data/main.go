package data

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
