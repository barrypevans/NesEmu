LDA #$00

JMP JMP_LBL

SUBROUTINE:
ADC #$01
ADC #$01
ADC #$01
ADC #$01
RTS

JMP_LBL:
ADC #$01
ADC #$01
ADC #$01
ADC #$01
JSR SUBROUTINE

BCC_LOOP:
CLC
BCC BCC_LOOP
