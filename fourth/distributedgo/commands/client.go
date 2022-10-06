package commands

type ClientCommand struct {
	Create CreateCommand `command:"create"`
	Get    GetCommand    `command:"get"`
	Fake   FakeCommand   `command:"fake"`
	// Vote            VoteCmd              `command:"vote"`
}

func (c ClientCommand) Execute([]string) error {
	return nil
}
