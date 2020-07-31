module test(q, d, clk, reset);
        output reg q;
    input wire d;
    input wire clk;
    input wire reset;

// Variable declaration.

    always @ (*
)if ( (!reset))
    q = 0;
else 
    q = d;
    always @ (*
)if ( (!reset))
    q = 0;
else 
    q = d;

endmodule
