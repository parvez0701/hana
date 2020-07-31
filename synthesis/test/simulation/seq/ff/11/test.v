module test(input  in,  input clk,   set, output reg out);
always @(posedge clk, negedge set)
	if(!set) out <= 1'b1;
	else out <= in;
endmodule	
