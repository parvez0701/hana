module Reduction(A1, A2, A3, A4, A5, A6, Y1, Y2, Y3, Y4, Y5, Y6);

input wire [1:0]A1;
input wire [1:0]A2;
input wire [1:0]A3;
input wire [1:0]A4;
input wire [1:0]A5;
input wire [1:0]A6;
output wire Y1;
output wire Y2;
output wire Y3;
output wire Y4;
output wire Y5;
output wire Y6;
assign Y1 = {(&A1[1:0])};
assign Y2 = {(|A2[1:0])};
assign Y3 = {(~&A3[1:0])};
assign Y4 = {(~|A4[1:0])};
assign Y5 = {(^A5[1:0])};
assign Y6 = {(~^A6[1:0])};
endmodule
