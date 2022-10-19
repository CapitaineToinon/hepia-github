package database

import (
	"reflect"

	"cloud.google.com/go/storage"
)

func (db *Database) Exists(t Transaction) (bool, error) {
	target, err := db.Read(t.Uuid)

	if err == storage.ErrObjectNotExist {
		return false, nil
	}

	if err != nil {
		return false, err
	}

	return reflect.DeepEqual(t, *target), nil
}
