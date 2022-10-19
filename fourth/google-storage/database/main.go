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

func Connect() (*Database, error) {
	var (
		bucketName    string = os.Getenv("GOOGLE_BUCKET_NAME")
		googleProject string = os.Getenv("GOOGLE_PROJECT")
	)

	ctx := context.Background()
	client, err := storage.NewClient(ctx)

	if err != nil {
		return nil, err
	}

	bucket := client.Bucket(bucketName)
	it := client.Buckets(ctx, googleProject)

	for {
		candidate, err := it.Next()

		if err == iterator.Done {
			// didn't find the bucket, create it
			if err = bucket.Create(ctx, googleProject, nil); err != nil {
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
