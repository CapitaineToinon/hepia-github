package messages

import (
	"capitainetoinon/distributed/data"
	"encoding/json"
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
	for _, b := range responses {
		var common CommonResponse

		if err := json.Unmarshal(b, &common); err != nil {
			return CommonResponse{
				Message:    err.Error(),
				Operiation: "vote",
				Data:       nil,
			}
		}

		var resp VoteResponse
		err := UnmarshalData(common.Data, &resp)

		if err != nil {
			return CommonResponse{
				Message:    err.Error(),
				Operiation: "vote",
				Data:       nil,
			}
		}

		c.Good += resp.Good
		c.Total += resp.Total
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "vote",
		Data:       c,
	}
}
