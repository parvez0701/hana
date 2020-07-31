module NonBlockingEx(clk, merge, er, xmit, fddi, claim);

input wire clk;
input wire merge;
input wire er;
input wire xmit;
input wire fddi;
output reg claim;
wire synth_net;
wire synth_net_1;
wire synth_net_2;
wire synth_net_4;
wire synth_net_5;
wire synth_net_7;
wire synth_net_8;
wire synth_net_9;
wire synth_net_10;
wire synth_net_11;
wire synth_net_12;
reg fcr;
OR_2_1 synth_OR(.OUT(synth_net), .IN({er, xmit}));
VCC_0_1 synth_VCC(.OUT(synth_net_1));
AND_2_1 synth_AND(.OUT(synth_net_2), .IN({fcr, fddi}));
VCC_0_1 synth_VCC_0(.OUT(synth_net_4));
AND_2_1 synth_AND_0(.OUT(synth_net_5), .IN({synth_net_3, synth_net_4}));
VCC_0_1 synth_VCC_1(.OUT(synth_net_7));
AND_2_1 synth_AND_1(.OUT(synth_net_8), .IN({synth_net_6, synth_net_7}));
MUX_2_1 synth_MUX(.OUT(synth_net_9), .IN({synth_net_5, synth_net_8}), .SEL(merge
    ));
MUX_2_1 synth_MUX_0(.OUT(synth_net_10), .IN({synth_net_4, synth_net_7}), .SEL(
    merge));
AND_2_1 synth_AND_2(.OUT(synth_net_11), .IN({synth_net_9, synth_net_10}));
FF_2_1 synth_FF(.Q(claim), .D(synth_net_11), .CLK(clk));
AND_2_1 synth_AND_3(.OUT(synth_net_12), .IN({synth_net_0, synth_net_1}));
FF_2_1 synth_FF_0(.Q(fcr), .D(synth_net_12), .CLK(clk));
endmodule
