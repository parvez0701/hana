module test(in1, in2, vout2, en1, ven1, ven2, out, vin1, vin2, vin3, vin4, vout1
);

input wire in1;
input wire in2;
output wire [1:0]vout2;
input wire en1;
input wire ven1;
input wire [1:0]ven2;
output wire out;
input wire [1:0]vin1;
input wire [1:0]vin2;
input wire [1:0]vin3;
input wire [1:0]vin4;
output wire [1:0]vout1;
wire synth_net_2;
MUX_2_1 synth_MUX(.OUT(out), .IN({in1, in2}), .SEL(en1));
MUX_2_1 synth_MUX_0(.OUT(vout1[1]), .IN({vin1[1], vin2[1]}), .SEL(ven1));
MUX_2_1 synth_MUX_1(.OUT(vout1[0]), .IN({vin1[0], vin2[0]}), .SEL(ven1));
OR_2_1 synth_OR(.OUT(synth_net_2), .IN({ven2[1], ven2[0]}));
MUX_2_1 synth_MUX_2(.OUT(vout2[1]), .IN({vin3[1], vin4[1]}), .SEL(synth_net_2));
MUX_2_1 synth_MUX_3(.OUT(vout2[0]), .IN({vin3[0], vin4[0]}), .SEL(synth_net_2));
endmodule
