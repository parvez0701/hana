module M(out, src, dest, a, b, c, d);
        output wire out;
    input wire [3:0] src ;
    output wire dest;
    input wire a;
    input wire b;
    input wire c;
    input wire d;
wire e;
    wire f;
    
// Variable declaration.

//Primitive gate instantiations.
    and(e, a, b);
    and(f, c, d);
    and(out, e, f);
    specify
        (a => out) = 9;
        (b => out) = 9;
        (c => out) = 11;
        (d => out) = 11;
        (src *> dest) = 32;
    endspecify

endmodule
