package main

import (
	"capitainetoinon/distributed/client"
	"capitainetoinon/distributed/server"

	"github.com/jessevdk/go-flags"
)

type clientOpts struct {
	To   string `long:"to" short:"t" description:"the ip to send the message to" required:"true"`
	Port string `long:"port" short:"p" description:"the port to use" default:"3000"`
}

type ServerCmd struct {
	Config string `short:"c" long:"config" description:"config file to load" required:"true"`
	Port   string `short:"p" long:"port" description:"the port to use" default:"3000"`
}

type ClientCmd struct {
	Create          CreateTransactionCmd `command:"create"`
	GetTransactions GetTransactionsCmd   `command:"get"`
	Vote            VoteCmd              `command:"vote"`
}

type CreateTransactionCmd struct {
	clientOpts
	Sender   string `long:"sender" default:"me"`
	Receiver string `long:"receiver" default:"them"`
	Amount   int    `long:"amount" default:"1000"`
}

type GetTransactionsCmd struct {
	clientOpts
}

type VoteCmd struct {
	clientOpts
	Id int `long:"id" required:"true"`
}

type VoteTransactionCmd struct {
	clientOpts
	Sender   string `long:"sender" default:"me"`
	Receiver string `long:"receiver" default:"them"`
	Amount   int    `long:"amount" default:"1000"`
}

var opts struct {
	Server ServerCmd `command:"server"`
	Client ClientCmd `command:"client"`
}

func (r ServerCmd) Execute(args []string) error {
	return server.Start(r.Config, r.Port)
}

func (r CreateTransactionCmd) Execute(args []string) error {
	return client.Create(r.To, r.Port, r.Sender, r.Receiver, r.Amount)
}

func (r GetTransactionsCmd) Execute(args []string) error {
	return client.Get(r.To, r.Port)
}

func (r VoteCmd) Execute(args []string) error {
	return client.Vote(r.To, r.Port, r.Id)
}

func main() {
	flags.Parse(&opts)
}
