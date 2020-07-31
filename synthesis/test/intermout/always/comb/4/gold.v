module test(a, b, c);

output reg a;
input wire b;
input wire c;
wire synth_net;
wire synth_net_0;
wire synth_net_2;
BUF_1_1 synth_BUF(.OUT(synth_net), .IN(b));
VCC_0_1 synth_VCC(.OUT(synth_net_0));
VCC_0_1 synth_VCC_0(.OUT(synth_net_2));
AND_2_1 synth_AND(.OUT(a), .IN({synth_net_1, synth_net_2}));
endmodule
