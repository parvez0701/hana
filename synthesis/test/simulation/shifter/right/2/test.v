module test(input [1:0] IN, SHIFT, output [1:0] OUT);

assign OUT = IN >> SHIFT;
endmodule
