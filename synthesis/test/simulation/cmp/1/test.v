module test(input in1, in2, output equal, notequal, lesser, greater);

assign equal = (in1 == in2);
assign notequal = (in1 != in2);
assign lesser = (in1 < in2);
assign greater = (in1 > in2);

endmodule
