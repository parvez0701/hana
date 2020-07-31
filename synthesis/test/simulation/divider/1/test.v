module test(input in1, in2, output reg  out);

always @ (in1 or in2)
    out = in1/in2;

endmodule	
