module test(input in, sel, output out, output reg myreg);
assign out = func(in, sel) * func(in, sel);

function func(input in, sel);
begin
    if(sel)begin
        myreg = in;
		func = in;
	end	
end
endfunction
endmodule
