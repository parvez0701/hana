module test(input in1, in2, output out);

//Y shaped buffers
wire w1, w2, w3;
assign w1 = in1;
assign w1 = in2;
assign out = w1;
endmodule
