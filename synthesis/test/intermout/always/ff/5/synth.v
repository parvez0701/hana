module FlipFlop(clock, cs, ns);

input wire clock;
input wire [3:0]cs;
output reg [3:0]ns;
wire synth_net;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
wire synth_net_4;
wire synth_net_5;
wire synth_net_6;
wire synth_net_8;
wire synth_net_10;
wire synth_net_12;
wire synth_net_14;
wire synth_net_16;
wire synth_net_18;
wire synth_net_20;
wire synth_net_22;
wire synth_net_23;
wire synth_net_24;
wire synth_net_25;
wire synth_net_26;
wire synth_net_27;
wire synth_net_28;
wire synth_net_29;
wire synth_net_30;
wire synth_net_31;
wire synth_net_32;
wire synth_net_33;
wire synth_net_34;
reg [3:0] temp;
reg synth_reg;
reg synth_reg_0;
reg synth_reg_1;
reg synth_reg_2;
BUF_1_1 synth_BUF(.OUT(synth_net), .IN(cs[3]));
VCC_0_1 synth_VCC(.OUT(synth_net_0));
BUF_1_1 synth_BUF_0(.OUT(synth_net_1), .IN(cs[2]));
VCC_0_1 synth_VCC_0(.OUT(synth_net_2));
BUF_1_1 synth_BUF_1(.OUT(synth_net_3), .IN(cs[1]));
VCC_0_1 synth_VCC_1(.OUT(synth_net_4));
BUF_1_1 synth_BUF_2(.OUT(synth_net_5), .IN(cs[0]));
VCC_0_1 synth_VCC_2(.OUT(synth_net_6));
VCC_0_1 synth_VCC_3(.OUT(synth_net_8));
VCC_0_1 synth_VCC_4(.OUT(synth_net_10));
VCC_0_1 synth_VCC_5(.OUT(synth_net_12));
VCC_0_1 synth_VCC_6(.OUT(synth_net_14));
VCC_0_1 synth_VCC_7(.OUT(synth_net_16));
VCC_0_1 synth_VCC_8(.OUT(synth_net_18));
VCC_0_1 synth_VCC_9(.OUT(synth_net_20));
VCC_0_1 synth_VCC_10(.OUT(synth_net_22));
AND_2_1 synth_AND(.OUT(synth_net_23), .IN({synth_net_15, synth_net_16}));
FF_2_1 synth_FF(.Q(ns[3]), .D(synth_net_23), .CLK(clock));
AND_2_1 synth_AND_0(.OUT(synth_net_24), .IN({synth_net_17, synth_net_18}));
FF_2_1 synth_FF_0(.Q(ns[2]), .D(synth_net_24), .CLK(clock));
AND_2_1 synth_AND_1(.OUT(synth_net_25), .IN({synth_net_19, synth_net_20}));
FF_2_1 synth_FF_1(.Q(ns[1]), .D(synth_net_25), .CLK(clock));
AND_2_1 synth_AND_2(.OUT(synth_net_26), .IN({synth_net_21, synth_net_22}));
FF_2_1 synth_FF_2(.Q(ns[0]), .D(synth_net_26), .CLK(clock));
AND_2_1 synth_AND_3(.OUT(synth_net_27), .IN({synth_net_13, synth_net_14}));
BUF_1_1 synth_BUF_11(.OUT(synth_reg_2), .IN(synth_net_27));
AND_2_1 synth_AND_4(.OUT(synth_net_28), .IN({synth_net_11, synth_net_12}));
BUF_1_1 synth_BUF_12(.OUT(synth_reg_1), .IN(synth_net_28));
AND_2_1 synth_AND_5(.OUT(synth_net_29), .IN({synth_net_9, synth_net_10}));
BUF_1_1 synth_BUF_13(.OUT(synth_reg_0), .IN(synth_net_29));
AND_2_1 synth_AND_6(.OUT(synth_net_30), .IN({synth_net_7, synth_net_8}));
BUF_1_1 synth_BUF_14(.OUT(synth_reg), .IN(synth_net_30));
AND_2_1 synth_AND_7(.OUT(synth_net_31), .IN({synth_net, synth_net_0}));
FF_2_1 synth_FF_3(.Q(temp[3]), .D(synth_net_31), .CLK(clock));
AND_2_1 synth_AND_8(.OUT(synth_net_32), .IN({synth_net_1, synth_net_2}));
FF_2_1 synth_FF_4(.Q(temp[2]), .D(synth_net_32), .CLK(clock));
AND_2_1 synth_AND_9(.OUT(synth_net_33), .IN({synth_net_3, synth_net_4}));
FF_2_1 synth_FF_5(.Q(temp[1]), .D(synth_net_33), .CLK(clock));
AND_2_1 synth_AND_10(.OUT(synth_net_34), .IN({synth_net_5, synth_net_6}));
FF_2_1 synth_FF_6(.Q(temp[0]), .D(synth_net_34), .CLK(clock));
endmodule