module test(input in1, in2, en, sel1, sel2, output reg out);
always @(in1 or in2 or en or sel1 ) 
	case (sel1)
		0: if(en) out = in1;
		1: case (sel2)
			0: out = in2;
		   endcase	
	endcase	

endmodule	
