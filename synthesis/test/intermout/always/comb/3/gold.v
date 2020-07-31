module test(in1, in2, out);

input wire in1;
input wire in2;
output reg out;
wire synth_net_0;
wire synth_net_1;
wire synth_net_3;
wire synth_net_4;
wire synth_net_7;
wire synth_net_9;
wire synth_net_10;
VCC_0_1 synth_VCC(.OUT(synth_net_0));
AND_2_1 synth_AND(.OUT(synth_net_1), .IN({synth_net, synth_net_0}));
VCC_0_1 synth_VCC_0(.OUT(synth_net_3));
AND_2_1 synth_AND_0(.OUT(synth_net_4), .IN({synth_net_2, synth_net_3}));
CMP_1_1 synth_CMP(.EQ(), .INEQ(), .GT(synth_net_7), .LT(), .A(in1), .B(in2));
MUX_2_1 synth_MUX(.OUT(synth_net_9), .IN({synth_net_1, synth_net_4}), .SEL(
    synth_net_7));
MUX_2_1 synth_MUX_0(.OUT(synth_net_10), .IN({synth_net_0, synth_net_3}), .SEL(
    synth_net_7));
AND_2_1 synth_AND_1(.OUT(out), .IN({synth_net_9, synth_net_10}));
endmodule
