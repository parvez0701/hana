module test(in, out, clk, reset);
input in, reset;
output reg out;
input clk;

always @(posedge clk or negedge reset) begin
if(!reset)
    out = 1'b0;
else
    out <= reset;
end
always @(posedge clk or negedge reset) begin
    out = 1'b0;
    out = reset;
end
endmodule
