module test(in, out, clk, reset);
input in, reset, clk;
output reg out;

always @(posedge clk, negedge reset)
	if(!reset)
		out <= 1'b0;
	else	
	    out <= in;

endmodule  
