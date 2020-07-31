module test(out, vout, in, vin);

output wire out;
output wire vout;
input wire in;
input wire [3:0]vin;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
NOT_1_1 synth_NOT(.OUT(out), .IN(in));
OR_2_1 synth_OR(.OUT(synth_net_0), .IN({vin[3], vin[2]}));
OR_2_1 synth_OR_0(.OUT(synth_net_1), .IN({vin[1], synth_net_0}));
OR_2_1 synth_OR_1(.OUT(synth_net_2), .IN({vin[0], synth_net_1}));
NOT_1_1 synth_NOT_0(.OUT(vout), .IN(synth_net_2));
endmodule
