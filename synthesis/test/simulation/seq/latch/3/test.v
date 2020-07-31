
module test(input IN, input [1:0] SEL, output reg OUT);
    always @(IN or SEL)
	    case (SEL)
			0: OUT = IN;
		endcase	

endmodule
