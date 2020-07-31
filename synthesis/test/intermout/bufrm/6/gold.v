module mybuf(in, out);

input wire in;
output wire out;
BUF_1_1 synth_BUF_1(.OUT(out), .IN(in));
endmodule
module test(in, out);

input wire in;
output wire out;
mybuf _mybuf(.in(in), .out(out));
endmodule
