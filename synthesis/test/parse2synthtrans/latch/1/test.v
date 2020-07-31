module test(in, out, clk);
input in;
output reg out;
input clk;

always @(clk)
    if(clk)
        out = in;
        
endmodule
