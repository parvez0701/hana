module inc(clock, counter);

input wire clock;
output reg [3:0]counter;
wire synth_net;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
wire synth_net_4;
wire synth_net_5;
wire synth_net_6;
wire synth_net_7;
wire synth_net_8;
wire synth_net_10;
wire synth_net_12;
wire synth_net_14;
wire synth_net_16;
wire synth_net_17;
wire synth_net_18;
wire synth_net_19;
wire synth_net_20;
GND_0_1 synth_GND(.OUT(synth_net));
GND_0_1 synth_GND_0(.OUT(synth_net_0));
GND_0_1 synth_GND_1(.OUT(synth_net_1));
VCC_0_1 synth_VCC(.OUT(synth_net_2));
GND_0_1 synth_GND_2(.OUT(synth_net_3));
ADD_4_4 synth_ADD(.COUT(synth_net_4), .C(
    {synth_net_5, synth_net_6, synth_net_7, synth_net_8}), .A(
    {counter[3], counter[2], counter[1], counter[0]}), .B(
    {synth_net, synth_net_0, synth_net_1, synth_net_2}), .CIN(synth_net_3));
VCC_0_1 synth_VCC_0(.OUT(synth_net_10));
VCC_0_1 synth_VCC_1(.OUT(synth_net_12));
VCC_0_1 synth_VCC_2(.OUT(synth_net_14));
VCC_0_1 synth_VCC_3(.OUT(synth_net_16));
AND_2_1 synth_AND(.OUT(synth_net_17), .IN({synth_net_9, synth_net_10}));
FF_2_1 synth_FF(.Q(counter[3]), .D(synth_net_17), .CLK(clock));
AND_2_1 synth_AND_0(.OUT(synth_net_18), .IN({synth_net_11, synth_net_12}));
FF_2_1 synth_FF_0(.Q(counter[2]), .D(synth_net_18), .CLK(clock));
AND_2_1 synth_AND_1(.OUT(synth_net_19), .IN({synth_net_13, synth_net_14}));
FF_2_1 synth_FF_1(.Q(counter[1]), .D(synth_net_19), .CLK(clock));
AND_2_1 synth_AND_2(.OUT(synth_net_20), .IN({synth_net_15, synth_net_16}));
FF_2_1 synth_FF_2(.Q(counter[0]), .D(synth_net_20), .CLK(clock));
endmodule
