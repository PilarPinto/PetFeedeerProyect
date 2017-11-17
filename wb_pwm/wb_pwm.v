//---------------------------------------------------------------------------
//
// Wishbone PWM
//
// Register Description:
//
//	0x00 [31:0] period: Period [us]
//	0x04 [31:0] dutyM0: Duty cycle motor 0 [us]
//	0x08 [31:0] dutyM1: Duty cycle motor 1 [us]
//	0x0C [31:0] dutyM2: Duty cycle motor 2 [us]
//	0x10 [31:0] dutyM3: Duty cycle motor 3 [us]
//
//---------------------------------------------------------------------------

module wb_pwm  (
	input              clk,
	input              reset,
	// Wishbone interface
	input              wb_stb_i,
	input              wb_cyc_i,
	output             wb_ack_o,
	input              wb_we_i,
	input       [31:0] wb_adr_i,
	input        [3:0] wb_sel_i,
	input       [31:0] wb_dat_i,
	output reg  [31:0] wb_dat_o,
	//
	output 	pwm_motor
);

reg [31:0] duty ;
reg [31:0] period ;

simplePWM PWM0 (.reset(reset), .clk(clk), .time_work(duty), .period(period), .PWM_out(pwm_motor));

wire wb_wr = wb_stb_i & wb_cyc_i &  wb_we_i;
reg  ack;
assign wb_ack_o = wb_stb_i & wb_cyc_i & ack;

always @(posedge clk)
begin
	if (reset) begin
		wb_dat_o[31:8] <= 24'b0;
		ack    <= 0; 
		period [31:0] <= 32'b0;
	end else begin
		wb_dat_o[31:8] <= 24'b0;
		ack    <= 0;
		if (wb_wr & ~ack) begin //Single write cycle
			ack <= 1;
			case (wb_adr_i[2])
			3'b000: begin period <= wb_dat_i; end
			3'b001: begin duty <= wb_dat_i; end
			endcase 
		end
	end
end

endmodule
