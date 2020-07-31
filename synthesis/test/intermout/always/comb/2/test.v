module And(input in1, in2, output reg out);

always @( in1)
	out = in1 & in2;
endmodule	
