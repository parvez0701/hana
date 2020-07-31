module test(input d, clk, reset, set, output reg q);
always @(negedge clk or posedge reset or posedge set) 
    if(reset)
	    q <= 0;
	else if(set)
	    q <= 1;
	else	
        q <= d;
endmodule	
