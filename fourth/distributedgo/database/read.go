package database

import (
	"bytes"
	"encoding/json"
	"io"
)

func (db *Database) Read(uuid string) (*Transaction, error) {
	obj := db.bucket.Object(uuid)
	_, err := obj.Attrs(db.ctx)

	// ensure it exists before reading
	if err != nil {
		return nil, err
	}

	r, err := obj.NewReader(db.ctx)

	if err != nil {
		return nil, err
	}

	defer r.Close()

	buffer := new(bytes.Buffer)
	if _, err = io.Copy(buffer, r); err != nil {
		return nil, err
	}

	var target Transaction
	if err = json.Unmarshal(buffer.Bytes(), &target); err != nil {
		return nil, err
	}

	return &target, nil
}
