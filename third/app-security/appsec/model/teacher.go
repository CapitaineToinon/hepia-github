package model

import (
	"gorm.io/gorm"
)

type Teacher struct {
	gorm.Model
	Lastname string
	Name     string
	Class    string
}

type TeacherRegister struct {
	Lastname string `json:"lastname" binding:"required"`
	Name     string `json:"name" binding:"required"`
	Class    string `json:"class" binding:"required"`
}
