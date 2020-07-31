module M(out, a, b, c, d);
        output wire out;
    input wire a;
    input wire b;
    input wire c;
    input wire d;
wire e;
    wire f;
    
// Variable declaration.

    specify
        if(a) (a => out) = 9;
        if( (~a)) (a => out) = 10;
        if((b&c)) (b => out) = 9;
        if( (~(b&c))) (b => out) = 19;
        if(({c, d}==2'b01)) (c, d *> out) = 11;
        if(({c, d}!=2'b01)) (c, d *> out) = 13;
    endspecify

endmodule
module DFF_1(d, clk, q);
        input wire d;
    input wire clk;
    output reg q;

// Variable declaration.

    always @ (posedge clk)    begin
        q <= d;
    end
    specify
        specparam t_delay = 11;
        specparam t_rise = 9;
        specparam t_fall = 13;
        specparam t_01 = 9;
        specparam t_10 = 13;
        specparam t_0z = 11;
        specparam t_z1 = 9;
        specparam t_1z = 11;
        specparam t_z0 = 13;
        (clk => q) = t_delay;
        (clk => q) =  (t_rise, t_fall);
        (clk => q) =  (t_01, t_10, t_0z, t_z1, t_1z, t_z0);
    endspecify

endmodule
module DFF_2(d, clk, q);
        input wire d;
    input wire clk;
    output reg q;

// Variable declaration.

    always @ (posedge clk)    begin
        q <= d;
    end
    specify
        specparam t_rise = 8:9:10;
        specparam t_fall = 12:13:14;
        specparam t_turnoff = 10:11:12;
        (clk => q) =  (t_rise, t_fall, t_turnoff);
    endspecify

endmodule
