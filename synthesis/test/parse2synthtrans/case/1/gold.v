module demultiplexer1_to_4(out0, out1, out2, out3, in, s1, s0);

output reg out0;
output reg out1;
output reg out2;
output reg out3;
input wire in;
input wire s1;
input wire s0;
reg [3:0] encoding;
reg [1:0] state;
always @(encoding[3:0]) begin
    state[1:0] = 0;
    case(encoding[3:0])
    0: begin

        state[1:0] = -1;
    end
    1: begin

        state[1:0] = -1;
    end
    2: begin

        state[1:0] = -1;
    end
    3: begin

        state[1:0] = 1;
    end
    4: begin

        state[1:0] = -2;
    end
    5: begin

        state[1:0] = -2;
    end
    6: begin

        state[1:0] = -2;
    end
    7: begin

        state[1:0] = 1;
    end
    8: begin

        state[1:0] = -1;
    end
    9: begin

        state[1:0] = -1;
    end
    10: begin

        state[1:0] = -1;
    end
    11: begin

        state[1:0] = 1;
    end
    12: begin

        state[1:0] = 0;
    end
    13: begin

        state[1:0] = 0;
    end
    14: begin

        state[1:0] = 0;
    end
    15: begin

        state[1:0] = 1;
    end
    endcase
end
always @(encoding[3:0]) begin
    state[1:0] = 0;
    case(encoding[3:0])
    0: begin

        state[1:0] = 1;
    end
    1: begin

    end
    2: begin

    end
    3: begin

    end
    4: begin

    end
    5: begin

    end
    6: begin

    end
    7: begin

    end
    8: begin

    end
    9: begin

    end
    10: begin

    end
    11: begin

    end
    12: begin

    end
    13: begin

    end
    14: begin

    end
    15: begin

    end
    endcase
end
endmodule
