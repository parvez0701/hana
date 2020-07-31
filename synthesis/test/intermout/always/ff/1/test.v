module FF (q, d, clk);
input clk, d;
output reg q;

always @( posedge clk)
	q <= d;
endmodule	
