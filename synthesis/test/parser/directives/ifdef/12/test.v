
`include "inc.v"

module test(input in, output out);

`ifdef RAM_MEM_WIDTH 
assign out = in;
`endif
endmodule
