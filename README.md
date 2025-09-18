기본 콘텐츠로 건너뛰기
Google 클래스룸
클래스룸
반도체 RTL설계+Layout+임베디드FPGA 실무프로젝트기반 시스템반도체 설계전문가 양성 2기
게시판
수업 과제
사용자
반도체 RTL설계+Layout+임베디드FPGA 실무프로젝트기반 시스템반도체 설계전문가 양성 2기
곧 마감되는 과제
기한이 곧 돌아오는 과제가 없습니다.

수업에 공지할 내용 입력

게시일: 엄민호
엄민호
생성일: 오후 4:34오후 4:34
error
ㅇㅇ.png 표시 중
ㅇㅇ.png

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 오후 3:29오후 3:29 (오후 3:38에 수정됨)
Latch , ff
오후 3:29 2025년 9월 18일.png 표시 중
오후 3:29 2025년 9월 18일.png

오후 3:38 2025년 9월 18일.png
이미지

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 오후 2:14오후 2:14
test
module test(clk, in_test, out_test);
    input clk;
    input in_test;
    output out_test;
   
    assign out_test = in_test;
   
endmodule

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 오전 11:45오전 11:45 (오후 12:29에 수정됨)
module bin2bcd(clk, iBIN, oBCD);

[본인 코드를 넣어 보세요.]

endmodule

