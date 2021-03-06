module M (out, a, b, c, d, src, dest);
output out, dest;
input a, b, c, d;
input [3:0] src;

wire e, f;

specify
	(a => out) = 9;
	(b => out) = 9;
	(c => out) = 11;
	(d => out) = 11;
	(src *> dest) = 32;
endspecify

and a1 (e, a, b);
and a2 (f, c, d);
and a3 (out, e, f);
endmodule
