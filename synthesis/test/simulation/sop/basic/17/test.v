module test(input in, clk, output out);
reg w;
assign out = w;
always @(posedge clk)
    w <= ~in;
endmodule
