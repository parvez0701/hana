module test(input d1, d2, en1,en2, output reg q);

always @(d1 or d2 or en1 or en2) begin
    if(en1)
	    q = d1;
    if(en2)
	    q = d2;
end		
endmodule		
