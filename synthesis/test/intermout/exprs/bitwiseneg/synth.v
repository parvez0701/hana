module test(out, in, vout, vin);

output wire out;
input wire in;
output wire [1:0]vout;
input wire [1:0]vin;
assign out = (~in);
assign vout[1:0] = (~vin[1:0]);
endmodule
