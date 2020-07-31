module test1(in, out, clk);
input in, clk;
output reg out;

always @(posedge clk) begin
	out <= in;
	out = in;
end	
endmodule

module test2(in, out, clk);
input [1:0] in; 
input clk;
output reg [1:0] out;

always @(posedge clk) begin
	out[0] <= in[0];
	out[1] = in[1];
end	
endmodule
