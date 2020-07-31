module test(input in1, in2, en, sel, output reg out);
always @(in1 or in2 or en or sel ) 
	case (sel)
		0: if(en) out = in1;
		1: out = in2;
	endcase	

endmodule	
