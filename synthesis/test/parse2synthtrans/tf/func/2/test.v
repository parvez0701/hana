module test(input [1:0] in, output [1:0] out);
assign out = func(in);

function [1:0] func(input [3:0] in);
begin
    func = 15;
end

endfunction
endmodule
