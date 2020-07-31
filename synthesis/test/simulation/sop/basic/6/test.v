module test(input in1, in2, in3, output out);
assign out = ~((in1 | in2) & in3);
endmodule
