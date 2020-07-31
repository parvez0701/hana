module test(input d, clk, rst, output reg q);
always @(negedge clk or posedge rst) 
    if(rst)
	    q <= 0;
	else	
        q <= d;
endmodule	
