	;; naskfunc
	;; TAB=4

	[FORMAT "WCOFF"]
	[INSTRSET "i486p"]
	[BITS 32]

	[FILE "naskfunc.nas"]

	GLOBAL	_io_hlt

[SECTION .text]

_io_hlt:			;void io_hlt(void);
	HLT
	RET
