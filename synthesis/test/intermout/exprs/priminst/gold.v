module prim(in0, out0, in1, out1, in2, out3, in3, out2, sel2, sel3);

input wire in0;
output wire out0;
input wire in1;
output wire out1;
input wire in2;
output wire out3;
input wire in3;
output wire out2;
input wire sel2;
input wire sel3;
wire out4;
wire in4;
wire sel4;
wire out5;
wire in5;
wire sel5;
buf(out0, in0);
not(out1, in1);
notif0(out2, in2, sel2);
notif1(out3, in3, sel3);
bufif0(out4, in4, sel4);
bufif1(out5, in5, sel5);
endmodule
