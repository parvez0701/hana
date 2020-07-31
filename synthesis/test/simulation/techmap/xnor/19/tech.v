module TECH_XNOR2(input [1:0] in, output out);
assign out = in[0] ~^ in[1];
endmodule
