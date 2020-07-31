module test(input d, clk, set, output reg q);
always @(posedge clk or negedge set) 
    if(!set)
	    q <= 1;
	else	
        q <= d;
endmodule	
