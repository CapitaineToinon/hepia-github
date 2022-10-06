package common

type Command string

const (
	Create Command = "create"
	Vote   Command = "vote"
	Get    Command = "get"
)

type Message struct {
	Source      string      `json:"source"`
	Operation   Command     `json:"operation"`
	Transaction Transaction `json:"transaction,omitempty"`
}

type Transaction struct {
	Id       int    `json:"id,omitempty"`
	Sender   string `json:"sender,omitempty"`
	Receiver string `json:"receiver,omitempty"`
	Amount   int    `json:"amount,omitempty"`
}
