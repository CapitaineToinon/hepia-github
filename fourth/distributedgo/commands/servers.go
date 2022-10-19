package commands

import (
	"capitainetoinon/server"
	"capitainetoinon/yaml"
	"log"
	"sync"
)

type ServersCommand struct {
	Configs []string `long:"configs" description:"config files to load" required:"true"`
}

func (cmd ServersCommand) Execute([]string) error {
	var wg sync.WaitGroup

	for _, path := range cmd.Configs {
		wg.Add(1)
		yamlPath := path

		go func() {
			defer wg.Done()
			config, err := yaml.Parse(yamlPath)

			if err != nil {
				log.Println(err)
				return
			}

			s, err := server.NewServer(config)

			if err != nil {
				log.Println(err)
				return
			}

			if err := s.Start(); err != nil {
				log.Println(err)
				return
			}
		}()
	}

	wg.Wait()

	return nil
}
