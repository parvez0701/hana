module mybuf(in, out);

input wire in;
output wire out;
BUF_1_1 synth_BUF_1(.OUT(out), .IN(in));
endmodule
module test(in1, in2, out);

input wire in1;
input wire in2;
output wire out;
wire w3;
wire w4;
wire w7;
wire w8;
mybuf _mybuf0(.in(in1), .out(w3));
mybuf _mybuf1(.in(w3), .out(w4));
mybuf _mybuf2(.in(in2), .out(w7));
mybuf _mybuf3(.in(w7), .out(w4));
mybuf _mybuf4(.in(w4), .out(w8));
mybuf _mybuf5(.in(w8), .out(out));
endmodule
