module test(out, i0, i1, i2, i3, s1, s0);
        output wire out;
    input wire i0;
    input wire i1;
    input wire i2;
    input wire i3;
    input wire s1;
    input wire s0;

// Variable declaration.
    assign  out = ((((( (~s1)&s0)&i0)|(( (~s1)&s0)&i1))|((s1& (~s0))&i2))|((s1&s0)&i3));


endmodule
module ternaryop(out, i0, i1, i2, i3, s1, s0);
        output wire out;
    input wire i0;
    input wire i1;
    input wire i2;
    input wire i3;
    input wire s1;
    input wire s0;

// Variable declaration.
    assign  out =  (s1) ?  ( (s0) ?  (i3) :  (i2)) :  ( (s0) ?  (i1) :  (i0));


endmodule
module fulladd4(sum, c_out, a, b, c_in);
        output wire [3:0] sum ;
    output wire c_out;
    input wire [3:0] a ;
    input wire [3:0] b ;
    input wire c_in;

// Variable declaration.
    assign  {c_out, sum} = ((a+b)+c_in);


endmodule
