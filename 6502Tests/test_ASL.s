LDA #$02
ASL
ASL
ASL
STA $00
ASL $00

LSR
LSR
LSR
LSR $00

LOOP:
CLC
BCC LOOP