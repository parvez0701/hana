module test(input in, output [1:0] out);
assign out = func1(in);

function func1(input in);
begin
    func1 = in*func2(in);
end
endfunction
function func2(input in);
begin
    func2 = ~in;
end
endfunction
endmodule
