module test(input d, clk, output reg q);
always @(negedge clk) 
    q <= d;
endmodule	
