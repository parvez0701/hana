module test(in, out, clk);
input in;
output reg out;
input clk;

always @(posedge clk or negedge clk or clk) begin: named_block
    reg in;
    out <= in;
end

endmodule
