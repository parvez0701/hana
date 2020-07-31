module test(in, out);

input wire [3:0]in;
output wire [3:0]out;
reg [3:0] func_0;
assign out[3:0] = 15;
function [3:0] func(in_0);

    begin
    func_0[3:0] = -1;
    end
endfunction
function [3:0] synth_func(in_0);

    begin
    15 = -1;
    end
endfunction
endmodule
