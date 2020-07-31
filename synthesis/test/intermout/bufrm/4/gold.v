module test(in1, in2, out1, out2);

input wire in1;
input wire in2;
output wire out1;
output wire out2;
wire w3;
BUF_1_1 synth_BUF_1(.OUT(w3), .IN(in1));
BUF_1_1 synth_BUF_2(.OUT(w3), .IN(in2));
BUF_1_1 synth_BUF_5(.OUT(out1), .IN(w3));
BUF_1_1 synth_BUF_6(.OUT(out2), .IN(w3));
endmodule
