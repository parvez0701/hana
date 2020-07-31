module test(input d, clk, rst, output reg q);
always @(negedge clk or negedge rst) 
    if(!rst)
	    q <= 0;
	else	
        q <= d;
endmodule	
