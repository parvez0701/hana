module test(in, out, sel);
output [1:0] out;
input sel;
input  in;

assign out[sel] = in;
endmodule
