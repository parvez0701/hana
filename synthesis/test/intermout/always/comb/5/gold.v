module test(ctrl, in1, in2, out);

input wire ctrl;
input wire in1;
input wire in2;
output reg out;
wire synth_net;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
wire synth_net_5;
wire synth_net_6;
wire synth_net_7;
wire synth_net_8;
AND_2_1 synth_AND(.OUT(synth_net), .IN({in1, in2}));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND_0(.OUT(synth_net_2), .IN({synth_net_0, synth_net_1}));
OR_2_1 synth_OR(.OUT(synth_net_3), .IN({in1, in2}));
VCC_0_1 synth_VCC_0(.OUT(synth_net_5));
AND_2_1 synth_AND_1(.OUT(synth_net_6), .IN({synth_net_4, synth_net_5}));
MUX_2_1 synth_MUX(.OUT(synth_net_7), .IN({synth_net_2, synth_net_6}), .SEL(ctrl)
);
MUX_2_1 synth_MUX_0(.OUT(synth_net_8), .IN({synth_net_1, synth_net_5}), .SEL(
    ctrl));
AND_2_1 synth_AND_2(.OUT(out), .IN({synth_net_7, synth_net_8}));
endmodule
