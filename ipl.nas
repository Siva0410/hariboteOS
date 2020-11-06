	;; hello-os
	;; TAB=4
	ORG	0x7c00
	;; FAT12 format
	JMP	entry
	DB	0x90
	DB	"HELLOIPL"	;bootsecter name
	DW	512 		;secter size
	DB	1		;claster size
	DW	1		;start FAT12
	DB	2		;FAT num
	DW	224		;size of root dir
	DW	2880		;drive size
	DB	0xf0		;type of media
	DW	9		;length of FAT
	DW	18		;number of secter in 1 track
	DW	2		;number of head
	DD	0		;partision
	DD	2880		;size of drive
	DB	0,0,0x29	;wakaran
	DD	0xffffffff	;volume serial
	DB	"HELLO-OS   "	;name of disc
	DB	"FAT12   "	;name of format
	RESB	18		;18 akeru

	;; ÉvÉçÉOÉâÉÄñ{ëÃ
entry:	
	MOV	AX, 0
	MOV	SS, AX
	MOV	SP, 0x7c00
	MOV	DS, AX
	MOV	ES, AX

	MOV	SI, msg
	
putloop:
	MOV	AL, [SI]
	ADD	SI, 1
	CMP	AL, 0
	JE	fin
	MOV	AH, 0x0e
	MOV	BX, 15
	INT	0x10
	JMP	putloop

fin:
	HLT
	JMP	fin

msg:
	DB	0x0a, 0x0a
	DB	"hello, world!"
	DB	0x0a
	DB	0

	RESB	0x7dfe-$

	DB	0x55, 0xaa
