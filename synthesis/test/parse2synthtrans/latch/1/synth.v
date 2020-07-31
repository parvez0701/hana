module test(in, out, clk);

input wire in;
output reg out;
input wire clk;
always @(clk) begin
    if(clk) begin

        out = in;
    end
end
endmodule
