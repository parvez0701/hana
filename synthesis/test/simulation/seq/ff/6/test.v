module test(input  in,  input clk,  reset, output reg out);
always @(negedge clk, negedge reset)
    if(!reset) out <= 1'b0;
	else out <= in;
endmodule	
