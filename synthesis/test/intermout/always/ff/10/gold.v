module test(byteIn, clockfa, byteout);

input wire [3:0]byteIn;
input wire clockfa;
output wire [3:0]byteout;
wire synth_net_0;
wire synth_net_2;
wire synth_net_4;
wire synth_net_6;
wire synth_net_7;
wire synth_net_8;
wire synth_net_9;
wire synth_net_10;
wire synth_net_11;
reg signed [31:0] j;
VCC_0_1 synth_VCC(.OUT(synth_net_0));
VCC_0_1 synth_VCC_0(.OUT(synth_net_2));
VCC_0_1 synth_VCC_1(.OUT(synth_net_4));
VCC_0_1 synth_VCC_2(.OUT(synth_net_6));
AND_2_1 synth_AND(.OUT(synth_net_7), .IN({synth_net_5, synth_net_6}));
AND_2_1 synth_AND_0(.OUT(synth_net_8), .IN({synth_net_3, synth_net_4}));
AND_2_1 synth_AND_1(.OUT(synth_net_9), .IN({synth_net_1, synth_net_2}));
AND_2_1 synth_AND_2(.OUT(synth_net_10), .IN({synth_net, synth_net_0}));
NOT_1_1 synth_NOT(.IN(clockfa), .OUT(synth_net_11));
AND_2_1 synth_AND_3(.OUT(byteout[3]), .IN({synth_net_10, synth_net_0}));
AND_2_1 synth_AND_4(.OUT(byteout[2]), .IN({synth_net_9, synth_net_2}));
AND_2_1 synth_AND_5(.OUT(byteout[1]), .IN({synth_net_8, synth_net_4}));
AND_2_1 synth_AND_6(.OUT(byteout[0]), .IN({synth_net_7, synth_net_6}));
endmodule
