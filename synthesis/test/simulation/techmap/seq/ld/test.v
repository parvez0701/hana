module latch(input d, en, output reg q);
    always @(en)
	    if(en)
		    q <= d;
endmodule			
