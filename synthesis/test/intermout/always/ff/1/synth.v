module FF(q, d, clk);

output reg q;
input wire d;
input wire clk;
wire synth_net_0;
wire synth_net_1;
VCC_0_1 synth_VCC(.OUT(synth_net_0));
AND_2_1 synth_AND(.OUT(synth_net_1), .IN({synth_net, synth_net_0}));
FF_2_1 synth_FF(.Q(q), .D(synth_net_1), .CLK(clk));
endmodule
