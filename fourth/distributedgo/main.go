package main

import (
	"capitainetoinon/distributed/client"
	"capitainetoinon/distributed/server"

	"github.com/jessevdk/go-flags"
)

type ServerCmd struct {
	Config string `short:"c" long:"config" description:"config file to load" required:"true"`
	Port   string `short:"p" long:"port" description:"the port to use" default:"3000"`
}

type ClientCmd struct {
	To        string `long:"to" description:"the ip to send the message to" required:"true"`
	Port      string `short:"p" long:"port" description:"the port to use" default:"3000"`
	Operation string `long:""`
}

var opts struct {
	Server ServerCmd `command:"server"`
	Client ClientCmd `command:"client"`
}

func (r *ServerCmd) Execute(args []string) error {
	return server.Start(r.Config, r.Port)
}

func (r *ClientCmd) Execute(args []string) error {
	return client.Start(r.To, r.Port)
}

func main() {
	flags.Parse(&opts)
}
