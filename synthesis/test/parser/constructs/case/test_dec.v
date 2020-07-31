module demultiplexer1_to_4(out0, out1, out2, out3, in, s1, s0);
        output reg out0;
    output reg out1;
    output reg out2;
    output reg out3;
    input wire in;
    input wire s1;
    input wire s0;

// Variable declaration.
    reg [3:0] encoding ;
    integer  state;

    always @ (s1 or s0 or in)    always @ (encoding)    begin
    end

endmodule
