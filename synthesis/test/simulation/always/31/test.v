module test(input cond, clk, output reg data);

reg tmp;
always @(cond)
    if(cond)
	    tmp = 1;
	else
	    tmp = data;

always @ (posedge clk)
   data <= tmp;

endmodule
