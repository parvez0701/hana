module test(in, out, clk, reset);
input in, reset;
output reg out;
input clk;

always @(posedge clk or posedge reset)
    out = in;
always @(negedge clk)
    out <= in;
endmodule
