`define X 4
module test(input in1, in2, output out);

`ifdef X               
`ifndef Y
assign out = in1;
`endif
`else
assign out = in2;
`endif

endmodule

