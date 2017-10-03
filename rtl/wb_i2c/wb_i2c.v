//---------------------------------------------------------------------------
//
// Wishbone I2C
//
// Register Description:
//
//    0x0000 data_rd
//    0x0004 ack_error
//    0x0008 addr
//    0x000C data_wr
//    0x0010 rw
//    0x0014 enable
//    0x0018 busy
//
//---------------------------------------------------------------------------

module wb_i2c#(
	parameter          clk_freq = 50000000
)( input              clk,
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
   // i2c Output
   inout sda,
   inout scl
);

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
reg  ack;
assign wb_ack_o = wb_stb_i & wb_cyc_i & ack;

wire wb_rd = wb_stb_i & wb_cyc_i & ~wb_we_i;
wire wb_wr = wb_stb_i & wb_cyc_i &  wb_we_i;

reg ena;
reg rw;
reg [6:0] addr;
reg [7:0] data_wr;
wire busy;
wire ack_error;
wire [7:0] data_rd;


i2c_master #(
	.freq_hz(   clk_freq )
) i2c0(
.clk(clk),
.reset(reset),
.ena(ena),
.addr(addr),
.rw(rw),
.data_wr(data_wr),
.busy(busy),
.data_rd(data_rd),
.ack_error(ack_error),
.sda(sda),
.scl(scl));

  always @(posedge clk)
  begin
    if (reset) begin
      ena <= 0;
      addr[6:0] <= 7'b0;
      data_wr[7:0] <= 8'b0;
    end else begin

      // Handle WISHBONE access
      ack    <= 0;
     if (wb_rd & ~ack) begin           // read cycle
       ack <= 1'b1;
       case (wb_adr_i[15:0])
       'h0000:  begin 
		wb_dat_o           <= data_rd[7:0];
       end
       'h0004: begin 
	       wb_dat_o            <= ack_error;
       end
       'h0018: begin 
                wb_dat_o           <= busy;
       end
       endcase
     end else if (wb_wr & ~ack) begin // write cycle
	ack <= 1'b1;
	case (wb_adr_i[15:0])
       'h0008:  begin 
		addr[6:0]     <= wb_dat_i[6:0];
       end
       'h000C: begin 
	       data_wr[7:0]  <= wb_dat_i[7:0];
             
       end
       'h0010: begin
	       rw  <= wb_dat_i[0];
       end
       'h0014: begin
               ena <= wb_dat_i[0];
	end
       endcase
     end
    end
  end

endmodule
