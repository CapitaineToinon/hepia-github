package commands

type ClientCmd struct {
	Create CreateCommand `command:"create"`
	Get    GetCommand    `command:"get"`
	// Vote            VoteCmd              `command:"vote"`
}

func (c ClientCmd) Execute([]string) error {
	return nil
}
