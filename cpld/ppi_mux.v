`timescale 1ns / 1ps
/* ECE@BU
This code has the following function:
1. Output a clock to ADC
2. Accept data from both ADCs (including Out of Range (OTR) Bit)
3. Output 12 multiplexed data bits (12 data bits) (2:1 mux) plus 4 extra bits
	-The 3 MSB's are set to be "Data Valid" bits. The DSP Sends a signal to 
	the CPLD when we want to start collecting data, the CPLD waits
	 for 4 clock cycles (ADC Delay) and then starts sending frame sync signals
	 while incrementing the 3 MSB's from 0 to 7 (looping) for synchronization
	 purposes. The 4th MSB is  used to synchronize every 256 nodes. It is set on
	 the 1st node every time.
4. Generate PPI Signals:
	-PPI CLK
	-PPI Sync
Note: To start muxing data, you must set the "PPI_Enable" bit. Setting this bit 
low will STOP muxing data (by stopping the framesync). 
Note 2: Due to ADI's PPI Protocol we will always miss a single sample after FS
goes high. This should be fine since FS rarely goes high (Currently set to go
high every 2560 samples).
Code written by Michael Kasparian, Electrical Engineering, 2012
mkasp@bu.edu
*/
`define FSAMPLES 2559
`define FSREGSIZE 14
module ppi_mux(dataout, framesync, LED7, LED6, ppi_clk, adc_clk, data1, data2, PPI_Enable, MINS_CLK5, MINS_CLK2, CLK25M);

	input [12:0] data1, data2; //12 bits + 1 bit for OTR
	input MINS_CLK2, MINS_CLK5, PPI_Enable, CLK25M; //MINS_CLK2 = 2.56MHz, MINS_CLK5 = 5.12MHz
	output [15:0] dataout;
	output framesync, ppi_clk, adc_clk, LED7, LED6;
	
	reg [15:0] dataout;
	reg [12:0] data1hold,data2hold;
	reg [2:0] pipeline_delay_count;
	reg CLK2, LED7, state, framesync, start_PPI;
	reg [`FSREGSIZE:0] fscount;
	reg [5:0] delay_count;

	
//Divide MINS_CLK5 down to 2.56MHz so we only depend on MINS_CLK5
	initial CLK2 = 0;
	initial fscount = 0;
	always @ (negedge ppi_clk) begin
		CLK2 <= ~CLK2;
		if(start_PPI && framesync)
			fscount<=fscount+1;
		else if (start_PPI && ~framesync) //if we're still running and fs goes low
			fscount<=1;
		else //if we reset everything..
			fscount<=0;
	end
	
//ADC Pipeline Delay, and enable of ADC, LEDs tell state
	initial pipeline_delay_count = 3'b000; 
	initial LED7 = 0;	
	initial state = 0;
	initial start_PPI=0;
	always @ (negedge adc_clk) begin
		if((PPI_Enable || (pipeline_delay_count > 3'b000)) && pipeline_delay_count < 3'b100) begin
			pipeline_delay_count <= pipeline_delay_count + 1; //count up to 4 and stop
		end
		else if(((pipeline_delay_count == 3'b100) && ~PPI_Enable)) begin
		//if it's converting, pipeline_delay_count will be > 3'b100
		//if PPI_Enable is asserted again, then it will stop PPI FS signal (stop sending data)
			pipeline_delay_count <= 3'b000; //reset
		end
		if(pipeline_delay_count == 3'b100) start_PPI <= 1;
		else start_PPI <=0;
		LED7 <= start_PPI; //LED ON = Sending Data, off = not sending
		
	end
	assign LED6 = PPI_Enable;
	
//Invert MINS 2.56MHz Clock to ADC
	assign #7.5 adc_clk = ~CLK2; 	
	
//Multiplexer
	//store data in register fpr each clock cycle
	always @ (posedge CLK2) begin
		#7.5 data1hold <= data1;
		#7.5 data2hold <= data2;
	end
	
	//Multiplex
	always @ (posedge CLK25M) begin
		dataout[11:0] <= (CLK2) ? data1hold[11:0] : data2hold[11:0]; //omit overflow bit (bit 12) 
		dataout[15:13] <= fscount[2:0]; //counts from zero to 7
		dataout[12] <= (fscount[7:0]==8'b0000_0000); //high whenever we are at node 1
	end
	
//Blackfin PPI Control Signals
	//Framesync
	initial delay_count = 0;
	always @ (posedge CLK25M) begin
		if (start_PPI) begin //dont start asserting frame sync until data is ready
			if(fscount == `FSAMPLES && delay_count < 3) begin
				delay_count<=delay_count+1;
			end
			else if (delay_count >= 3 && delay_count < 10) begin
				framesync <= 0;
				delay_count<=delay_count+1;
			end
			else begin
				framesync <=1;
				delay_count <= 0;
			end
		end
		else framesync<=0;
	end

	//PPI_CLK
	assign #7.5 ppi_clk = ~MINS_CLK5;
	
endmodule
