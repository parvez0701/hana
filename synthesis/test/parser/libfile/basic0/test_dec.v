module test(in, out, io, vin, vout, vio);
    // Parameter declarations.
    parameter signed [31:0]v2kparam = 5;
    parameter signed [31:0]myparam = 10;
    input wire in;
    output wire out;
    inout wire io;
    input wire [3:0] vin ;
    output wire [v2kparam:0] vout ;
    inout wire [0:3] vio ;

// Variable declaration.

//Module instantiations.
    LIBMOD libmod();

endmodule
module LIBMOD();
    
// Variable declaration.


endmodule
