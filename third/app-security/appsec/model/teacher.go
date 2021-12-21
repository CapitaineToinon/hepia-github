package model

import (
	"gorm.io/gorm"
)

type Teacher struct {
	gorm.Model
	Lastname string `json:"lastname"`
	Name     string `json:"name"`
	Class    string `json:"class"`
}

type TeacherRegister struct {
	Lastname string `json:"lastname" binding:"required"`
	Name     string `json:"name" binding:"required"`
	Class    string `json:"class" binding:"required"`
}

type TeacherDelete struct {
	ID string `json:"id" binding:"required"`
}
