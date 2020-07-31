module test(in, clk, out);
input in, clk;
output reg [5:0] out;
integer i;
parameter p = 5;

always @(posedge clk) begin
    for(i= 0; i < 3; i = i + 1) begin
        out[i] = in;
        out[i+1] = out[i+2];
    end
    repeat (5)
    out[0] = in;
end

endmodule

