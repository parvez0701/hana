module test(input [3:0] in1, in2, output equal, notequal, lesser, greater, le, ge);

assign equal = (in1 == in2);
assign notequal = (in1 != in2);
assign lesser = (in1 < in2);
assign greater = (in1 > in2);
assign le = in1 <= in2;
assign ge = in1 >= in2;

endmodule
