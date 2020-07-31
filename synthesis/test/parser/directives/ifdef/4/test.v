`define X 4
module test(input in, output out);

`ifndef X               
assign out = in;
`endif

endmodule

