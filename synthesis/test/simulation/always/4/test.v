module test(input in1, in2, en, output reg out);
reg tmp;
always @(in1 or in2 or en) begin
    if(en) out = in1;
	out = in2;
end	
endmodule	
