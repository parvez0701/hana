module test(in, out, vin, vout, vin1, vout1, vin2, vout2);

input wire in;
output wire out;
input wire [3:0]vin;
output wire [3:0]vout;
input wire [3:0]vin1;
output wire [3:0]vout1;
input wire [3:0]vin2;
output wire [3:0]vout2;
GND_0_1 synth_GND(.OUT(out));
GND_0_1 synth_GND_0(.OUT(vout[1]));
GND_0_1 synth_GND_1(.OUT(vout[0]));
BUF_1_1 synth_BUF_0(.OUT(vout[3]), .IN(vin[1]));
BUF_1_1 synth_BUF_1(.OUT(vout[2]), .IN(vin[0]));
GND_0_1 synth_GND_2(.OUT(vout1[3]));
GND_0_1 synth_GND_3(.OUT(vout1[2]));
BUF_1_1 synth_BUF_6(.OUT(vout1[1]), .IN(vin1[3]));
BUF_1_1 synth_BUF_7(.OUT(vout1[0]), .IN(vin1[2]));
GND_0_1 synth_GND_4(.OUT(vout2[3]));
GND_0_1 synth_GND_5(.OUT(vout2[2]));
BUF_1_1 synth_BUF_10(.OUT(vout2[1]), .IN(vin2[3]));
BUF_1_1 synth_BUF_11(.OUT(vout2[0]), .IN(vin2[2]));
endmodule
