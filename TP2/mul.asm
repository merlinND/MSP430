PUBLIC mul ; export `mul` to the outside world
RSEG  CODE ; this is a relocatable segment containing code

mul:       ; entry point to the function
  TST R13
  JZ mulendnull ; jump if second operand is null
  MOV R12,R14 ; working copy
  DEC R13 ; we already got the first operand one time
mulloop:
  TST R13
  JZ mulend ; return when the second operand is null
  ADD R14,R12 ; add the working copy to the accumulator
  DEC R13 ; decrement the counter
  JMP mulloop ; loop
mulend:
  RET ; return R12
mulendnull:
  CLR R12
  RET ; return 0
END