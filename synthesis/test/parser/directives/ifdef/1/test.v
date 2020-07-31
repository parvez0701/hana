`define X 4
module test(input in, output out);

`ifdef X               
assign out = in;
`endif

endmodule

