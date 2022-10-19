package commands

type CommonOptions struct {
	Ip   string `long:"ip" description:"the ip to send the message to" required:"true"`
	Port string `long:"port" description:"the port to use" default:"3000"`
}

var Options struct {
	Server  ServerCommand  `command:"server"`
	Servers ServersCommand `command:"servers"`
	Client  ClientCommand  `command:"client"`
}
