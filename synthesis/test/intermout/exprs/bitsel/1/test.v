module test(in, out, sel);
output out;
input sel;
input [1:0] in;

assign out = in[sel];
endmodule
