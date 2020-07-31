module test(in0, in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, out0_1
, out2_3, out4_5, out6_7, out8_9, out10_11);

input wire in0;
input wire in1;
input wire in2;
input wire in3;
input wire in4;
input wire in5;
input wire in6;
input wire in7;
input wire in8;
input wire in9;
input wire in10;
input wire in11;
output wire out0_1;
output wire out2_3;
output wire out4_5;
output wire out6_7;
output wire out8_9;
output wire out10_11;
AND_2_1 synth_AND(.OUT(out0_1), .IN({in0, in1}));
OR_2_1 synth_OR(.OUT(out2_3), .IN({in2, in3}));
XOR_2_1 synth_XOR(.OUT(out4_5), .IN({in4, in5}));
XNOR_2_1 synth_XNOR(.OUT(out6_7), .IN({in6, in7}));
XNOR_2_1 synth_XNOR_0(.OUT(out8_9), .IN({in8, in9}));
endmodule
