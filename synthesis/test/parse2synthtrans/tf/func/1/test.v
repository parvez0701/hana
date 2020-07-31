module test(input [3:0] in, output [3:0] out);
assign out = func(in);

function [3:0] func(input [1:0] in);
begin
    func = 15;
end

endfunction
endmodule
