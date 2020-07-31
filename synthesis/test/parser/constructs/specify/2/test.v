module test(input in, clk, output out);

specify
	(posedge clk => (out +: in)) = (10 : 8 : 6);
endspecify

specify
	specparam d_to_q = 9;
	specparam clk_to_q = 11;
endspecify
endmodule

