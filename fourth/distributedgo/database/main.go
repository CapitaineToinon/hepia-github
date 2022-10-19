package database

import (
	"context"
	"os"

	"cloud.google.com/go/storage"
	"google.golang.org/api/iterator"
)

type Database struct {
	ctx    context.Context
	client *storage.Client
	bucket *storage.BucketHandle
}

func Connect(bucketName string) (*Database, error) {
	ctx := context.Background()
	client, err := storage.NewClient(ctx)

	if err != nil {
		return nil, err
	}

	bucket := client.Bucket(bucketName)
	it := client.Buckets(ctx, os.Getenv("GOOGLE_PROJECT"))

	for {
		candidate, err := it.Next()

		if err == iterator.Done {
			// didn't find the bucket, create it
			if err = bucket.Create(ctx, os.Getenv("GOOGLE_PROJECT"), nil); err != nil {
				return nil, err
			}

			break
		}

		if candidate.Name == bucketName {
			// bucket exists!
			break
		}
	}

	db := &Database{
		ctx:    ctx,
		client: client,
		bucket: bucket,
	}

	return db, nil
}
