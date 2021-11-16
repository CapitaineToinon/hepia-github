package model

import (
	"gorm.io/gorm"
)

type Student struct {
	gorm.Model
	Lastname string
	Name     string
	Filiere  string
}

type StudentRegister struct {
	Lastname string `json:"lastname" binding:"required"`
	Name     string `json:"name" binding:"required"`
	Filiere  string `json:"filiere" binding:"required"`
}
