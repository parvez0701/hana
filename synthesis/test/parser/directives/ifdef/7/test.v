`define X 4
module test(input in1, in2, output out);

`ifndef Y               
assign out = in1;
`else
assign out = in2;
`endif

endmodule

