package messages

import (
	"capitainetoinon/distributed/data"
	"encoding/json"
	"log"
)

type VoteMessage struct {
	Id       int    `json:"id"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type VoteResponse struct {
	Good  int `jsong:"good"`
	Total int `jsong:"total"`
}

func (c VoteMessage) Reach() CommonResponse {
	response := VoteResponse{
		Good:  0,
		Total: 1,
	}

	if data.HasExact(data.Transaction{
		Id:       c.Id,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	}) {
		response.Good = 1
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "vote",
		Data:       response,
	}
}

func (c VoteResponse) Aggregate(responses [][]byte) CommonResponse {
	log.Println("aggregating")
	log.Print("me is ")
	log.Println(c)

	for i, b := range responses {
		var common CommonResponse

		if err := json.Unmarshal(b, &common); err != nil {
			return CommonResponse{
				Message: err.Error(),
				Data:    nil,
			}
		}

		var resp VoteResponse
		err := UnmarshalData(common.Data, &resp)

		if err != nil {
			return CommonResponse{
				Message: err.Error(),
				Data:    nil,
			}
		}

		log.Printf("index %d\n", i)
		log.Println(resp)

		c.Good += resp.Good
		c.Total += resp.Total
	}

	log.Print("done with ")
	log.Println(c)

	return CommonResponse{
		Message: "ok",
		Data:    c,
	}
}
