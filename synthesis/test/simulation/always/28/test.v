module test(input in1, in2, in3, output reg out1, out2);

always @(in1 or in2 or in3) begin
    out1 = in1&in2;
	out2 = out1;
	out1 = out2 | in3;
end
endmodule
