module test(in, clk, out);
        input wire in;
    input wire clk;
    output wire out;

// Variable declaration.

    specify
        (posedge clk  => (out +: in)) =  (10:8:6 );
    endspecify
    specify
        specparam d_to_q = 9;
        specparam clk_to_q = 11;
    endspecify

endmodule
