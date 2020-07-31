module UDP0();
    wire w1;
    wire w2;
    
// Variable declaration.

//UDP instantiations.
    prim (w1, w2);

endmodule
module UDP1();
    wire w1;
    wire w2;
    
// Variable declaration.

//UDP instantiations.
    prim _prim(w1, w2);

endmodule
module UDP2();
    wire w1;
    wire w2;
    
// Variable declaration.

//UDP instantiations.
    prim _prim(w1, w2);

endmodule
module UDP3();
    wire w1;
    wire w2;
    
// Variable declaration.

//UDP instantiations.
    prim (w1, w2);

endmodule
primitive prim(out, in);
output out;
input in;
initial out = 1;
table
    0  :  1;
    1  :  0;
endtable
endprimitive
