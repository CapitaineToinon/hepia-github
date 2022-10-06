package utils

func Map[K any, T any](data []K, f func(K) T) []T {
	mapped := make([]T, len(data))

	for i, e := range data {
		mapped[i] = f(e)
	}

	return mapped
}
