module test(in1, in2, out);

input wire in1;
input wire in2;
output wire out;
wire w1;
BUF_1_1 synth_BUF(.OUT(w1), .IN(in1));
BUF_1_1 synth_BUF_0(.OUT(w1), .IN(in2));
BUF_1_1 synth_BUF_1(.OUT(out), .IN(w1));
endmodule
