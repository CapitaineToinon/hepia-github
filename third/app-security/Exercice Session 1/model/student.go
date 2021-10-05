package model

import (
	uuid "github.com/google/uuid"
)

type Student struct {
	UUID     uuid.UUID
	Lastname string
	Name     string
	Filiere  string
}
