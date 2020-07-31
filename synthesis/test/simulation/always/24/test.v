module test(input IN1, IN2, input EN, output reg OUT);
always @(IN1 or IN2 or EN) 
	case (EN)
		0: OUT = IN1;
		1: OUT = IN2;
	endcase	

endmodule	
