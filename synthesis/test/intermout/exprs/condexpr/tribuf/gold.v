module test(in, out, en, vout1, en1, vin1);

input wire in;
output wire out;
input wire en;
output wire [1:0]vout1;
input wire en1;
input wire [1:0]vin1;
TRIBUF_2_1 synth_TRIBUF(.OUT(out), .IN(in), .EN(en));
TRIBUF_2_1 synth_TRIBUF_0(.OUT(vout1[1]), .IN(vin1[1]), .EN(en1));
TRIBUF_2_1 synth_TRIBUF_1(.OUT(vout1[0]), .IN(vin1[0]), .EN(en1));
endmodule
