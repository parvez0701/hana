module FF(clk, a, b, c, e);
input clk, a, b, c;
output e;
reg e, d;

always @(posedge clk)
    e <= d | c;


always @( negedge clk)
    d <= a & b;

endmodule
