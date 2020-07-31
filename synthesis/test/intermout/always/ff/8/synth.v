module NegEdgeClock(q, d, clk, reset);

output reg q;
input wire d;
input wire clk;
input wire reset;
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
wire synth_net_12;
GND_0_1 synth_GND(.OUT(synth_net));
BUF_1_1 synth_BUF(.OUT(synth_net_0), .IN(synth_net));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND(.OUT(synth_net_2), .IN({synth_net_0, synth_net_1}));
VCC_0_1 synth_VCC_0(.OUT(synth_net_4));
AND_2_1 synth_AND_0(.OUT(synth_net_5), .IN({synth_net_3, synth_net_4}));
NOT_1_1 synth_NOT(.OUT(synth_net_6), .IN(reset));
VCC_0_1 synth_VCC_1(.OUT(synth_net_7));
AND_2_1 synth_AND_1(.OUT(synth_net_8), .IN({synth_net_7, synth_net_5}));
MUX_2_1 synth_MUX(.OUT(synth_net_9), .IN({synth_net_1, synth_net_4}), .SEL(
    synth_net_6));
NOT_1_1 synth_NOT_0(.IN(clk), .OUT(synth_net_10));
AND_2_1 synth_AND_2(.OUT(synth_net_11), .IN({synth_net_8, synth_net_9}));
NOT_1_1 synth_NOT_1(.IN(reset), .OUT(synth_net_12));
FFR_3_1 synth_FFR(.Q(q), .D(synth_net_11), .CLK(synth_net_10), .RST(synth_net_12
    ));
endmodule
