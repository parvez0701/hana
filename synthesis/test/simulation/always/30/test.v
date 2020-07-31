module test(input [3:0] in1, in2, in3, input clk, output reg out1, out2, out3);

always @(posedge clk) begin
	out1 <= ~|in1;
	out2 <= ~&in2;
	out3 <= ~^in3;
end

endmodule
