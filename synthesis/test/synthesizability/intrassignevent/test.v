module test(in1, in2, out1, out2, clk1, clk2);
input in1, in2, clk1, clk2;
output reg out1, out2;

always @(posedge clk1) begin
	out1 <= in1;
	out2 <= @(posedge clk2) in2;
end	
endmodule
