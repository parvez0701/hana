module test(input [3:0] in1, in2, output reg [3:0] out);

always @ (in1 or in2)
    out = in1/in2;

endmodule	
