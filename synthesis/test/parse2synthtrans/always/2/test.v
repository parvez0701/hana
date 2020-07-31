module test(in, out, clk, reset);
input in, reset;
output reg out;
input clk;

always @(posedge clk) begin
    out = in;
    out <= reset;
end
endmodule
