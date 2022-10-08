package main

import (
	"capitainetoinon/distributed/commands"
	"log"
	"os"

	"github.com/jessevdk/go-flags"
)

func main() {
	log.SetFlags((log.LstdFlags | log.Llongfile) &^ (log.Ldate | log.Ltime))
	_, err := flags.Parse(&commands.Options)

	if err != nil {
		log.Fatal(err)
		os.Exit(1)
	}
}
