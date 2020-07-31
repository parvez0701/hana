module test(in1, in2, vin1, vin2, out, vout, vin3, vin4, vout1);

input wire in1;
input wire in2;
input wire [1:0]vin1;
input wire [3:0]vin2;
output wire out;
output wire vout;
input wire [1:0]vin3;
input wire [3:0]vin4;
output wire vout1;
assign out = (in1&&in2);
assign vout = ({0, 0, vin1[1:0]}&&vin2[3:0]);
assign vout1 = ({0, 0, vin3[1:0]}||vin4[3:0]);
endmodule
