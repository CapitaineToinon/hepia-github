package messages

import "capitainetoinon/database"

type FakeMessage struct {
	Uuid     string `json:"uuid"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

type FakeResponse struct {
	Uuid     string `json:"uuid"`
	Sender   string `json:"sender"`
	Receiver string `json:"receiver"`
	Amount   int    `json:"amount"`
}

func (c FakeMessage) Reach(db *database.Database) CommonResponse {
	faked, err := db.Fake(database.Transaction{
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
		Operiation: "fake",
		Data: FakeResponse{
			Uuid:     faked.Uuid,
			Sender:   faked.Sender,
			Receiver: faked.Receiver,
			Amount:   faked.Amount,
		},
	}
}
