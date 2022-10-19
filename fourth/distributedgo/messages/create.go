package messages

import "capitainetoinon/database"

type CreateMessage struct {
	Uuid     string `json:"uuid"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type CreateResponse struct {
	Uuid     string `json:"uuid"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

func (c CreateMessage) Reach(db *database.Database) CommonResponse {
	created, err := db.Create(database.CreateTransaction{
		Uuid:     c.Uuid,
		Sender:   c.Sender,
		Receiver: c.Receiver,
		Amount:   c.Amount,
	})

	if err != nil {
		return CommonResponse{
			Message:    err.Error(),
			Operiation: "fake",
			Data:       nil,
		}
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "create",
		Data: CreateResponse{
			Uuid:     created.Uuid,
			Sender:   created.Sender,
			Receiver: created.Receiver,
			Amount:   created.Amount,
		},
	}
}

func (c CreateResponse) Aggregate(responses []CommonResponse) CommonResponse {
	for _, common := range responses {
		// if any of the create failed, fail all of them
		if !common.Ok() {
			return CommonResponse{
				Message:    common.Message,
				Operiation: "create",
				Data:       nil,
			}
		}
	}

	return CommonResponse{
		Message:    "ok",
		Operiation: "create",
		Data:       c,
	}
}
