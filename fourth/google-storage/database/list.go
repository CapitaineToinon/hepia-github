package database

import (
	"cloud.google.com/go/storage"
	"google.golang.org/api/iterator"
)

func (db *Database) List() (*[]string, error) {
	query := &storage.Query{Prefix: ""}

	names := make([]string, 0)
	it := db.bucket.Objects(db.ctx, query)

	for {
		attrs, err := it.Next()

		if err == iterator.Done {
			break
		}

		if err != nil {
			return nil, err
		}

		names = append(names, attrs.Name)
	}

	return &names, nil
}
