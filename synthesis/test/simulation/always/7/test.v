module test(input in1, in2, en1, en2, output reg out);
always @(in1 or in2 or en1 or en2) begin
    if(en1) out = in1;
    if(en2) out = in2;
end	
endmodule	
