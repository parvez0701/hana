module test(output out, input in1, in2, in3, in4, in5);
assign out = ~(in1 & in2 & in3 & in4 & in5);
endmodule
