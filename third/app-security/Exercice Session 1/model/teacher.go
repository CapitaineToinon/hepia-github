package model

import (
	uuid "github.com/google/uuid"
)

type Teacher struct {
	UUID     uuid.UUID
	Lastname string
	Name     string
	Class    string
}
