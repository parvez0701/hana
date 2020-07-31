module ripple_adder(co, sum, a0, a1, ci);
    // Parameter declarations.
    parameter signed [31:0]N = 4;
    output wire co;
    output wire [(N-1):0] sum ;
    input wire [(N-1):0] a0 ;
    input wire [(N-1):0] a1 ;
    input wire ci;
wire [N:0] carry ;
    wire gen_t1;
    wire gen_t2;
    wire gen_t3;
    wire gen_t1_0;
    wire gen_t2_0;
    wire gen_t3_0;
    wire gen_t1_1;
    wire gen_t2_1;
    wire gen_t3_1;
    wire gen_t1_2;
    wire gen_t2_2;
    wire gen_t3_2;
    
// Variable declaration.
    genvar i;
    assign  carry[0] = ci;

//Primitive gate instantiations.
    xor(gen_t1, a0[0], a1[0]);
    xor(sum[0], gen_t1, carry[0]);
    and(gen_t2, a0[0], a1[0]);
    and(gen_t3, gen_t1, carry[0]);
    or(carry[(0+1)], gen_t2, gen_t3);
    xor(gen_t1_0, a0[1], a1[1]);
    xor(sum[1], gen_t1_0, carry[1]);
    and(gen_t2_0, a0[1], a1[1]);
    and(gen_t3_0, gen_t1_0, carry[1]);
    or(carry[(1+1)], gen_t2_0, gen_t3_0);
    xor(gen_t1_1, a0[2], a1[2]);
    xor(sum[2], gen_t1_1, carry[2]);
    and(gen_t2_1, a0[2], a1[2]);
    and(gen_t3_1, gen_t1_1, carry[2]);
    or(carry[(2+1)], gen_t2_1, gen_t3_1);
    xor(gen_t1_2, a0[3], a1[3]);
    xor(sum[3], gen_t1_2, carry[3]);
    and(gen_t2_2, a0[3], a1[3]);
    and(gen_t3_2, gen_t1_2, carry[3]);
    or(carry[(3+1)], gen_t2_2, gen_t3_2);

endmodule
