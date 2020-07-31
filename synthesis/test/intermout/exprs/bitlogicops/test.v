module test(in0, in1, in2, in3,in4, in5, in6, in7, in8, in9, in10, in11,
			out0_1, out2_3, out4_5, out6_7, out8_9, out10_11);
input in0, in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11;
output out0_1, out2_3, out4_5, out6_7, out8_9, out10_11;

assign out0_1 = in0 & in1;
assign out2_3 = in2 | in3;
assign out4_5 = in4 ^ in5;
assign out6_7 = in6 ~^ in7;
assign out8_9 = in8 ^~ in9;

endmodule



