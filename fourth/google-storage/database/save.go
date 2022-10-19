package database

type SaveTransaction = Transaction

func (db *Database) Save(t SaveTransaction) (*Transaction, error) {
	obj := db.bucket.Object(t.Uuid)
	w := obj.NewWriter(db.ctx)

	bytes, err := t.Marshal()

	if err != nil {
		return nil, err
	}

	if _, err := w.Write(bytes); err != nil {
		return nil, err
	}

	if err := w.Close(); err != nil {
		return nil, err
	}

	return &t, nil
}
