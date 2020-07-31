module test(input IN1, IN2, IN3, IN4, EN1, EN2, output reg OUT);
always @(IN1 or IN2 or EN1 or EN2) begin
    if(EN1) begin
	    if(EN2)
		    OUT = IN1;
		else
		    OUT = IN2;
	end		
	else
	    if(EN2)
	        OUT = IN3;
		else
		    OUT = IN4;
end	
endmodule	
