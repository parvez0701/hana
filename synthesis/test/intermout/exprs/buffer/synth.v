module buffer(in, out, vin, vout);

input wire in;
output wire out;
input wire [1:0]vin;
output wire [1:0]vout;
assign out = in;
assign vout[1:0] = vin[1:0];
endmodule
