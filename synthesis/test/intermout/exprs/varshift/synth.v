module test(vin0, vout0);

input wire [2:0]vin0;
output reg [7:0]vout0;
wire [7:0]myreg0;
wire [7:0]myreg1;
wire [7:0]myreg2;
wire synth_net;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
wire synth_net_4;
wire synth_net_5;
wire synth_net_6;
wire synth_net_7;
wire synth_net_8;
wire synth_net_9;
wire synth_net_10;
wire synth_net_11;
wire synth_net_12;
wire synth_net_13;
wire synth_net_14;
wire synth_net_15;
wire synth_net_16;
wire synth_net_17;
wire synth_net_18;
wire synth_net_19;
wire synth_net_20;
wire synth_net_21;
wire synth_net_22;
wire synth_net_23;
reg signed [31:0] i;
GND_0_1 synth_GND(.OUT(synth_net));
GND_0_1 synth_GND_0(.OUT(synth_net_0));
GND_0_1 synth_GND_1(.OUT(synth_net_1));
GND_0_1 synth_GND_2(.OUT(synth_net_2));
GND_0_1 synth_GND_3(.OUT(synth_net_3));
GND_0_1 synth_GND_4(.OUT(synth_net_4));
GND_0_1 synth_GND_5(.OUT(synth_net_5));
SHIFTER_8_8 synth_SHIFTER(.OUT(
    {synth_net_6, synth_net_7, synth_net_8, synth_net_9, synth_net_10, synth_net_11, synth_net_12, synth_net_13}
    ), .IN(
    {vout0[7], vout0[6], vout0[5], vout0[4], vout0[3], vout0[2], vout0[1], vout0[0]}
    ), .SHIFT({synth_net_3, vin0[2], vin0[1], vin0[0]}), .OP(synth_net_4), .VAL(
    synth_net_5));
BUF_1_1 synth_BUF(.OUT(myreg0[7]), .IN(synth_net_6));
BUF_1_1 synth_BUF_0(.OUT(myreg0[6]), .IN(synth_net_7));
BUF_1_1 synth_BUF_1(.OUT(myreg0[5]), .IN(synth_net_8));
BUF_1_1 synth_BUF_2(.OUT(myreg0[4]), .IN(synth_net_9));
BUF_1_1 synth_BUF_3(.OUT(myreg0[3]), .IN(synth_net_10));
BUF_1_1 synth_BUF_4(.OUT(myreg0[2]), .IN(synth_net_11));
BUF_1_1 synth_BUF_5(.OUT(myreg0[1]), .IN(synth_net_12));
BUF_1_1 synth_BUF_6(.OUT(myreg0[0]), .IN(synth_net_13));
VCC_0_1 synth_VCC(.OUT(synth_net_14));
GND_0_1 synth_GND_6(.OUT(synth_net_15));
SHIFTER_8_8 synth_SHIFTER_0(.OUT(
    {synth_net_16, synth_net_17, synth_net_18, synth_net_19, synth_net_20, synth_net_21, synth_net_22, synth_net_23}
    ), .IN(
    {myreg2[7], myreg2[6], myreg2[5], myreg2[4], myreg2[3], myreg2[2], myreg2[1], myreg2[0]}
    ), .SHIFT({i[3], i[2], i[1], i[0]}), .OP(synth_net_14), .VAL(synth_net_15));
BUF_1_1 synth_BUF_7(.OUT(myreg1[7]), .IN(synth_net_16));
BUF_1_1 synth_BUF_8(.OUT(myreg1[6]), .IN(synth_net_17));
BUF_1_1 synth_BUF_9(.OUT(myreg1[5]), .IN(synth_net_18));
BUF_1_1 synth_BUF_10(.OUT(myreg1[4]), .IN(synth_net_19));
BUF_1_1 synth_BUF_11(.OUT(myreg1[3]), .IN(synth_net_20));
BUF_1_1 synth_BUF_12(.OUT(myreg1[2]), .IN(synth_net_21));
BUF_1_1 synth_BUF_13(.OUT(myreg1[1]), .IN(synth_net_22));
BUF_1_1 synth_BUF_14(.OUT(myreg1[0]), .IN(synth_net_23));
endmodule
