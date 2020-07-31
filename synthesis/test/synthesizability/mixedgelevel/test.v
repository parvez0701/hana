module test(in1, in2, out1, out2, clk);
input in1, in2, clk;
output reg out1, out2;
wire tmp;

always @(posedge clk, tmp) begin
	out1 <= in1;
	out2 <= in2;
end	
endmodule
