module test(input in, output out1, out2);

//Y shaped buffers
wire w1, w2, w3;
assign w1 = in;
assign w2 = w1;
assign w3 = w1;
assign out1 = w2;
assign out2 = w3;
endmodule
