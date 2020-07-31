module test(clk, in, out, reset);
input in;
input clk;
input reset;
output out;

wire  synth_net_10;
wire  synth_net_11;
INV synth_NOT_0(.in(reset), .out(
    synth_net_11));
RFF synth_FFR(.d(synth_net_10), .clk(clk), .reset(
    synth_net_11), .q(out));
BUF synth_BUF(.in(in), .out(synth_net_10));

endmodule

