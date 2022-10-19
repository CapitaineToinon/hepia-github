package database

func (db *Database) Fake(fake Transaction) (*Transaction, error) {
	target, err := db.Read(fake.Uuid)

	if err != nil {
		return nil, err
	}

	// fake the transaction
	return db.Save(Transaction{
		Uuid:     target.Uuid,
		Sender:   fake.Sender,
		Receiver: fake.Receiver,
		Amount:   fake.Amount,
	})
}
