module test(input in1, in2, input [1:0] sel, output reg  out);

always @(in1, in2, sel)
	case (sel)
	    0: out = in1;
	    1: out = in2;
		default:
		    out = 0;
	endcase

endmodule	
	
	
