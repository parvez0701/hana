module FF(clk, a, b, c, e);

input wire clk;
input wire a;
input wire b;
input wire c;
output reg e;
wire synth_net;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
wire synth_net_5;
wire synth_net_6;
wire synth_net_7;
reg d;
OR_2_1 synth_OR(.OUT(synth_net), .IN({d, c}));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND(.OUT(synth_net_2), .IN({synth_net_0, synth_net_1}));
FF_2_1 synth_FF(.Q(e), .D(synth_net_2), .CLK(clk));
AND_2_1 synth_AND_0(.OUT(synth_net_3), .IN({a, b}));
VCC_0_1 synth_VCC_0(.OUT(synth_net_5));
NOT_1_1 synth_NOT(.IN(clk), .OUT(synth_net_6));
AND_2_1 synth_AND_1(.OUT(synth_net_7), .IN({synth_net_4, synth_net_5}));
FF_2_1 synth_FF_0(.Q(d), .D(synth_net_7), .CLK(synth_net_6));
endmodule
