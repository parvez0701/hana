module bitwise_xor(out, i0, i1);
    // Parameter declarations.
    parameter signed [31:0]N = 32;
    output reg [(N-1):0] out ;
    input wire [(N-1):0] i0 ;
    input wire [(N-1):0] i1 ;

// Variable declaration.
    integer  j;
    integer  gen_I;
    integer  gen_I_0;
    integer  gen_I_1;
    integer  gen_I_2;
    integer  gen_I_3;
    integer  gen_I_4;
    integer  gen_I_5;
    integer  gen_I_6;
    integer  gen_I_7;
    integer  gen_I_8;
    integer  gen_I_9;
    integer  gen_I_10;
    integer  gen_I_11;
    integer  gen_I_12;
    integer  gen_I_13;
    integer  gen_I_14;
    integer  gen_I_15;
    integer  gen_I_16;
    integer  gen_I_17;
    integer  gen_I_18;
    integer  gen_I_19;
    integer  gen_I_20;
    integer  gen_I_21;
    integer  gen_I_22;
    integer  gen_I_23;
    integer  gen_I_24;
    integer  gen_I_25;
    integer  gen_I_26;
    integer  gen_I_27;
    integer  gen_I_28;
    integer  gen_I_29;
    integer  gen_I_30;
    genvar k;

//Primitive gate instantiations.
    xor(out[0], i0[0], i1[0]);
    xor(out[1], i0[1], i1[1]);
    xor(out[2], i0[2], i1[2]);
    xor(out[3], i0[3], i1[3]);
    xor(out[4], i0[4], i1[4]);
    xor(out[5], i0[5], i1[5]);
    xor(out[6], i0[6], i1[6]);
    xor(out[7], i0[7], i1[7]);
    xor(out[8], i0[8], i1[8]);
    xor(out[9], i0[9], i1[9]);
    xor(out[10], i0[10], i1[10]);
    xor(out[11], i0[11], i1[11]);
    xor(out[12], i0[12], i1[12]);
    xor(out[13], i0[13], i1[13]);
    xor(out[14], i0[14], i1[14]);
    xor(out[15], i0[15], i1[15]);
    xor(out[16], i0[16], i1[16]);
    xor(out[17], i0[17], i1[17]);
    xor(out[18], i0[18], i1[18]);
    xor(out[19], i0[19], i1[19]);
    xor(out[20], i0[20], i1[20]);
    xor(out[21], i0[21], i1[21]);
    xor(out[22], i0[22], i1[22]);
    xor(out[23], i0[23], i1[23]);
    xor(out[24], i0[24], i1[24]);
    xor(out[25], i0[25], i1[25]);
    xor(out[26], i0[26], i1[26]);
    xor(out[27], i0[27], i1[27]);
    xor(out[28], i0[28], i1[28]);
    xor(out[29], i0[29], i1[29]);
    xor(out[30], i0[30], i1[30]);
    xor(out[31], i0[31], i1[31]);
    always @ (i0[0] or i1[0])    out[0] = (i0[0]^i1[0]);
    always @ (i0[1] or i1[1])    out[1] = (i0[1]^i1[1]);
    always @ (i0[2] or i1[2])    out[2] = (i0[2]^i1[2]);
    always @ (i0[3] or i1[3])    out[3] = (i0[3]^i1[3]);
    always @ (i0[4] or i1[4])    out[4] = (i0[4]^i1[4]);
    always @ (i0[5] or i1[5])    out[5] = (i0[5]^i1[5]);
    always @ (i0[6] or i1[6])    out[6] = (i0[6]^i1[6]);
    always @ (i0[7] or i1[7])    out[7] = (i0[7]^i1[7]);
    always @ (i0[8] or i1[8])    out[8] = (i0[8]^i1[8]);
    always @ (i0[9] or i1[9])    out[9] = (i0[9]^i1[9]);
    always @ (i0[10] or i1[10])    out[10] = (i0[10]^i1[10]);
    always @ (i0[11] or i1[11])    out[11] = (i0[11]^i1[11]);
    always @ (i0[12] or i1[12])    out[12] = (i0[12]^i1[12]);
    always @ (i0[13] or i1[13])    out[13] = (i0[13]^i1[13]);
    always @ (i0[14] or i1[14])    out[14] = (i0[14]^i1[14]);
    always @ (i0[15] or i1[15])    out[15] = (i0[15]^i1[15]);
    always @ (i0[16] or i1[16])    out[16] = (i0[16]^i1[16]);
    always @ (i0[17] or i1[17])    out[17] = (i0[17]^i1[17]);
    always @ (i0[18] or i1[18])    out[18] = (i0[18]^i1[18]);
    always @ (i0[19] or i1[19])    out[19] = (i0[19]^i1[19]);
    always @ (i0[20] or i1[20])    out[20] = (i0[20]^i1[20]);
    always @ (i0[21] or i1[21])    out[21] = (i0[21]^i1[21]);
    always @ (i0[22] or i1[22])    out[22] = (i0[22]^i1[22]);
    always @ (i0[23] or i1[23])    out[23] = (i0[23]^i1[23]);
    always @ (i0[24] or i1[24])    out[24] = (i0[24]^i1[24]);
    always @ (i0[25] or i1[25])    out[25] = (i0[25]^i1[25]);
    always @ (i0[26] or i1[26])    out[26] = (i0[26]^i1[26]);
    always @ (i0[27] or i1[27])    out[27] = (i0[27]^i1[27]);
    always @ (i0[28] or i1[28])    out[28] = (i0[28]^i1[28]);
    always @ (i0[29] or i1[29])    out[29] = (i0[29]^i1[29]);
    always @ (i0[30] or i1[30])    out[30] = (i0[30]^i1[30]);
    always @ (i0[31] or i1[31])    out[31] = (i0[31]^i1[31]);

endmodule
