module test(out, vin, out1, vin1);

output wire out;
input wire [1:0]vin;
output wire out1;
input wire [3:0]vin1;
assign out = {(&vin[1:0])};
assign out1 = {(&vin1[3:0])};
endmodule
