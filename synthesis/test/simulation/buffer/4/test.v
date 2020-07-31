module test(input in, output out);
assign out = func(in);

function func(input in);
begin
    func = in;
end
endfunction
endmodule
