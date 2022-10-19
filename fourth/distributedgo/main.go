package main

import (
	"capitainetoinon/commands"
	"log"
	"os"

	"github.com/jessevdk/go-flags"
	"github.com/joho/godotenv"
)

func main() {
	err := godotenv.Load()

	if err != nil {
		log.Fatal("Error loading .env file")
		os.Exit(1)
		return
	}

	if _, err := flags.Parse(&commands.Options); err != nil {
		log.Fatal(err)
		os.Exit(1)
		return
	}
}
