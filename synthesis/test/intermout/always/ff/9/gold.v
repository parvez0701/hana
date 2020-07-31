module MyCounter(clock, preset, updown, presetdata, counter);

input wire clock;
input wire preset;
input wire updown;
input wire [1:0]presetdata;
output reg [1:0]counter;
wire synth_net_0;
wire synth_net_2;
wire synth_net_3;
wire synth_net_4;
wire synth_net_5;
wire synth_net_6;
wire synth_net_7;
wire synth_net_8;
wire synth_net_9;
wire synth_net_10;
wire synth_net_12;
wire synth_net_14;
wire synth_net_15;
wire synth_net_16;
wire synth_net_17;
wire synth_net_18;
wire synth_net_19;
wire synth_net_20;
wire synth_net_21;
wire synth_net_22;
wire synth_net_24;
wire synth_net_26;
wire synth_net_27;
wire synth_net_28;
wire synth_net_29;
wire synth_net_30;
wire synth_net_31;
wire synth_net_32;
wire synth_net_33;
wire synth_net_34;
wire synth_net_35;
wire synth_net_36;
wire synth_net_37;
wire synth_net_38;
wire synth_net_39;
wire synth_net_40;
VCC_0_1 synth_VCC(.OUT(synth_net_0));
VCC_0_1 synth_VCC_0(.OUT(synth_net_2));
AND_2_1 synth_AND(.OUT(synth_net_3), .IN({synth_net, synth_net_0}));
AND_2_1 synth_AND_0(.OUT(synth_net_4), .IN({synth_net_1, synth_net_2}));
GND_0_1 synth_GND(.OUT(synth_net_5));
VCC_0_1 synth_VCC_1(.OUT(synth_net_6));
GND_0_1 synth_GND_0(.OUT(synth_net_7));
ADD_2_2 synth_ADD(.COUT(synth_net_8), .C({synth_net_9, synth_net_10}), .A(
    {counter[1], counter[0]}), .B({synth_net_5, synth_net_6}), .CIN(synth_net_7)
);
VCC_0_1 synth_VCC_2(.OUT(synth_net_12));
VCC_0_1 synth_VCC_3(.OUT(synth_net_14));
AND_2_1 synth_AND_1(.OUT(synth_net_15), .IN({synth_net_11, synth_net_12}));
AND_2_1 synth_AND_2(.OUT(synth_net_16), .IN({synth_net_13, synth_net_14}));
GND_0_1 synth_GND_1(.OUT(synth_net_17));
VCC_0_1 synth_VCC_4(.OUT(synth_net_18));
GND_0_1 synth_GND_2(.OUT(synth_net_19));
SUB_2_2 synth_SUB(.BOUT(synth_net_20), .C({synth_net_21, synth_net_22}), .A(
    {counter[1], counter[0]}), .B({synth_net_17, synth_net_18}), .BIN(
    synth_net_19));
VCC_0_1 synth_VCC_5(.OUT(synth_net_24));
VCC_0_1 synth_VCC_6(.OUT(synth_net_26));
AND_2_1 synth_AND_3(.OUT(synth_net_27), .IN({synth_net_23, synth_net_24}));
AND_2_1 synth_AND_4(.OUT(synth_net_28), .IN({synth_net_25, synth_net_26}));
MUX_2_1 synth_MUX(.OUT(synth_net_29), .IN({synth_net_15, synth_net_27}), .SEL(
    updown));
MUX_2_1 synth_MUX_0(.OUT(synth_net_30), .IN({synth_net_12, synth_net_24}), .SEL(
    updown));
MUX_2_1 synth_MUX_1(.OUT(synth_net_31), .IN({synth_net_16, synth_net_28}), .SEL(
    updown));
MUX_2_1 synth_MUX_2(.OUT(synth_net_32), .IN({synth_net_14, synth_net_26}), .SEL(
    updown));
AND_2_1 synth_AND_5(.OUT(synth_net_33), .IN({synth_net_29, synth_net_30}));
AND_2_1 synth_AND_6(.OUT(synth_net_34), .IN({synth_net_31, synth_net_32}));
MUX_2_1 synth_MUX_3(.OUT(synth_net_35), .IN({synth_net_3, synth_net_33}), .SEL(
    preset));
MUX_2_1 synth_MUX_4(.OUT(synth_net_36), .IN({synth_net_0, synth_net_30}), .SEL(
    preset));
MUX_2_1 synth_MUX_5(.OUT(synth_net_37), .IN({synth_net_4, synth_net_34}), .SEL(
    preset));
MUX_2_1 synth_MUX_6(.OUT(synth_net_38), .IN({synth_net_2, synth_net_32}), .SEL(
    preset));
AND_2_1 synth_AND_7(.OUT(synth_net_39), .IN({synth_net_35, synth_net_36}));
FF_2_1 synth_FF(.Q(counter[1]), .D(synth_net_39), .CLK(clock));
AND_2_1 synth_AND_8(.OUT(synth_net_40), .IN({synth_net_37, synth_net_38}));
FF_2_1 synth_FF_0(.Q(counter[0]), .D(synth_net_40), .CLK(clock));
endmodule