// 7-segment 디코더
module seven_seg_decoder(
    input [3:0] digit,
    output [6:0] segments
);
   
    assign segments = (digit == 4'h0) ? 7'b1000000 : // 0
                      (digit == 4'h1) ? 7'b1111001 : // 1
                      (digit == 4'h2) ? 7'b0100100 : // 2
                      (digit == 4'h3) ? 7'b0110000 : // 3
                      (digit == 4'h4) ? 7'b0011001 : // 4
                      (digit == 4'h5) ? 7'b0010010 : // 5
                      (digit == 4'h6) ? 7'b0000010 : // 6
                      (digit == 4'h7) ? 7'b1111000 : // 7
                      (digit == 4'h8) ? 7'b0000000 : // 8
                      (digit == 4'h9) ? 7'b0010000 : // 9
                      7'b1111111; // 기본값

endmodule

// Binary 입력을 받아 BCD 변환 후 7-segment 디스플레이 (4자리 지원)
module bin_to_7seg_display(
    input clk,                    // 100MHz 클록
    input reset,                  // 리셋 신호
    input [13:0] binary_input,    // 14비트 이진수 입력 (0~9999)
    output [6:0] seg,             // 7-segment 출력
    output [3:0] an               // digit 선택 신호 (active low)
);

    // Binary to BCD 변환
    wire [15:0] bcd_output;
   
    bin2bcd bcd_converter(
        .clk(clk),
        .iBIN(binary_input),
        .oBCD(bcd_output)
    );

    // 클록 분주기 - 디스플레이 다중화용
    reg [16:0] clk_divider;
    wire refresh_clk;
   
    always @(posedge clk or posedge reset) begin
        if (reset)
            clk_divider <= 0;
        else
            clk_divider <= clk_divider + 1;
    end
   
    assign refresh_clk = clk_divider[16]; // 약 763Hz
   
    // 디스플레이 선택 카운터 (2비트로 0~3 순환)
    reg [1:0] digit_select;
   
    always @(posedge refresh_clk or posedge reset) begin
        if (reset)
            digit_select <= 0;
        else
            digit_select <= digit_select + 1;
    end
   
    // BCD에서 각 자리수 분리
    wire [3:0] digit0, digit1, digit2, digit3;
   
    assign digit0 = bcd_output[3:0];     // 일의 자리
    assign digit1 = bcd_output[7:4];     // 십의 자리
    assign digit2 = bcd_output[11:8];    // 백의 자리
    assign digit3 = bcd_output[15:12];   // 천의 자리
   
    // 현재 선택된 digit의 값 (조합논리)
    wire [3:0] current_digit;
   
    assign current_digit = (digit_select == 2'b00) ? digit0 :
                          (digit_select == 2'b01) ? digit1 :
                          (digit_select == 2'b10) ? digit2 : digit3;
   
    // Anode 제어 (조합논리) - active low
    assign an = (digit_select == 2'b00) ? 4'b1110 :  // digit0 활성화
                (digit_select == 2'b01) ? 4'b1101 :  // digit1 활성화
                (digit_select == 2'b10) ? 4'b1011 :  // digit2 활성화
                                          4'b0111;   // digit3 활성화
   
    // 7-segment 디코더 인스턴스
    seven_seg_decoder seg_decoder(
        .digit(current_digit),
        .segments(seg)
    );

endmodule

// 테스트를 위한 top 모듈
module top_module(
    input clk,
    input [13:0] sw,              // 14비트 스위치 입력 (이진수, 0~9999)
    input btnC,                   // 센터 버튼 (리셋용)
    output [6:0] seg,
    output [3:0] an
);

    // 리셋 동기화
    reg reset_sync;
    reg [1:0] reset_ff;
   
    always @(posedge clk) begin
        reset_ff <= {reset_ff[0], btnC};
        reset_sync <= reset_ff[1];
    end
   
    // Binary to 7-segment 디스플레이 인스턴스
    bin_to_7seg_display display_inst(
        .clk(clk),
        .reset(reset_sync),
        .binary_input(sw),        // 14비트 스위치로 이진수 입력
        .seg(seg),
        .an(an)
    );

endmodule


======================================

## This file is a general .xdc for the Basys3 rev B board
## To use it in a project:
## - uncomment the lines corresponding to used pins
## - rename the used ports (in each line, after get_ports) according to the top level signal names in the project

## Clock signal
set_property -dict { PACKAGE_PIN W5   IOSTANDARD LVCMOS33 } [get_ports clk]
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports clk]


## Switches
set_property -dict { PACKAGE_PIN V17   IOSTANDARD LVCMOS33 } [get_ports {sw[0]}]
set_property -dict { PACKAGE_PIN V16   IOSTANDARD LVCMOS33 } [get_ports {sw[1]}]
set_property -dict { PACKAGE_PIN W16   IOSTANDARD LVCMOS33 } [get_ports {sw[2]}]
set_property -dict { PACKAGE_PIN W17   IOSTANDARD LVCMOS33 } [get_ports {sw[3]}]
set_property -dict { PACKAGE_PIN W15   IOSTANDARD LVCMOS33 } [get_ports {sw[4]}]
set_property -dict { PACKAGE_PIN V15   IOSTANDARD LVCMOS33 } [get_ports {sw[5]}]
set_property -dict { PACKAGE_PIN W14   IOSTANDARD LVCMOS33 } [get_ports {sw[6]}]
set_property -dict { PACKAGE_PIN W13   IOSTANDARD LVCMOS33 } [get_ports {sw[7]}]
set_property -dict { PACKAGE_PIN V2    IOSTANDARD LVCMOS33 } [get_ports {sw[8]}]
set_property -dict { PACKAGE_PIN T3    IOSTANDARD LVCMOS33 } [get_ports {sw[9]}]
set_property -dict { PACKAGE_PIN T2    IOSTANDARD LVCMOS33 } [get_ports {sw[10]}]
set_property -dict { PACKAGE_PIN R3    IOSTANDARD LVCMOS33 } [get_ports {sw[11]}]
set_property -dict { PACKAGE_PIN W2    IOSTANDARD LVCMOS33 } [get_ports {sw[12]}]
set_property -dict { PACKAGE_PIN U1    IOSTANDARD LVCMOS33 } [get_ports {sw[13]}]
set_property -dict { PACKAGE_PIN T1    IOSTANDARD LVCMOS33 } [get_ports {sw[14]}]
set_property -dict { PACKAGE_PIN R2    IOSTANDARD LVCMOS33 } [get_ports {sw[15]}]


## LEDs
#set_property -dict { PACKAGE_PIN U16   IOSTANDARD LVCMOS33 } [get_ports {led[0]}]
#set_property -dict { PACKAGE_PIN E19   IOSTANDARD LVCMOS33 } [get_ports {led[1]}]
#set_property -dict { PACKAGE_PIN U19   IOSTANDARD LVCMOS33 } [get_ports {led[2]}]
#set_property -dict { PACKAGE_PIN V19   IOSTANDARD LVCMOS33 } [get_ports {led[3]}]
#set_property -dict { PACKAGE_PIN W18   IOSTANDARD LVCMOS33 } [get_ports {led[4]}]
#set_property -dict { PACKAGE_PIN U15   IOSTANDARD LVCMOS33 } [get_ports {led[5]}]
#set_property -dict { PACKAGE_PIN U14   IOSTANDARD LVCMOS33 } [get_ports {led[6]}]
#set_property -dict { PACKAGE_PIN V14   IOSTANDARD LVCMOS33 } [get_ports {led[7]}]
#set_property -dict { PACKAGE_PIN V13   IOSTANDARD LVCMOS33 } [get_ports {led[8]}]
#set_property -dict { PACKAGE_PIN V3    IOSTANDARD LVCMOS33 } [get_ports {led[9]}]
#set_property -dict { PACKAGE_PIN W3    IOSTANDARD LVCMOS33 } [get_ports {led[10]}]
#set_property -dict { PACKAGE_PIN U3    IOSTANDARD LVCMOS33 } [get_ports {led[11]}]
#set_property -dict { PACKAGE_PIN P3    IOSTANDARD LVCMOS33 } [get_ports {led[12]}]
#set_property -dict { PACKAGE_PIN N3    IOSTANDARD LVCMOS33 } [get_ports {led[13]}]
#set_property -dict { PACKAGE_PIN P1    IOSTANDARD LVCMOS33 } [get_ports {led[14]}]
#set_property -dict { PACKAGE_PIN L1    IOSTANDARD LVCMOS33 } [get_ports {led[15]}]


##7 Segment Display
set_property -dict { PACKAGE_PIN W7   IOSTANDARD LVCMOS33 } [get_ports {seg[0]}]
set_property -dict { PACKAGE_PIN W6   IOSTANDARD LVCMOS33 } [get_ports {seg[1]}]
set_property -dict { PACKAGE_PIN U8   IOSTANDARD LVCMOS33 } [get_ports {seg[2]}]
set_property -dict { PACKAGE_PIN V8   IOSTANDARD LVCMOS33 } [get_ports {seg[3]}]
set_property -dict { PACKAGE_PIN U5   IOSTANDARD LVCMOS33 } [get_ports {seg[4]}]
set_property -dict { PACKAGE_PIN V5   IOSTANDARD LVCMOS33 } [get_ports {seg[5]}]
set_property -dict { PACKAGE_PIN U7   IOSTANDARD LVCMOS33 } [get_ports {seg[6]}]

set_property -dict { PACKAGE_PIN V7   IOSTANDARD LVCMOS33 } [get_ports dp]

set_property -dict { PACKAGE_PIN U2   IOSTANDARD LVCMOS33 } [get_ports {an[0]}]
set_property -dict { PACKAGE_PIN U4   IOSTANDARD LVCMOS33 } [get_ports {an[1]}]
set_property -dict { PACKAGE_PIN V4   IOSTANDARD LVCMOS33 } [get_ports {an[2]}]
set_property -dict { PACKAGE_PIN W4   IOSTANDARD LVCMOS33 } [get_ports {an[3]}]


##Buttons
set_property -dict { PACKAGE_PIN U18   IOSTANDARD LVCMOS33 } [get_ports btnC]
#set_property -dict { PACKAGE_PIN T18   IOSTANDARD LVCMOS33 } [get_ports btnU]
#set_property -dict { PACKAGE_PIN W19   IOSTANDARD LVCMOS33 } [get_ports btnL]
#set_property -dict { PACKAGE_PIN T17   IOSTANDARD LVCMOS33 } [get_ports btnR]
#set_property -dict { PACKAGE_PIN U17   IOSTANDARD LVCMOS33 } [get_ports btnD]


##Pmod Header JA
#set_property -dict { PACKAGE_PIN J1   IOSTANDARD LVCMOS33 } [get_ports {JA[0]}];#Sch name = JA1
#set_property -dict { PACKAGE_PIN L2   IOSTANDARD LVCMOS33 } [get_ports {JA[1]}];#Sch name = JA2
#set_property -dict { PACKAGE_PIN J2   IOSTANDARD LVCMOS33 } [get_ports {JA[2]}];#Sch name = JA3
#set_property -dict { PACKAGE_PIN G2   IOSTANDARD LVCMOS33 } [get_ports {JA[3]}];#Sch name = JA4
#set_property -dict { PACKAGE_PIN H1   IOSTANDARD LVCMOS33 } [get_ports {JA[4]}];#Sch name = JA7
#set_property -dict { PACKAGE_PIN K2   IOSTANDARD LVCMOS33 } [get_ports {JA[5]}];#Sch name = JA8
#set_property -dict { PACKAGE_PIN H2   IOSTANDARD LVCMOS33 } [get_ports {JA[6]}];#Sch name = JA9
#set_property -dict { PACKAGE_PIN G3   IOSTANDARD LVCMOS33 } [get_ports {JA[7]}];#Sch name = JA10

##Pmod Header JB
#set_property -dict { PACKAGE_PIN A14   IOSTANDARD LVCMOS33 } [get_ports {JB[0]}];#Sch name = JB1
#set_property -dict { PACKAGE_PIN A16   IOSTANDARD LVCMOS33 } [get_ports {JB[1]}];#Sch name = JB2
#set_property -dict { PACKAGE_PIN B15   IOSTANDARD LVCMOS33 } [get_ports {JB[2]}];#Sch name = JB3
#set_property -dict { PACKAGE_PIN B16   IOSTANDARD LVCMOS33 } [get_ports {JB[3]}];#Sch name = JB4
#set_property -dict { PACKAGE_PIN A15   IOSTANDARD LVCMOS33 } [get_ports {JB[4]}];#Sch name = JB7
#set_property -dict { PACKAGE_PIN A17   IOSTANDARD LVCMOS33 } [get_ports {JB[5]}];#Sch name = JB8
#set_property -dict { PACKAGE_PIN C15   IOSTANDARD LVCMOS33 } [get_ports {JB[6]}];#Sch name = JB9
#set_property -dict { PACKAGE_PIN C16   IOSTANDARD LVCMOS33 } [get_ports {JB[7]}];#Sch name = JB10

##Pmod Header JC
#set_property -dict { PACKAGE_PIN K17   IOSTANDARD LVCMOS33 } [get_ports {JC[0]}];#Sch name = JC1
#set_property -dict { PACKAGE_PIN M18   IOSTANDARD LVCMOS33 } [get_ports {JC[1]}];#Sch name = JC2
#set_property -dict { PACKAGE_PIN N17   IOSTANDARD LVCMOS33 } [get_ports {JC[2]}];#Sch name = JC3
#set_property -dict { PACKAGE_PIN P18   IOSTANDARD LVCMOS33 } [get_ports {JC[3]}];#Sch name = JC4
#set_property -dict { PACKAGE_PIN L17   IOSTANDARD LVCMOS33 } [get_ports {JC[4]}];#Sch name = JC7
#set_property -dict { PACKAGE_PIN M19   IOSTANDARD LVCMOS33 } [get_ports {JC[5]}];#Sch name = JC8
#set_property -dict { PACKAGE_PIN P17   IOSTANDARD LVCMOS33 } [get_ports {JC[6]}];#Sch name = JC9
#set_property -dict { PACKAGE_PIN R18   IOSTANDARD LVCMOS33 } [get_ports {JC[7]}];#Sch name = JC10

##Pmod Header JXADC
#set_property -dict { PACKAGE_PIN J3   IOSTANDARD LVCMOS33 } [get_ports {JXADC[0]}];#Sch name = XA1_P
#set_property -dict { PACKAGE_PIN L3   IOSTANDARD LVCMOS33 } [get_ports {JXADC[1]}];#Sch name = XA2_P
#set_property -dict { PACKAGE_PIN M2   IOSTANDARD LVCMOS33 } [get_ports {JXADC[2]}];#Sch name = XA3_P
#set_property -dict { PACKAGE_PIN N2   IOSTANDARD LVCMOS33 } [get_ports {JXADC[3]}];#Sch name = XA4_P
#set_property -dict { PACKAGE_PIN K3   IOSTANDARD LVCMOS33 } [get_ports {JXADC[4]}];#Sch name = XA1_N
#set_property -dict { PACKAGE_PIN M3   IOSTANDARD LVCMOS33 } [get_ports {JXADC[5]}];#Sch name = XA2_N
#set_property -dict { PACKAGE_PIN M1   IOSTANDARD LVCMOS33 } [get_ports {JXADC[6]}];#Sch name = XA3_N
#set_property -dict { PACKAGE_PIN N1   IOSTANDARD LVCMOS33 } [get_ports {JXADC[7]}];#Sch name = XA4_N


##VGA Connector
#set_property -dict { PACKAGE_PIN G19   IOSTANDARD LVCMOS33 } [get_ports {vgaRed[0]}]
#set_property -dict { PACKAGE_PIN H19   IOSTANDARD LVCMOS33 } [get_ports {vgaRed[1]}]
#set_property -dict { PACKAGE_PIN J19   IOSTANDARD LVCMOS33 } [get_ports {vgaRed[2]}]
#set_property -dict { PACKAGE_PIN N19   IOSTANDARD LVCMOS33 } [get_ports {vgaRed[3]}]
#set_property -dict { PACKAGE_PIN N18   IOSTANDARD LVCMOS33 } [get_ports {vgaBlue[0]}]
#set_property -dict { PACKAGE_PIN L18   IOSTANDARD LVCMOS33 } [get_ports {vgaBlue[1]}]
#set_property -dict { PACKAGE_PIN K18   IOSTANDARD LVCMOS33 } [get_ports {vgaBlue[2]}]
#set_property -dict { PACKAGE_PIN J18   IOSTANDARD LVCMOS33 } [get_ports {vgaBlue[3]}]
#set_property -dict { PACKAGE_PIN J17   IOSTANDARD LVCMOS33 } [get_ports {vgaGreen[0]}]
#set_property -dict { PACKAGE_PIN H17   IOSTANDARD LVCMOS33 } [get_ports {vgaGreen[1]}]
#set_property -dict { PACKAGE_PIN G17   IOSTANDARD LVCMOS33 } [get_ports {vgaGreen[2]}]
#set_property -dict { PACKAGE_PIN D17   IOSTANDARD LVCMOS33 } [get_ports {vgaGreen[3]}]
#set_property -dict { PACKAGE_PIN P19   IOSTANDARD LVCMOS33 } [get_ports Hsync]
#set_property -dict { PACKAGE_PIN R19   IOSTANDARD LVCMOS33 } [get_ports Vsync]


##USB-RS232 Interface
#set_property -dict { PACKAGE_PIN B18   IOSTANDARD LVCMOS33 } [get_ports RsRx]
#set_property -dict { PACKAGE_PIN A18   IOSTANDARD LVCMOS33 } [get_ports RsTx]


##USB HID (PS/2)
#set_property -dict { PACKAGE_PIN C17   IOSTANDARD LVCMOS33   PULLUP true } [get_ports PS2Clk]
#set_property -dict { PACKAGE_PIN B17   IOSTANDARD LVCMOS33   PULLUP true } [get_ports PS2Data]


##Quad SPI Flash
##Note that CCLK_0 cannot be placed in 7 series devices. You can access it using the
##STARTUPE2 primitive.
#set_property -dict { PACKAGE_PIN D18   IOSTANDARD LVCMOS33 } [get_ports {QspiDB[0]}]
#set_property -dict { PACKAGE_PIN D19   IOSTANDARD LVCMOS33 } [get_ports {QspiDB[1]}]
#set_property -dict { PACKAGE_PIN G18   IOSTANDARD LVCMOS33 } [get_ports {QspiDB[2]}]
#set_property -dict { PACKAGE_PIN F18   IOSTANDARD LVCMOS33 } [get_ports {QspiDB[3]}]
#set_property -dict { PACKAGE_PIN K19   IOSTANDARD LVCMOS33 } [get_ports QspiCSn]


## Configuration options, can be used for all designs
set_property CONFIG_VOLTAGE 3.3 [current_design]
set_property CFGBVS VCCO [current_design]

## SPI configuration mode options for QSPI boot, can be used for all designs
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 33 [current_design]
set_property CONFIG_MODE SPIx4 [current_design]

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 오전 10:23오전 10:23
바이너리 bcd
오전 10:22 2025년 9월 18일.png 표시 중
오전 10:22 2025년 9월 18일.png

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 어제어제
인코더-디코더

https://drive.google.com/drive/folders/1MPmoqyvi371mEtzROGZKNZBX9yjrrRmJ?usp=sharing

인코더_디코더
Google Drive 폴더

수업 댓글 추가...


게시일: Caleb Shin
Caleb Shin
생성일: 어제어제
elebration schematics
001.png 표시 중
001.png

002.png
이미지

003.png
이미지

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 16일9월 16일
TO261AASOT223
SMC0603
SMC0805
JUMPER2
JUMPER3
9031 -> SMSOT23123
LD_BICOLOR_CMS -> SMSOT143CBEE

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 16일9월 16일
vi editor 명령어 모음

1. esc : 명령어 모드로 복귀
2. i, a - 문자 입력 모드로 변경
3. yy 라인 복사 (1라인)
  - 응용 : 여러라인 복사 10yy : 10라인 복사
4. dd 라인 삭제 (1라인)
  - 응용 : 여러라인 복사 10dd : 10라인 삭제
5. : 1 -> 1번 이라인으로 이동
  - 응용 :10 -> 10번 라인으로 이동
6. Shift + G : 가장 끝 라인으로 이동
7. s -> 해당 1글자 수정
8. c + w : 1 워드 수정
9. Shift + $ : 현위치에서 줄의 마지막까지 선택
 - 응용 : c + Shift + $ : 현위치에서 줄의 마지막까지 대치
10. /iX : iX 탐색
  - 탐색 이우기 : (:nohl : 검색 단어 표시 지우기)
  - 탐색한 단어 다음 찾기 : n
11. u : undo
12. "." : 방금전의 명령 이나 동작 반복 수행
13. :set nu -> 라인 넘버 표시
14. e: file_name : 새로운 화면에서 파일 열기
  - 응용 : 이전 문서로 이동 : bp
  - 응용 : 다음 문서로 이동 : np
15. w! -> 강제 저장 (강제 : !)

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 16일9월 16일 (9월 16일에 수정됨)
=======================================
full_adder
=======================================

module full_adder(iX, iY, iCIN, oSUM, oCARRY);
input iX, iY, iCIN;
output oSUM, oCARRY;

    wire s0, c0, c1;

    xor U0(s0, iX, iY);
    and U1(c0, iX, iY);
    xor U2(oSUM, s0, iCIN);
    and U3(c1,s0, iCIN);
    or U4(oCARRY, c1, c0);

endmodule

=======================================
module full_adder(iX, iY, iCIN, oSUM, oCARRY);
input iX, iY, iCIN;
output oSUM, oCARRY;

    wire s0, c0, c1;

    assign s0 = iX & iY;
    assign c0 = iX & iY;
    assign c1 = s0 & iCIN;
    assign oSUM = s0 ^ iCIN;
    assign oCARRY = c0 | c1;

endmodule

=======================================
module half_adder(iA, iB, oC, oS);
input iA, iB;
output oS, oC;

    xor U1(oS, iA, iB);
    and U2(oC, iA, iB);

endmodule

module full_adder(iX, iY, iCIN, oSUM, oCARRY);
    input iX, iY, iCIN;
    output oSUM, oCARRY;

    wire s0, c0, c1;

    half_adder ha0(.iA(iX), .iB(iY), .oC(c0), .oS(s0));
    half_adder ha1(.iA(s0), .iB(iCIN), .oC(c1), .oS(oSUM));
    or OR(oCARRY, c1, c0);

endmodule

=======================================
>modelsim simulation command
=======================================
restart
force -freeze sim:/full_adder/iX 0 0, 1 {2000 ps} -r 4000
force -freeze sim:/full_adder/iY 0 0, 1 {1000 ps} -r 2000
force -freeze sim:/full_adder/iCIN 0 0, 1 {4000 ps} -r 8000
run 10ns

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 16일9월 16일
modelsim

Intel® Quartus® II Web Edition Design Software Version 13.1 for Windows
https://www.intel.com/content/www/us/en/software-kit/666221/intel-quartus-ii-web-edition-design-software-version-13-1-for-windows.html

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 16일9월 16일 (오전 9:07에 수정됨)
Verilog HDL

Ch01.디지털의+이해.pdf
PDF

Ch02.Verilog+HDL.pdf
PDF

Ch03.Verilog+HDL_툴실습.pdf
PDF

Ch04.Verilog+HDL_기본문법.pdf
PDF

Ch05.조합회로+모델링.pdf
PDF

Ch6_1_순차회로 모델링.pdf
PDF

Ch7_1_응용회로 모델링.pdf
PDF

Ch08_디지털시계.pdf
PDF
수업 댓글 1개

이은현어제
module mux (iA, iB, iC, iD, iSEL, oOUT);
    input [7:0] iA, iB, iC, iD;
    input [1:0] iSEL;
    output [7:0] oOUT;

    assign oOUT = (iSEL == 0)? iA:
        (iSEL == 1)? iB:
        (iSEL == 2)? iC:iD;

endmodule

/*
module mux (iA, iB, iC, iD, iSEL, oOUT);
    input [7:0] iA, iB, iC, iD;
    input [1:0] iSEL;
    output [7:0] oOUT;

    reg [7:0] out;

    always@(iA or iB or iC or iD or iSEL)begin
        if(iSEL==0)
            out=iA;
        else if(iSEL==1)
            out=iB;
        else if(iSEL==2)
            out=iC;
        else
            out=iD;
    end

    assign oOUT = out;

endmodule
*/

/*
module mux (iA, iB, iC, iD, iSEL, oOUT);
    input [7:0] iA, iB, iC, iD;
    input [1:0] iSEL;
    output [7:0] oOUT;

    reg [7:0] out;

    always@(iA or iB or iC or iD or iSEL)begin
        case(iSEL)
            0  :out=iA;
            1  :out=iB;
            2  :out=iC;
            default: out=iD;
        endcase
    end

    assign oOUT=out;

endmodule
*/


`timescale 1ns/10ps

module mux;
  reg [7:0] iA, iB, iC, iD;
  reg [1:0] iSEL;
  wire [7:0] oOUT;
 
mux U0(
 .iA (iA),
 .iB (iB),
 .iC (iC),
 .iD (iD),
 .iSEL (iSEL),
 .out (out));
 

initial begin
  iA = 8'h00;iB = 8'h01;iC = 8'h02;iD = 8'h03;iSEL = 2'b00;#100;
  iA = 8'h00;iB = 8'h01;iC = 8'h02;iD = 8'h03;iSEL = 2'b01;#100;
  iA = 8'h00;iB = 8'h01;iC = 8'h02;iD = 8'h03;iSEL = 2'b10;#100;
  iA = 8'h00;iB = 8'h01;iC = 8'h02;iD = 8'h03;iSEL = 2'b11;#100;
end

endmodule

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 15일9월 15일
I2C Interface

ARM-software-I2C.pdf
PDF

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 15일9월 15일 (9월 15일에 수정됨)
PCF8574
오후 2:52 2025년 9월 15일.jpg 표시 중
오후 2:52 2025년 9월 15일.jpg

오후 2:52 2025년 9월 15일(1).jpg
이미지

PCF8574_PCF8574A.pdf
PDF

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 15일9월 15일
github md 파일

ADC.md.txt
텍스트

Buzzer.md.txt
텍스트

EXTI.md.txt
텍스트

HC-SR04.md.txt
텍스트

LED_Blink.md.txt
텍스트

ServoMotor.md.txt
텍스트

TIM_TimeBase.txt
텍스트

USART_Print.md.txt
텍스트

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 15일9월 15일
CHIP Design

gotree94/ChipDesign

GitHub - gotree94/ChipDesign
https://github.com/gotree94/ChipDesign

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 14일9월 14일
Verilog HDL 설계 Vivado와 FPGA를 이용한 설계 실습

Verilog HDL 설계 Vivado와 FPGA를 이용한 설계 실습.pdf
PDF

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 14일9월 14일
Sobel 필터 처리 관련 HTML 설명자료

sobel_sim1.html
HTML

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 14일9월 14일
NUCLEO-F103RB 기타 예제

GitHub - gotree94/STM32
https://github.com/gotree94/STM32

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 14일9월 14일
ARM Architecture - GPIO

GPIO.pdf
PDF

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 12일9월 12일
github 마트다운 문법

마크다운(Markdown) 문법 가이드
마크다운은 텍스트를 HTML로 변환하기 위한 경량 마크업 언어입니다.
📝 제목 (Headers)
markdown# H1 제목
## H2 제목  
### H3 제목
#### H4 제목
##### H5 제목
###### H6 제목
🔤 텍스트 스타일링
강조 텍스트
markdown*이탤릭체* 또는 _이탤릭체_
**볼드체** 또는 __볼드체__
***볼드 이탤릭*** 또는 ___볼드 이탤릭___
~~취소선~~
`인라인 코드`
결과:
이탤릭체, 볼드체, 볼드 이탤릭, 취소선, 인라인 코드
📋 목록 (Lists)
순서 없는 목록
markdown- 항목 1
- 항목 2
  - 하위 항목 1
  - 하위 항목 2
    - 더 깊은 하위 항목

* 별표로도 가능
+ 플러스로도 가능
순서 있는 목록
markdown1. 첫 번째 항목
2. 두 번째 항목
   1. 하위 항목 1
   2. 하위 항목 2
3. 세 번째 항목
체크리스트
markdown- [x] 완료된 작업
- [ ] 미완료 작업
- [x] 또 다른 완료 작업
🔗 링크 (Links)
markdown[링크 텍스트](https://example.com)
[링크 텍스트](https://example.com "툴팁 텍스트")
<https://example.com&gt;
[참조 링크][1]
[1]: https://example.com "참조 링크"

// 내부 링크 (앵커)
[목차로 이동](#목차)
🖼️ 이미지 (Images)
markdown![대체 텍스트](이미지주소.jpg)
![대체 텍스트](이미지주소.jpg "이미지 제목")

// 이미지에 링크 추가
[![이미지](이미지주소.jpg)](https://example.com)

// 참조 방식
![대체 텍스트][image1]
[image1]: 이미지주소.jpg "이미지 제목"
💻 코드 (Code)
인라인 코드
markdown`console.log('Hello World')`
코드 블록
markdown```javascript
function hello() {
    console.log('Hello World');
}
```
```python
def hello():
    print("Hello World")
```
```bash
npm install package-name
```
들여쓰기 코드 블록
markdown    // 4개의 공백으로 들여쓰기
    console.log('Hello');
📊 표 (Tables)
markdown| 열1 | 열2 | 열3 |
|-----|-----|-----|
| 데이터1 | 데이터2 | 데이터3 |
| 데이터4 | 데이터5 | 데이터6 |

// 정렬
| 왼쪽 정렬 | 중앙 정렬 | 오른쪽 정렬 |
|:----------|:---------:|-----------:|
| 데이터1   |  데이터2  |     데이터3 |
📑 인용문 (Blockquotes)
markdown> 이것은 인용문입니다.
> 여러 줄의 인용문도 가능합니다.
>
> > 중첩된 인용문
➖ 구분선 (Horizontal Rules)
markdown---
***
___
🔧 HTML 태그
마크다운 안에서 HTML 태그 사용 가능:
html<details>
<summary>클릭하면 펼쳐집니다</summary>
숨겨진 내용입니다.
</details>

<kbd>Ctrl</kbd> + <kbd>C</kbd>

<mark>하이라이트된 텍스트</mark>

<sub>아래첨자</sub> <sup>위첨자</sup>
🚫 이스케이프 문자
특수 문자를 그대로 표시하려면 백슬래시(\) 사용:
markdown\*별표\*
\#해시태그
\`백틱\`
📚 GitHub 확장 문법
이모지
markdown:smile: :heart: :thumbsup: :octocat:
멘션과 이슈 참조
markdown@username
#123 (이슈 번호)
커밋해시: a1b2c3d
각주 (Footnotes)
markdown여기에 각주가 있습니다[^1].

[^1]: 이것은 각주입니다.
수학 수식 (LaTeX)
markdown$E = mc^2$ (인라인)

$$
\sum_{i=1}^{n} x_i = x_1 + x_2 + ... + x_n
$$
(블록)
🎨 배지 (Badges) 예제
markdown![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Build Status](https://img.shields.io/travis/user/repo)

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 12일9월 12일
이동체 방향 전환

1. 전진/후진:
전진: 모든 바퀴를 같은 방향으로 회전
후진: 모든 바퀴를 반대 방향으로 회전

2. 회전 동작:
좌회전: 좌측 바퀴들을 우측 바퀴들보다 느리게 회전 (또는 반대 방향)
우회전: 우측 바퀴들을 좌측 바퀴들보다 느리게 회전 (또는 반대 방향)

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 12일9월 12일
void timer_start(void)
{
   HAL_TIM_Base_Start(&htim1);
}

void delay_us(uint16_t us)
{
   __HAL_TIM_SET_COUNTER(&htim1, 0); // initislize counter to start from 0
   while((__HAL_TIM_GET_COUNTER(&htim1))<us); // wait count until us
}

//////////////////////////////////
    void trig(void)
   {
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, HIGH);
       delay_us(10);
       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, LOW);
   }

   /**
    * @brief echo 신호가 HIGH를 유지하는 시간을 (㎲)단위로 측정하여 리턴한다.
    * @param no param(void)
    */
   long unsigned int echo(void)
   {
       long unsigned int echo = 0;

       while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == LOW){}
            __HAL_TIM_SET_COUNTER(&htim1, 0);
            while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == HIGH);
            echo = __HAL_TIM_GET_COUNTER(&htim1);
       if( echo >= 240 && echo <= 23000 ) return echo;
       else return 0;
   }

///////////////////////////////////////////////////

timer_start();
printf("Ranging with HC-SR04\n");
/* USER CODE END 2 */

/* Infinite loop */
/* USER CODE BEGIN WHILE */
while (1)
{
    trig();
    echo_time = echo();
    if( echo_time != 0){
        dist = (int)(17 * echo_time / 100);
        printf("Distance = %d(mm)\n", dist);
    }
    else printf("Out of Range!\n");

/* USER CODE END WHILE */

/* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */
}

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 12일9월 12일 (9월 12일에 수정됨)
초음파
오전 11:04 2025년 9월 12일.jpg 표시 중
오전 11:04 2025년 9월 12일.jpg

오전 11:04 2025년 9월 12일(1).jpg
이미지

shield-002.png
이미지

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 12일9월 12일
ultrasonic


정확한 측정을 위한 온도.습고 보정 공식
speedOfSound = 331.4 + (0.6 * temp) + (0.0124 * hum); // Calculate speed of sound in m/s

음속: 약 343m/s = 34,300cm/s
초음파는 왕복 거리를 이동하므로 실제 거리는 절반
계산: 거리(cm) = (echo_time × 34,300) ÷ (1,000,000 × 2) = echo_time × 0.01715
밀리미터 단위로 변환: echo_time × 0.1715
정수 연산을 위해 근사값 사용: echo_time × 17 ÷ 100
echo_time: 왕복 시간 (마이크로초, μs)
34,300: 음속 (cm/s)
1,000,000: 마이크로초를 초로 변환하는 값
2: 왕복 거리를 편도 거리로 나누는 값

계산 과정:
기본 공식: 거리 = 속도 × 시간

단위 맞추기:
     echo_time (μs) → 초: echo_time ÷ 1,000,000
     왕복 거리 → 편도 거리: ÷ 2

최종 공식:
         거리(cm) = 음속(cm/s) × 시간(s) ÷ 2
         거리(cm) = 34,300 × (echo_time ÷ 1,000,000) ÷ 2
         거리(cm) = (echo_time × 34,300) ÷ (1,000,000 × 2)
         거리(cm) = echo_time × 0.01715

 밀리미터 변환:
         거리(mm) = echo_time × 0.01715 × 10 = echo_time × 0.1715

코드에서 사용한 근사값:
         거리(mm) ≈ echo_time × 17 ÷ 100 = echo_time × 0.17
ultrasonic_001.png 표시 중
ultrasonic_001.png

ultrasonic_002.png
이미지

ultrasonic_003.png
이미지

ultrasonic_004.png
이미지

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 11일9월 11일
아두이노
image01.png 표시 중
image01.png

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 11일9월 11일
Altium Viewer : 

Online PCB Viewer to Visualize and Share Electronics Design Projects | Altium 365 Viewer


Gerber Viewer
Gerber Viewer | Solution | interCAD

Online PCB Viewer to Visualize and Share Electronics Design Projects | Altium 365 Viewer
https://www.altium.com/viewer/

Gerber Viewer | Solution | interCAD
https://www.intercad.com/solution/viewer/gerber-viewer

수업 댓글 추가...


게시일: 조민석
조민석
생성일: 9월 11일9월 11일
.
스크린샷 2025-09-11 122251.png 표시 중
스크린샷 2025-09-11 122251.png

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 11일9월 11일
Timer it
오전 10:54 2025년 9월 11일.jpg 표시 중
오전 10:54 2025년 9월 11일.jpg

수업 댓글 추가...


게시일: 김남우
김남우
생성일: 9월 11일9월 11일
자율주행 자동차 - 레고

AI_CAR.pdf
PDF

수업 댓글 추가...

# 피에조 부저로 마리오 음악을 연주

<img width="644" height="586" alt="F103RB-pin" src="https://github.com/user-attachments/assets/774d7903-5392-4df3-b8da-f16d6996ea9c" />

### 타이머 설정 (TIM1):
   * Timers → TIM1 → Clock Source: Internal Clock
   * Channel1 → PWM Generation CH1
   * Configuration → Parameter Settings:
      * Prescaler: 64MHz ÷ 64 (1MHz 클록)
      * Counter Period: 1000 (초기값, 코드에서 동적 변경)
      * Pulse: 500 (50% duty cycle)

<img width="800" height="600" alt="piezo_002" src="https://github.com/user-attachments/assets/6cd1ec46-4fc0-4612-98e3-4301c19f623a" />
<br>
<img width="800" height="600" alt="piezo_001" src="https://github.com/user-attachments/assets/d07acb56-dbe3-4881-80dd-251518bbb7a4" />
<br>


```c
/* USER CODE BEGIN PD */
// 음표 주파수 정의 (Hz)
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

#define REST 0

// 음표 길이 정의 (밀리초)
#define WHOLE     1400      // 원래 2000
#define HALF      700       // 원래 1000
#define QUARTER   350       // 원래 500
#define EIGHTH    175       // 원래 250
#define SIXTEENTH 90        // 원래 125
/* USER CODE END PD */
```

```c
/* USER CODE BEGIN PV */
typedef struct {
    uint16_t frequency;
    uint16_t duration;
} Note;

const Note mario_theme[] = {
    // 첫 번째 구간
    {NOTE_E7, EIGHTH}, {NOTE_E7, EIGHTH}, {REST, EIGHTH}, {NOTE_E7, EIGHTH},
    {REST, EIGHTH}, {NOTE_C7, EIGHTH}, {NOTE_E7, EIGHTH}, {REST, EIGHTH},
    {NOTE_G7, QUARTER}, {REST, QUARTER}, {NOTE_G6, QUARTER}, {REST, QUARTER},

    // 두 번째 구간
    {NOTE_C7, QUARTER}, {REST, EIGHTH}, {NOTE_G6, EIGHTH}, {REST, EIGHTH},
    {NOTE_E6, QUARTER}, {REST, EIGHTH}, {NOTE_A6, EIGHTH}, {REST, EIGHTH},
    {NOTE_B6, EIGHTH}, {REST, EIGHTH}, {NOTE_AS6, EIGHTH}, {NOTE_A6, QUARTER},

    // 세 번째 구간
    {NOTE_G6, EIGHTH}, {NOTE_E7, EIGHTH}, {NOTE_G7, EIGHTH}, {NOTE_A7, QUARTER},
    {NOTE_F7, EIGHTH}, {NOTE_G7, EIGHTH}, {REST, EIGHTH}, {NOTE_E7, EIGHTH},
    {REST, EIGHTH}, {NOTE_C7, EIGHTH}, {NOTE_D7, EIGHTH}, {NOTE_B6, QUARTER},

    // 반복 구간
    {NOTE_C7, QUARTER}, {REST, EIGHTH}, {NOTE_G6, EIGHTH}, {REST, EIGHTH},
    {NOTE_E6, QUARTER}, {REST, EIGHTH}, {NOTE_A6, EIGHTH}, {REST, EIGHTH},
    {NOTE_B6, EIGHTH}, {REST, EIGHTH}, {NOTE_AS6, EIGHTH}, {NOTE_A6, QUARTER},

    {NOTE_G6, EIGHTH}, {NOTE_E7, EIGHTH}, {NOTE_G7, EIGHTH}, {NOTE_A7, QUARTER},
    {NOTE_F7, EIGHTH}, {NOTE_G7, EIGHTH}, {REST, EIGHTH}, {NOTE_E7, EIGHTH},
    {REST, EIGHTH}, {NOTE_C7, EIGHTH}, {NOTE_D7, EIGHTH}, {NOTE_B6, QUARTER},

    // 마무리
    {REST, QUARTER}, {NOTE_G7, EIGHTH}, {NOTE_FS7, EIGHTH}, {NOTE_F7, EIGHTH},
    {NOTE_DS7, QUARTER}, {NOTE_E7, EIGHTH}, {REST, EIGHTH}, {NOTE_GS6, EIGHTH},
    {NOTE_A6, EIGHTH}, {NOTE_C7, EIGHTH}, {REST, EIGHTH}, {NOTE_A6, EIGHTH},
    {NOTE_C7, EIGHTH}, {NOTE_D7, EIGHTH}
};

const int mario_theme_length = sizeof(mario_theme) / sizeof(mario_theme[0]);
/* USER CODE END PV */
```

```c
/* USER CODE BEGIN PFP */
void play_tone(uint16_t frequency, uint16_t duration);
void play_mario_theme(void);
/* USER CODE END PFP */
```

```c
/* USER CODE BEGIN 0 */
/**
 * @brief 특정 주파수와 지속시간으로 톤 재생
 * @param frequency: 재생할 주파수 (Hz), 0이면 무음
 * @param duration: 재생 시간 (밀리초)
 */
void play_tone(uint16_t frequency, uint16_t duration) {
    if (frequency == 0) {
        // 무음 처리
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    } else {
        // 주파수에 따른 ARR 값 계산
        // APB2 클록이 72MHz이고, Prescaler가 72-1이면 1MHz
        // ARR = 1000000 / frequency - 1
        uint32_t arr_value = 1000000 / frequency - 1;

        // 타이머 설정 업데이트
        __HAL_TIM_SET_AUTORELOAD(&htim1, arr_value);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, arr_value / 2); // 50% duty cycle

        // PWM 시작
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    }

    // 지정된 시간만큼 대기
    HAL_Delay(duration);

    // 톤 정지
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

    // 음표 사이의 짧은 간격 (더 빠른 연주를 위해 단축)
    HAL_Delay(30);
}

/**
 * @brief 마리오 테마 음악 재생
 */
void play_mario_theme(void) {
    for (int i = 0; i < mario_theme_length; i++) {
        play_tone(mario_theme[i].frequency, mario_theme[i].duration);
    }
}

/* USER CODE END 0 */
```

```c
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	// 마리오 테마 음악 재생
	play_mario_theme();

	// 음악 종료 후 5초 대기
	HAL_Delay(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
```


Buzzer.md.txt
Buzzer.md.txt 표시 중입니다.
