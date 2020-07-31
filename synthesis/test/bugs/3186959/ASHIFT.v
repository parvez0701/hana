/* Testing of Arithmatic Shift operator */
`timescale 1ns/1ns

module ASHIFT(in, in2, out1, out2);
input [15:0] in;
input signed [15:0] in2;
output [5:0] out1, out2;
reg  signed [5:0] out1, out2 ;

function signed [9:0]  lShift;
input signed [5:0] x;
   lShift = x + in2 <<< 2'b11;
endfunction

function signed [9:0]  rShift;
input signed [5:0] x;
   rShift = x + in2 >>> 2'b11;
endfunction

always @(in or in2)
begin : B1
    out1 = lShift(in);
    out2 = rShift({in2});
end

endmodule

module TEST(out1, out2, in, in2);
output signed [5:0] out1, out2;
input [15:0] in;
input signed [15:0] in2;

ASHIFT inst1 (.in(in), .in2(in2), .out1(out1), .out2(out2));

endmodule
