module test(input [1:0] in, sel, output reg [1:0] out);

always @(sel or in)
    if(sel) begin
	    out[0] = in[0];
		out[1] = in[1];
	end	
	else begin
	    out[0] = in[1];
		out[1] = in[0];
	end

endmodule	
