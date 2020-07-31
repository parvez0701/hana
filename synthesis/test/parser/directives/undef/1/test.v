//Should be able to parse
`define XYZ

`ifdef XYZ
module test();
`endif
wire w;
`undef XYZ

`ifndef XYZ
endmodule
`endif

