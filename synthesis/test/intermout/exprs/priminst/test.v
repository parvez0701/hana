module prim(in0, out0, in1, out1, in2, out2, sel2, out3, in3, sel3, in4, in5, out4, out5, sel4, sel5);
input in0, in1, in2, in3, sel2, sel3;
output out0, out1, out2, out3;

buf(out0, in0);
not(out1, in1);

notif0(out2, in2, sel2);
notif1(out3, in3, sel3);
bufif0(out4, in4, sel4);
bufif1(out5, in5, sel5);
endmodule
