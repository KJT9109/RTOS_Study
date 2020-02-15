#include "ARMv7AR.h"
#include "MemoryMap.h"

.text
	.code 32

	.global vector_start
	.global vector_end

     vector_start:
	LDR PC, reset_handler_addr       //0x00 Reset
	LDR PC, undef_handler_addr       //0x04 Undefined Instruction
	LDR PC, svc_handler_addr         //0x08 SVC exception
	LDR PC, pftch_abt_handler_addr   //0x0c Prefetch Abort
	LDR PC, data_abt_handler_addr    //0x10 Data Abort
	B .                              //0x14 Not used
	LDR PC, irq_handler_addr         //0x18 IRQ Interrupt
	LDR PC, fiq_handler_addr         //0x1C FIQ Interrupt
	
	reset_handler_addr: 	.word reset_handler
	undef_handler_addr: 	.word dummy_handler
	svc_handler_addr:   	.word dummy_handler
	pftch_abt_handler_addr: .word dummy_handler
	data_abt_handler_addr:	.word dummy_handler
	irq_handler_addr: 	.word Irq_Handler
	fiq_handler_addr:	      .word Fiq_Handler
      vector_end:
	

      reset_handler:
	MRS r0, cpsr
	BIC r1, r0, #0x1F
	ORR r1, r1, #ARM_MODE_BIT_SVC
	MSR cpsr, r1
	LDR sp, =SVC_STACK_TOP
	
	MRS r0, cpsr
	BIC r1, r0, #0x1F
	ORR r1, r1, #ARM_MODE_BIT_IRQ
	MSR cpsr, r1
	LDR sp, =IRQ_STACK_TOP
	
	MRS r0, cpsr
	BIC r1, r0, #0x1F
	ORR r1, r1, #ARM_MODE_BIT_FIQ
	MSR cpsr, r1
	LDR sp, =FIQ_STACK_TOP

	MRS r0, cpsr
	BIC r1, r0, #0x1F
	ORR r1, r1, #ARM_MODE_BIT_ABT
	MSR cpsr, r1
	LDR sp, =ABT_STACK_TOP

	MRS r0, cpsr
	BIC r1, r0, #0x1F
	ORR r1, r1, #ARM_MODE_BIT_UND
	MSR cpsr, r1
	LDR sp, =UND_STACK_TOP

	MRS r0, cpsr
	BIC r1, r0, #0x1F
	ORR r1, r1, #ARM_MODE_BIT_SYS
	MSR cpsr, r1
	LDR sp, =USRSYS_STACK_TOP
      BL main
      SWI #11

      dummy_handler:
	
      //B .
	
.end

/** 
  **************************************************************
  * 2020.01.18
  * @소스 코드
  *
  * vector_start:
  * LDR R0, = 0x10000000
  * LDR R1, [R0]
  * vector_end:
  *
  **************************************************************
  * @attention
  * R0 에 0x10000000 메모리 주소를 읽어 오는 이유는 
  * realviewPB 데이터시트에 0x10000000 = ID Register 이기 때문에다.
  *
  * 아래 그림은 SYS_ID REGISTER 구조이다                    
  *
  * |31   28|27      16|15     12|11      8|7      0|
  * -------------------------------------------------
  * |  REV  |   HBI    |  BUILD  |  ARCH   |  FPGA  |
  * -------------------------------------------------
  *  
  * REV: 보드 버전 
  * HBI: 보드 번호
  * BUILD: 보드 빌드 변형
  * ARCH: 버스 아키텍처
  * FPGA: FPGA 빌드
  *
  * 모두 읽기 전용 비트이다.
  **************************************************************
  */


  /**
   *************************************************************
   * 2020.01.24
   * @소스 코드
   * 
   * MRS r0, cpsr
   * BIC r1, r0, #0x1F
   * ORR r1, r1, #동작 모드
   * MSR cpsr, r1
   * LDR sp, =스택 꼭대기 메모리 주소
   *
   *************************************************************
   * @attention
   * 반복되는 위 코드는 cpsr에 각 동작 모드를 입력하고 정의한 모드에서 
   * stack top을 설정한다.
   * 
   * r0 에 cpsr 을 가지고 온다음 BIC 명령어를 통해 bit clear를 한 값을
   * r1에 집어 넣는다. r1에 ORR 명령어를 통해 동작모드를 넣고 cpsr에 값을 
   * 넣는다. 마지막으로 설정 된 동작모드에 스텍 주소(top)을 넣는다.
   * Stack은 Top에서  Bottom으로 자라기 때문에 Top에 넣는다.
   *  
   *************************************************************
   * @assembly language
   * MRS: psr에 업데이트(수정)를 위한 읽기 명령어
   * BIC: bit clear r1 = r1 and ~0x1F
   * ORR: bit set
   * MSR: psr에 업데이트(수정)를 위한 쓰기 명령어
   * 
   * ***********************************************************
   * @CPSR operation Mode
   * User 	: 10000
   * FIQ  	: 10001
   * IRQ  	: 10010
   * Supervisor : 10011
   * Abort	: 10111
   * Undefined	: 11011
   * System	: 11111
   *
   ************************************************************* 
   */







