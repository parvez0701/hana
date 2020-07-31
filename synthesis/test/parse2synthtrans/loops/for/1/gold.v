module test(in, clk, out);

input wire in;
input wire clk;
output reg [5:0]out;
reg signed [31:0] i;
always @(posedge clk) begin
    out[0] = in;
    out[1] = out[2];
    out[1] = in;
    out[2] = out[3];
    out[2] = in;
    out[3] = out[4];
    out[0] = in;
    out[0] = in;
    out[0] = in;
    out[0] = in;
    out[0] = in;
end
endmodule
