module multiplier(product, a0, a1);
    // Parameter declarations.
    parameter signed [31:0]a0_width = 8;
    parameter signed [31:0]a1_width = 8;
// Localparam declarations.
    localparam signed product_width = (a0_width+a1_width);
    output wire [(product_width-1):0] product ;
    input wire [(a0_width-1):0] a0 ;
    input wire [(a1_width-1):0] a1 ;

// Variable declaration.

//Module instantiations.
    tree_multipler #( a0_width, a1_width) gen_m0(product, a0, a1);

endmodule
