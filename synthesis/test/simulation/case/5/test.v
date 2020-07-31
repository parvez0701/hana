module test(input [3:0] in1, in2, sel, output reg [3:0] out);

integer i;
always @(in1, in2, sel) begin
    i = sel;
	case (i)
	    0: out = in1;
	    1: out = in2;
		default:
		    out = 0;
	endcase
end
endmodule	
	
	
