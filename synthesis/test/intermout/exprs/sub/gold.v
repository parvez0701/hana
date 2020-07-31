module test(out, in1, in2, vin1, vin2, vout1);

output wire out;
input wire in1;
input wire in2;
input wire [1:0]vin1;
input wire [2:0]vin2;
output wire [3:0]vout1;
wire synth_net;
wire synth_net_0;
wire synth_net_2;
wire synth_net_3;
wire synth_net_4;
wire synth_net_5;
wire synth_net_6;
GND_0_1 synth_GND(.OUT(synth_net));
SUB_1_1 synth_SUB(.BOUT(synth_net_0), .C(out), .A(in1), .B(in2), .BIN(synth_net)
);
GND_0_1 synth_GND_0(.OUT(synth_net_2));
GND_0_1 synth_GND_1(.OUT(synth_net_3));
GND_0_1 synth_GND_2(.OUT(synth_net_4));
GND_0_1 synth_GND_3(.OUT(synth_net_5));
SUB_4_4 synth_SUB_0(.BOUT(synth_net_6), .C(
    {synth_net_7, synth_net_8, synth_net_9, synth_net_10}), .A(
    {synth_net_2, synth_net_3, vin1[1], vin1[0]}), .B(
    {synth_net_4, vin2[2], vin2[1], vin2[0]}), .BIN(synth_net_5));
endmodule
