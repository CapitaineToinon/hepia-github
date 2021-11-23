package env

import (
	"errors"
	"log"
	"os"
)

func Get(key string) string {
	value, exist := os.LookupEnv(key)

	if !exist {
		message := key + " was not found in env variables"
		log.Fatal(message)
		panic(errors.New(message))
	}

	if value == "" {
		message := key + " was found in env variables but is empty"
		log.Fatal(message)
		panic(errors.New(message))
	}

	return value
}
