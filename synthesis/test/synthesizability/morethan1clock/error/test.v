module test(in, out, clk, reset);
input in, reset, clk;
output reg out;

always @(posedge clk, negedge reset)
	out <= in;

endmodule
