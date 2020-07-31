module test(input IN, output [1:0] OUT);
assign OUT = func1(IN)*func2(IN);

function func1(input in);
begin
    func1 = in;
end
endfunction
function func2(input in);
begin
    func2 = ~in;
end
endfunction
endmodule
