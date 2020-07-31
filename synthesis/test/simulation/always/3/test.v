module test(input in1, in2, output reg out1, out2);
reg tmp;
always @(in1 or in2) begin
    tmp = in1;
    out1 = tmp;
	tmp = in2;
    out2 = tmp;
end	
endmodule	
