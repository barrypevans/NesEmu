LDA #$00

CMP_LOOP:
CLC
ADC #$01
CMP #$02
BNE CMP_LOOP

LDA #$00
CPX_LOOP:
CLC
ADC #$01
TAX
CPX #$02
BNE CPX_LOOP

LDA #$00
CPY_LOOP:
CLC
ADC #$01
TAY
CPY #$02
BNE CPY_LOOP

BCC_LOOP:
CLC
BCC BCC_LOOP
