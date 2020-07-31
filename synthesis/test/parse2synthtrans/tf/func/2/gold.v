module test(in, out);

input wire [1:0]in;
output wire [1:0]out;
reg [1:0] func_0;
assign out[1:0] = 3;
function [1:0] func(in_0);

    begin
    func_0[1:0] = -1;
    end
endfunction
function [1:0] synth_func(in_0);

    begin
    3 = -1;
    end
endfunction
endmodule
