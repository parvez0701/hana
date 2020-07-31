module test(input  [3:0] in, output [7:0] out1, out2, output [15:0] out3);
assign out1 = func(in);
assign out2 = func(3);
assign out3 = func(5)**2;

function [7:0] func(input [3:0] in);
    begin
	    func = in + in;
	end
endfunction	
endmodule
