module And(in1, in2, out);

input wire in1;
input wire in2;
output reg out;
wire synth_net;
wire synth_net_1;
AND_2_1 synth_AND(.OUT(synth_net), .IN({in1, in2}));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND_0(.OUT(out), .IN({synth_net_0, synth_net_1}));
endmodule
