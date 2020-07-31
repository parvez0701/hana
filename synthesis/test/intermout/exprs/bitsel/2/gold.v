module test(in, out, sel);

input wire in;
output wire [1:0]out;
input wire sel;
wire synth_net;
MUX_2_1 synth_MUX(.OUT(synth_net), .IN({out[1], out[0]}), .SEL(sel));
BUF_1_1 synth_BUF(.OUT(synth_net), .IN(in));
endmodule
