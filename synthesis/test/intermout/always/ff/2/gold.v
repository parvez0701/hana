module test(d, clk, preset, q);

input wire d;
input wire clk;
input wire preset;
output reg q;
wire synth_net;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
wire synth_net_4;
wire synth_net_5;
wire synth_net_6;
wire synth_net_7;
wire synth_net_8;
wire synth_net_9;
wire synth_net_10;
wire synth_net_11;
GND_0_1 synth_GND(.OUT(synth_net));
BUF_1_1 synth_BUF(.OUT(synth_net_0), .IN(synth_net));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND(.OUT(synth_net_2), .IN({synth_net_0, synth_net_1}));
VCC_0_1 synth_VCC_0(.OUT(synth_net_4));
AND_2_1 synth_AND_0(.OUT(synth_net_5), .IN({synth_net_3, synth_net_4}));
NOT_1_1 synth_NOT(.OUT(synth_net_6), .IN(preset));
VCC_0_1 synth_VCC_1(.OUT(synth_net_7));
AND_2_1 synth_AND_1(.OUT(synth_net_8), .IN({synth_net_7, synth_net_5}));
MUX_2_1 synth_MUX(.OUT(synth_net_9), .IN({synth_net_1, synth_net_4}), .SEL(
    synth_net_6));
AND_2_1 synth_AND_2(.OUT(synth_net_10), .IN({synth_net_8, synth_net_9}));
NOT_1_1 synth_NOT_0(.IN(preset), .OUT(synth_net_11));
FFR_3_1 synth_FFR(.Q(q), .D(synth_net_10), .CLK(clk), .RST(synth_net_11));
endmodule
