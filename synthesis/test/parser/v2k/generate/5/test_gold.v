module test(q, d, clk, rst);
        output reg q;
    input wire d;
    input wire clk;
    input wire rst;
wire w;
    
// Variable declaration.
    reg  r1;
    reg  r2;
    reg  gen_r;
    genvar i;
    assign  w = r2;
    assign  w = r1;

    always @ (*
)    begin
if (rst)
        q <= 1'b1;
else 
        q <= d;
    end

endmodule
