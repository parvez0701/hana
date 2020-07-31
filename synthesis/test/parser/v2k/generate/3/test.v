module multiplier(product, a0, a1);

parameter a0_width = 8;
parameter a1_width = 8;

localparam product_width = a0_width + a1_width;

output [product_width -1: 0] product;
input [a0_width-1:0] a0;
input [a1_width-1:0] a1;

generate
	if((a0_width < 8) || (a1_width < 8))
		cla_multiplier #( a0_width, a1_width) m0 (product, a0, a1);
	else
		tree_multipler #(a0_width, a1_width) m0 (product, a0, a1);
endgenerate
endmodule
