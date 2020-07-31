module test();

wire synth_net;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
reg A;
reg B;
reg AB_AND;
reg ab_and;
AND_2_1 synth_AND(.OUT(synth_net), .IN({A, B}));
BUF_1_1 synth_BUF(.OUT(synth_net_0), .IN(synth_net));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND_0(.OUT(synth_net_2), .IN({synth_net_0, synth_net_1}));
AND_2_1 synth_AND_1(.OUT(synth_net_3), .IN({synth_net_2, synth_net_1}));
BUF_1_1 synth_BUF_0(.OUT(AB_AND), .IN(synth_net_3));
endmodule
