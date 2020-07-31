module test(input   in, output out1, out2);

assign out1 = func(0);
assign out2 = func(1);

function  func(input  in);
    begin
	    if(in) func = 0;
		else func = 1;
	end
endfunction	

endmodule
