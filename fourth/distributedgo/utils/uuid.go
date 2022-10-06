package utils

var uuid = 0

func NextUuid() int {
	uuid++
	return uuid
}
