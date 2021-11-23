package database

import (
	"appSec/myApp/env"
	"appSec/myApp/model"
	"fmt"

	_ "github.com/joho/godotenv/autoload"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

var DB *gorm.DB

type dbConfig struct {
	host     string
	port     string
	user     string
	dbname   string
	password string
}

var config = dbConfig{
	env.Get("MYSQL_HOST"),
	env.Get("MYSQL_PORT"),
	env.Get("MYSQL_USER"),
	env.Get("MYSQL_DB"),
	env.Get("MYSQL_PASSWORD"),
}

var dns = fmt.Sprintf(
	"%s:%s@tcp(%s:%s)/%s?charset=utf8mb4&parseTime=True&loc=Local",
	config.user, config.password, config.host, config.port, config.dbname)

func Init() error {
	var err error

	if DB, err = gorm.Open(mysql.Open(dns), &gorm.Config{}); err != nil {
		return err
	}

	if err = DB.AutoMigrate(&model.Student{}); err != nil {
		return err
	}

	if err = DB.AutoMigrate(&model.Teacher{}); err != nil {
		return err
	}

	return nil
}
