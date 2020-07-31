module test(out, in1, in2, vin1, vin2, vout1);

output wire out;
input wire in1;
input wire in2;
input wire [1:0]vin1;
input wire [2:0]vin2;
output wire [3:0]vout1;
wire synth_net_0;
wire synth_net_1;
wire synth_net_2;
wire synth_net_3;
wire synth_net_8;
wire synth_net_9;
wire synth_net_10;
wire synth_net_11;
DIV_1_1 synth_DIV(.REM(out), .C(synth_net_0), .A(in1), .B(in2));
GND_0_1 synth_GND(.OUT(synth_net_1));
GND_0_1 synth_GND_0(.OUT(synth_net_2));
GND_0_1 synth_GND_1(.OUT(synth_net_3));
DIV_4_4 synth_DIV_0(.REM({synth_net_4, synth_net_5, synth_net_6, synth_net_7}), 
    .C({synth_net_8, synth_net_9, synth_net_10, synth_net_11}), .A(
    {synth_net_1, synth_net_2, vin1[1], vin1[0]}), .B(
    {synth_net_3, vin2[2], vin2[1], vin2[0]}));
endmodule
