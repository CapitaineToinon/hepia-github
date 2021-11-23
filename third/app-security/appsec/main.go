package main

import (
	"log"
	"net/http"
	"os"
	"os/signal"

	"appSec/myApp/database"
	"appSec/myApp/env"
	"appSec/myApp/router"
)

func main() {
	port := env.Get("GO_PORT")

	if err := database.Init(); err != nil {
		log.Fatal(err)
		panic(err)
	}

	r := router.GetRouter()

	go func() {
		log.Print("Listening on port " + port)
		if err := http.ListenAndServe(":"+port, r); err != nil {
			log.Fatal(err)
			panic(err)
		}
	}()

	// Wait for interrupt signal to gracefully shutdown
	quit := make(chan os.Signal, 1)
	signal.Notify(quit, os.Interrupt)
	<-quit

	log.Println("Shutdown Server ...")
}
