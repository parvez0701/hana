module test(input in1, in2, in3, in4, in5, in6, in7, in8, in9, 
    output out1, out2, out3, out4, out5);
assign out1 = ~(in1&in2);
assign out2 = ~(in3 | in4);
assign out3 = ~(in5^in6);
assign out4 = ~(in7~^in8);
assign out5 = ~(!in9);

endmodule
