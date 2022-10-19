package messages

import (
	"capitainetoinon/database"
)

type VoteMessage struct {
	Uuid     string `json:"uuid"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type VoteResponse struct {
	Good  int `json:"good"`
	Total int `json:"total"`
	Rate  int `json:"rate"`
}

func (c VoteMessage) Reach(db *database.Database) CommonResponse {
	response := VoteResponse{
		Good:  0,
		Rate:  0,
		Total: 1,
	}

	exists, err := db.Exists(database.Transaction{
		Uuid:     c.Uuid,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	if err != nil {
		return CommonResponse{
			Message:    err.Error(),
			Operiation: "vote",
			Data:       nil,
		}
	}

	if exists {
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
		// if any of the vote failed, fail all of them
		if !common.Ok() {
			return CommonResponse{
				Message:    common.Message,
				Operiation: "create",
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

	if c.Total > 0 {
		c.Rate = c.Good * 100 / c.Total
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "vote",
		Data:       c,
	}
}
