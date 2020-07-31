module test(input   in, output  out);

always @(in)
    func(0);

function  func(input  in1);
    begin
		out = in;
	    func = in1;
	end
endfunction	
endmodule
