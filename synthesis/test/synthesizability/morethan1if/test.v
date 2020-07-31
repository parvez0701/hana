module test(in, out, clk, reset);
input [1:0] in;
input clk, reset;

output reg [1:0] out;

always @(posedge clk1,  negedge reset)
begin
	if(!reset) out[0] = 1'b0;
	else out[0] = in[0];

	if(!reset) out[1] = 1'b0;
	else out[1] = in[1];
end

endmodule


