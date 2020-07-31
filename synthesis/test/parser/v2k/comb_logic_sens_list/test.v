module test(q, d, clk, reset);
output reg q;
input d, clk, reset;

always @* 
	if(!reset) q = 0;
	else q = d;

always @ (*) 
	if(!reset) q = 0;
	else q = d;
endmodule
