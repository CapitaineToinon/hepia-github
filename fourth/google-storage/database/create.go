package database

type CreateTransaction = Transaction

func (db *Database) Create(t CreateTransaction) (*Transaction, error) {
	return db.Save(t)
}
