module test(in, out);

input wire in;
output wire out;
BUF_1_1 synth_BUF(.OUT(out), .IN(in));
endmodule
