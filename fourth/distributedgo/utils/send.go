package utils

import (
	"bytes"
	"fmt"
	"io"
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

	defer con.Close()

	var buf bytes.Buffer

	if _, err := io.Copy(&buf, con); err != nil {
		return nil, err
	}

	return bytes.Trim(buf.Bytes(), "\x00"), nil
}
