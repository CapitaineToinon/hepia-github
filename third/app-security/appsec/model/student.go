package model

import (
	"gorm.io/gorm"
)

type Student struct {
	gorm.Model
	Lastname string `json:"lastname"`
	Name     string `json:"name"`
	Filiere  string `json:"filiere"`
}

type StudentRegister struct {
	Lastname string `json:"lastname" binding:"required"`
	Name     string `json:"name" binding:"required"`
	Filiere  string `json:"filiere" binding:"required"`
}
