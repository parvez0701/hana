module test(in1, in2, in3, in4, out1, in5, in6, out2, in7, in8, out3, in9, in10
, out4, in11, in12, out, out5, out6);

input wire in1;
input wire in2;
input wire in3;
input wire in4;
output wire out1;
input wire in5;
input wire in6;
output wire out2;
input wire in7;
input wire in8;
output wire out3;
input wire in9;
input wire in10;
output wire out4;
input wire in11;
input wire in12;
output wire out;
output wire out5;
output wire [1:0]out6;
reg [2:0] a;
reg [3:0] b;
assign out = (in1==in2);
assign out1 = (in3!=in4);
assign out2 = (in5>in6);
assign out3 = (in7>=in8);
assign out4 = (in9<in10);
assign out5 = (in11<=in12);
assign out6[1:0] = ({0, a[2:0]}==b[3:0]);
endmodule
