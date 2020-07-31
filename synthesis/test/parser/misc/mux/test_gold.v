module mux4_to_1(out, i0, i1, i2, i3, s1, s0);
        output wire out;
    input wire i0;
    input wire i1;
    input wire i2;
    input wire i3;
    input wire s1;
    input wire s0;
wire s1n;
    wire s0n;
    wire y0;
    wire y1;
    wire y2;
    wire y3;
    
// Variable declaration.

//Primitive gate instantiations.
    not(s1n, s1);
    not(s0n, s0);
    and(y0, i0, s1n, s0n);
    and(y1, i1, s1n, s0);
    and(y2, i2, s1, s0n);
    and(y3, i3, s1, s0);
    or(out, y0, y1, y2, y3);

endmodule
module stimulus();
    wire OUTPUT;
    
// Variable declaration.
    reg  IN0;
    reg  IN1;
    reg  IN2;
    reg  IN3;
    reg  S1;
    reg  S0;
    initial         begin
            IN0 = 1;
            IN1 = 0;
            IN2 = 1;
            IN3 = 0;
            S1 = 0;
            S0 = 0;
        end

//Module instantiations.
    mux4_to_1 mux(.out(OUTPUT), .i0(IN0), .i1(IN1), .i2(IN2), .i3(IN3), .s1(S1), .s0(S0));

endmodule
