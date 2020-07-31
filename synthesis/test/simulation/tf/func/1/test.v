module test(input in, clk, output reg OUT);

//assign OUT = func(in);
always @(posedge clk)
    OUT = func(in);


function  func(input IN);
begin
    func = IN;
end
endfunction

endmodule	
