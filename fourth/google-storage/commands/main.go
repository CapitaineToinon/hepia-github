package commands

var Options struct {
	Create CreateCommand `command:"create"`
	Delete DeleteCommand `command:"delete"`
	List   ListCommand   `command:"list"`
	Get    GetCommand    `command:"get"`
	Fake   FakeCommand   `command:"fake"`
}
