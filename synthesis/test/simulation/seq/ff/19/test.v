module test(input  in,  input clk,  cond, set, reset, output reg out);
always @(posedge clk or negedge set or negedge reset)
    if(!set) out <= 1'b1;
	else if(!reset) out <= 1'b0;
	else if(cond) out <= in;
	
endmodule	
