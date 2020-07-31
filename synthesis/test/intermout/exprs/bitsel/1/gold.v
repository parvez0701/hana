module test(in, out, sel);

input wire [1:0]in;
output wire out;
input wire sel;
MUX_2_1 synth_MUX(.OUT(out), .IN({in[1], in[0]}), .SEL(sel));
endmodule
