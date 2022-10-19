package commands

import (
	"capitainetoinon/server"
	"capitainetoinon/yaml"
)

type ServerCommand struct {
	Config string `long:"config" description:"config file to load" required:"true"`
}

func (cmd ServerCommand) Execute([]string) error {
	config, err := yaml.Parse(cmd.Config)

	if err != nil {
		return err
	}

	s, err := server.NewServer(config)

	if err != nil {
		return err
	}

	return s.Start()
}
