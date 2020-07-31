module test(input  in,  input clk,  cond, set, output reg out);
always @(posedge clk)
    if(!set) out <= 1'b1;
	else if(cond) out <= in;
	
endmodule	
