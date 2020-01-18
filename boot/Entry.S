.text
	.code 32

	.global vector_start
	.global vector_end

	vector_start:
	LDR R0, =0x10000000
	LDR R1, [R0]
	LDR R2, [R0]
	MOV R3, R2
	vector_end:
	.space 1024,0x11
.end

/* R0 에 0x10000000 메모리 주소를 읽어 오는 이유는 realviewPB 데이터시트에 0x10000000 = ID Register 이기 때문에다.

아래 그림은 SYS_ID REGISTER 구조이다                    

|31   28|27      16|15     12|11      8|7      0|
-------------------------------------------------
|  REV  |   HBI    |  BUILD  |  ARCH   |  FPGA  |
-------------------------------------------------

REV: 보드 버전 
HBI: 보드 번호
BUILD: 보드 빌드 변형
ARCH: 버스 아키텍처
FPGA: FPGA 빌드

모두 읽기 전용 비트이다.


*/
