module test(in, clk, reset, out);

input wire in;
input wire clk;
input wire reset;
output reg out;
reg signed [3:0] a;
reg signed [3:0] b;
reg signed [3:0] c;
reg [5:0] d;
reg [5:0] e;
always @(clk, reset) begin
    a[3:0] = -4;
    b[3:0] = 2;
    c[3:0] = -2;
    d[5:0] = -4;
    d[5:0] = 16;
    e[5:0] = 0;
end
endmodule
