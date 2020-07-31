module test(input in1, in2, output reg [1:0] out);

always @ (in1 or in2)
    out = in1-in2;

endmodule	
