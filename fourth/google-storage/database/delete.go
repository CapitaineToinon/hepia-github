package database

func (db *Database) Delete(uuid string) (*string, error) {
	obj := db.bucket.Object(uuid)

	if err := obj.Delete(db.ctx); err != nil {
		return nil, err
	}

	return &uuid, nil
}
