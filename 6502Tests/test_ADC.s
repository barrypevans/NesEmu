LDA #$01
STA $08
ADC #$01
ADC $08

CLC
SBC #$02
SBC #$01

LOOP:
CLC
BCC LOOP