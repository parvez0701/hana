module test(input [1:0] in1, in2, output reg [1:0] out);

always @ (in1 or in2)
    out = in1%in2;

endmodule	
