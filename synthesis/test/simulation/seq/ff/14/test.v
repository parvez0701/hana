module test(input  in,  input clk,  reset, set, output reg out);
always @(posedge clk, negedge reset, negedge set)
    if(!reset) out <= 1'b0;
	else if(!set) out <= 1'b1;
	else out <= in;
endmodule	
