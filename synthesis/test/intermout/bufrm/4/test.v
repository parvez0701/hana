module test(input in1, in2, output out1, out2);

//X shaped buffers
wire w1, w2, w3, w4, w5, w6;
assign w1 = in1;
assign w2 = in2;
assign w3 = w1;
assign w3 = w2;
assign w4 = w3;
assign w5 = w3;
assign out1 = w4;
assign out2 = w5;
endmodule
