package messages

import (
	"capitainetoinon/distributed/data"
)

type VoteMessage struct {
	Id       int    `json:"id"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type VoteResponse struct {
	Good  int `json:"good"`
	Total int `json:"total"`
	Rate  int `json:"rate"`
}

func (c VoteMessage) Reach() CommonResponse {
	response := VoteResponse{
		Good:  0,
		Total: 1,
		Rate:  0,
	}

	if data.HasExact(data.Transaction{
		Id:       c.Id,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	}) {
		response.Good = 1
		response.Rate = 100
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "vote",
		Data:       response,
	}
}

func (c VoteResponse) Aggregate(responses []CommonResponse) CommonResponse {
	for _, common := range responses {
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

	if c.Total > 0 {
		c.Rate = c.Good * 100 / c.Total
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "vote",
		Data:       c,
	}
}
