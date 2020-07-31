module test(in1, in2, in3, in4, out, out1, in5, in6, out2, in7, in8, out3, in9, in10, out4, in11, in12, out5, out6);
input in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12;
output out, out1, out2, out3, out4, out5;
output [1:0] out6;
reg [2:0] a;
reg [3:0] b;
integer  b;

assign out = (in1 == in2);
assign out1 = (in3 != in4);
assign out2 = (in5 > in6);
assign out3 = (in7 >= in8);
assign out4 = (in9 < in10);
assign out5 = (in11 <= in12);

assign out6 = (a == b);
endmodule
