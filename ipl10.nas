	;; hello-os
	;; TAB=4
CYLS	EQU	10
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

	;; プログラム本体
entry:	
	MOV	AX, 0
	MOV	SS, AX
	MOV	SP, 0x7c00
	MOV	DS, AX

	;; ディスクを読みこむ
	MOV	AX, 0x0820
	MOV	ES, AX
	MOV	CH, 0
	MOV	DH, 0
	MOV	CL, 2

readloop:	
	MOV	SI, 0
	
retry:
	MOV	AH, 0x02
	MOV	AL, 1
	MOV	BX, 0
	MOV	DL, 0x00
	INT	0x13
	JNC	next
	ADD	SI, 1
	CMP	SI, 5
	JAE	error
	MOV	AH, 0x00
	MOV	DL, 0x00
	INT	0x13
	JMP	retry

next:
	MOV	AX, ES
	ADD	AX, 0x0020
	MOV	ES, AX
	ADD	CL, 1
	CMP	CL, 18
	JBE	readloop
	MOV	CL, 1
	ADD	DH, 1
	CMP	DH, 2
	JB	readloop
	MOV	DH, 0
	ADD	CH, 1
	CMP	CH, CYLS
	JB	readloop

	MOV	[0x0ff0], CH
	JMP	0xc200

error:
	MOV	AX, 0
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
	DB	"load error"
	DB	0x0a
	DB	0

	RESB	0x7dfe-$

	DB	0x55, 0xaa
