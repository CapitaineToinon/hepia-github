package utils

func Contains[K comparable](input []K, value K) bool {
	for _, n := range input {
		if n == value {
			return true
		}
	}

	return false
}
