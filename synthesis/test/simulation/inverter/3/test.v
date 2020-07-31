module test(input in1, in2, output out1, out2);
wire w1, w2;

assign w1 = ~in1;
assign w2 = !in2;
assign out1 = !w1;
assign out2 = ~w2;
endmodule
