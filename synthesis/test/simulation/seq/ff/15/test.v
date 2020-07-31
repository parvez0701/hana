module test(input  in,  input clk,  reset, set, output reg out);
always @(posedge clk, posedge reset, negedge set)
    if(!set) out <= 1'b0;
	else if(reset) out <= 1'b1;
	else out <= in;
endmodule	
