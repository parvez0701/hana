module test(input IN1, IN2, IN3, IN4, EN1, EN2, EN3, output reg OUT);
always @(IN1 or IN2 or EN1 or EN2, EN3) begin
    if(EN1) begin
	    if(EN2) begin
		    OUT = IN2;
			if(EN3)
			    OUT = IN3;
		end	
		else 
		    OUT = IN4;
	end		
	else
	    OUT = IN1;
end	
endmodule	
