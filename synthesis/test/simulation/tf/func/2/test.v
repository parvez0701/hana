module test(input [1:0] in,  output reg  [1:0] OUT);

//assign OUT = func(in);
always @(in)
    OUT = func(in);


function [1:0] func(input [1:0] IN);
begin
    func = IN;
end
endfunction

endmodule	
