module test(output out, input in1, in2);
assign out = ~(in1 | in2);
endmodule
