package messages

import (
	"bytes"
	"capitainetoinon/distributed/utils"
	"encoding/json"
	"fmt"
	"log"
)

type Reachable interface {
	Reach() CommonResponse
}

type Aggregatable interface {
	Aggregate([]CommonResponse) CommonResponse
}

type CommonMessage struct {
	Source     string      `json:"source"`
	Operiation string      `json:"operation"`
	Broadcast  bool        `json:"broadcast"`
	Payload    interface{} `json:"payload"`
}

type CommonResponse struct {
	Message    string      `json:"message"`
	Operiation string      `json:"operation"`
	Data       interface{} `json:"data"`
}

func (c CommonMessage) Reach() (*CommonResponse, error) {
	var value map[string]interface{}
	var bytes []byte
	var err error

	if c.Payload != nil {
		value = c.Payload.(map[string]interface{})
		bytes, err = json.Marshal(value)

		if err != nil {
			return nil, err
		}
	}

	var message Reachable

	switch c.Operiation {
	case "create":
		var msg CreateMessage
		err = json.Unmarshal(bytes, &msg)

		if err != nil {
			return nil, err
		}

		message = msg
	case "get":
		message = GetMessage{}
	case "fake":
		var msg FakeMessage
		err = json.Unmarshal(bytes, &msg)

		if err != nil {
			return nil, err
		}

		message = msg
	case "vote":
		var msg VoteMessage
		err = json.Unmarshal(bytes, &msg)

		if err != nil {
			return nil, err
		}

		message = msg
	default:
		return nil, fmt.Errorf("operation not found")
	}

	log.Printf("Operation of type %s received by %s\n", c.Operiation, c.Source)

	response := message.Reach()
	return &response, nil
}

func (c CommonMessage) Marshal() ([]byte, error) {
	return json.Marshal(c)
}

func (c CommonResponse) Aggregate(responses []CommonResponse) (*CommonResponse, error) {
	var aggregatable Aggregatable

	switch c.Operiation {
	case "create":
		aggregatable = c.Data.(CreateResponse)
	case "vote":
		aggregatable = c.Data.(VoteResponse)
	default:
		// assuming the response cannot be aggregated but that's fine
		return &c, nil
	}

	response := aggregatable.Aggregate(responses)
	return &response, nil
}

func (c CommonResponse) Marshal() ([]byte, error) {
	return json.Marshal(c)
}

func (c CommonResponse) Ok() bool {
	return c.Message == "ok"
}

func (c CommonMessage) Execute(ip string, port string) error {
	bytes, err := c.Marshal()

	if err != nil {
		return err
	}

	received, err := utils.Send(ip, port, bytes)

	if err != nil {
		return err
	}

	var resp CommonResponse
	if err := json.Unmarshal(received, &resp); err != nil {
		return err
	}

	output, err := resp.PrettyString()

	if err != nil {
		return err
	}

	fmt.Println(*output)
	return nil
}

func (c CommonResponse) PrettyString() (*string, error) {
	b, err := json.Marshal(c)

	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	dst := &bytes.Buffer{}
	if err := json.Indent(dst, b, "", "  "); err != nil {
		return nil, err
	}

	output := dst.String()
	return &output, nil
}

func UnmarshalData[T any](data interface{}, output *T) error {
	value := data.(map[string]interface{})
	bytes, err := json.Marshal(value)

	if err != nil {
		return err
	}

	err = json.Unmarshal(bytes, output)

	if err != nil {
		return err
	}

	return nil
}
