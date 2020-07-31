module adder(co, sum, a0, a1, ci);
    // Parameter declarations.
    parameter signed [31:0]N = 4;
    output wire co;
    output wire [(N-1):0] sum ;
    input wire [(N-1):0] a0 ;
    input wire [(N-1):0] a1 ;
    input wire ci;

// Variable declaration.

//Module instantiations.
    adder_cla #( N) gen_adder3(co, sum, a0, a1, ci);

endmodule
