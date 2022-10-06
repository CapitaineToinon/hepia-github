package utils

import (
	"bytes"
	"fmt"
	"net"
)

func Send(ip string, port string, message []byte) ([]byte, error) {
	con, err := net.Dial("tcp", fmt.Sprintf("%s:%s", ip, port))

	if err != nil {
		return nil, err
	}

	_, err = con.Write(message)

	if err != nil {
		return nil, err
	}

	reply := make([]byte, 4096)
	_, err = con.Read(reply)

	if err != nil {
		return nil, err
	}

	return bytes.Trim(reply, "\x00"), nil
}
