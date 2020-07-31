module test(input d, clk, preset, output reg q);

always @(posedge clk or negedge preset)
    if(!preset)
	    q <= 1'b0;
	else
	    q <= d;
endmodule		
