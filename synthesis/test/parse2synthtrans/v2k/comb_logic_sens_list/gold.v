module test(q, d, clk, reset);

output reg q;
input wire d;
input wire clk;
input wire reset;
always @(posedge clk, negedge reset) begin
    if((!reset)) begin

        q <= 0;
    end
    else begin

        q <= d;
    end
end
endmodule
