package yaml

import (
	"fmt"
	"io/ioutil"

	"gopkg.in/yaml.v3"
)

type Node struct {
	Id         int    `yaml:"id"`
	Address    string `yaml:"address"`
	Neighbours []Node `yaml:"neighbours"`
}

func Parse(filename string) (*Node, error) {
	buf, err := ioutil.ReadFile(filename)
	if err != nil {
		return nil, err
	}

	n := &Node{}

	err = yaml.Unmarshal(buf, n)
	if err != nil {
		return nil, fmt.Errorf("in file %q: %w", filename, err)
	}

	return n, err
}
