module test(input in1, in2, clk, rst, set, output reg q1, q2);

always @ (posedge clk or posedge rst or posedge set)
    if(rst)
	    q1 <= 0;
	else begin
	    q1 <= in1;
		q2 <= in2;
	end
endmodule	
	    
