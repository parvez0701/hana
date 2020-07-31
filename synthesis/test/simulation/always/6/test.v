module test(input in1, in2, en, output reg out);
reg tmp;
always @(in1 or in2 or en) begin
	out = in2;
    if(en) out = in1;
end	
endmodule	
