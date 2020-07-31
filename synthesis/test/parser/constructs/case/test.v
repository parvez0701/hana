module demultiplexer1_to_4 (out0, out1, out2, out3, in, s1, s0);
output out0, out1, out2, out3;
reg out0, out1, out2, out3;
input in;
input s1, s0;
reg [3:0] encoding;
integer state;

always @ (s1 or s0 or in)
    case ({s1, s0} )
    2'b00 : begin out0 = in;
            out1 = 1'bz;
            out2 = 1'bz;
            out3 = 1'bz;
            end
    2'b01: begin
           out0 = 1'bz;
           out1 = in;
           out2 = 1'bz;
           out3 = 1'bz;
           end
    2'b10: begin
           out0 = 1'bz;
           out1 = 1'bz;
           out2 = in;
           out3 = 1'bz;
           end
    2'b11: begin
           out0 = 1'bz;
           out1 = 1'bz;
           out2 = 1'bz;
           out3 = in;
           end
    2'bx0, 2'bx1, 2'bxx, 2'b0x, 2'b1x, 2'bzx:
        begin
           out0 = 1'bx;
           out1 = 1'bx;
           out2 = 1'bx;
           out3 = 1'bx;
       end
   2'bz0, 2'bz1, 2'bzz, 2'b0z, 2'b1z :
       begin
           out0 = 1'bz;
           out1 = 1'bz;
           out2 = 1'bz;
           out3 = 1'bz;
       end
   default: $display("Unspecified control signals");
   endcase
   always @(encoding) begin
        casex (encoding)
        4'b1xxx: state = 3;
        4'bx1xx: state = 2;
        4'bxx1x: state = 1;
        4'bxxx1: state = 0;
        default: state = 0;
        endcase
   end
endmodule
