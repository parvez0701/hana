module test(in, out1, out2);

input wire in;
output wire out1;
output wire out2;
wire w1;
BUF_1_1 synth_BUF(.OUT(w1), .IN(in));
BUF_1_1 synth_BUF_2(.OUT(out1), .IN(w1));
BUF_1_1 synth_BUF_3(.OUT(out2), .IN(w1));
endmodule
