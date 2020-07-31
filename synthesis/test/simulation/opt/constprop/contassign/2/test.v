module test(input  [1:0] in, output [2:0] out);
assign out = func(2);

function [2:0] func(input [1:0] in);
    begin
	    func = in + in;
	end
endfunction	
endmodule
