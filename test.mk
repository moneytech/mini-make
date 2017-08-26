include rules.mk vars.mk

-include overrides.mk

t1 t2: p1 p2
	cmd1
	-cmd2
	@cmd3
	-@cmd4
	@-cmd5
