module test(clk, in1, in2, out1, out2);
input in1;
input in2;
input clk;

output out2;
output out1;
wire  synth_net_3;
wire  synth_net_4;
FF synth_FF(.d(
    synth_net_3), .clk(clk), .q(out2));
FF synth_FF_0(.d(synth_net_4), .clk(clk)
    , .q(out1));
BUF synth_BUF(.in(in1), .out(synth_net_4));
BUF synth_BUF_0(.in
    (in2), .out(synth_net_3));
endmodule

