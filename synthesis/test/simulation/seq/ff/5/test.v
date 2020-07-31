module test(input  ABCD,  input clk,  reset, output reg out);
always @(posedge clk, negedge reset)
    if(!reset) out <= 1'b0;
	else out <= ABCD;
endmodule	
