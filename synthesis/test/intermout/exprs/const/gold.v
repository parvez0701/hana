module test(out, vout);

output wire out;
output wire [7:0]vout;
VCC_0_1 synth_VCC(.OUT(out));
GND_0_1 synth_GND(.OUT(vout[7]));
GND_0_1 synth_GND_0(.OUT(vout[6]));
GND_0_1 synth_GND_1(.OUT(vout[5]));
GND_0_1 synth_GND_2(.OUT(vout[4]));
VCC_0_1 synth_VCC_0(.OUT(vout[3]));
GND_0_1 synth_GND_3(.OUT(vout[2]));
GND_0_1 synth_GND_4(.OUT(vout[1]));
VCC_0_1 synth_VCC_1(.OUT(vout[0]));
endmodule
