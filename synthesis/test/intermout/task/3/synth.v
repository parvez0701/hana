module test();

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
reg [1:0] A;
reg [1:0] B;
reg [1:0] AB_AND;
reg ab_and;
AND_2_1 synth_AND(.OUT(synth_net), .IN({A[1], B[1]}));
AND_2_1 synth_AND_0(.OUT(synth_net_0), .IN({A[0], B[0]}));
BUF_1_1 synth_BUF(.OUT(synth_net_1), .IN(synth_net));
VCC_0_1 synth_VCC(.OUT(synth_net_2));
BUF_1_1 synth_BUF_0(.OUT(synth_net_3), .IN(synth_net_0));
VCC_0_1 synth_VCC_0(.OUT(synth_net_4));
AND_2_1 synth_AND_1(.OUT(synth_net_5), .IN({synth_net_1, synth_net_2}));
AND_2_1 synth_AND_2(.OUT(synth_net_6), .IN({synth_net_3, synth_net_4}));
AND_2_1 synth_AND_3(.OUT(synth_net_7), .IN({synth_net_5, synth_net_2}));
BUF_1_1 synth_BUF_1(.OUT(AB_AND[1]), .IN(synth_net_7));
AND_2_1 synth_AND_4(.OUT(synth_net_8), .IN({synth_net_6, synth_net_4}));
BUF_1_1 synth_BUF_2(.OUT(AB_AND[0]), .IN(synth_net_8));
endmodule
