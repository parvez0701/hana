module test(input  [3:0] in, output [7:0] out);
assign out = func(5);

function [7:0] func(input [3:0] in);
    begin
	    func = in * in;
	end
endfunction	
endmodule
