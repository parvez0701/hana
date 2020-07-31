module test(out, in, vout, vin);

output wire out;
input wire in;
output wire [31:0]vout;
input wire [31:0]vin;
assign out = (-in);
assign vout[31:0] = (-vin[31:0]);
endmodule
