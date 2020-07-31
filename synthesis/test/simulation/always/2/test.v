module test(input in1, in2, output reg out);
always @(in1 or in2) begin
    out = in1;
    out = in2;
end	
endmodule	
