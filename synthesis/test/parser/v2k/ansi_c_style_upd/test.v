
module UDP0();
wire w1, w2;
prim (w1, w2);
endmodule

module UDP1();
wire w1, w2;
prim _prim(w1, w2);
endmodule

primitive prim(output out, input in);

table
	0 : 1;
	1 : 0;
endtable

endprimitive

module UDP2();
wire w1, w2;
prim _prim(w1, w2);
endmodule

module UDP3();
wire w1, w2;
prim (w1, w2);
endmodule
