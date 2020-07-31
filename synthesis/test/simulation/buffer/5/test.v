module test(input in, output [1:0] out);
assign out = func(in)*func(in);

function func(input in);
begin
    func = in;
end
endfunction
endmodule
