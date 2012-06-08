`timescale 1ns / 1ps
module test_ppi_mux3;

	// Inputs
	reg [12:0] data1;
	reg [12:0] data2;
	reg start_adc;
	reg MINS_CLK5;
	reg MINS_CLK2;
	reg CLK25M;

	// Outputs
	wire [15:0] dataout;
	wire framesync;
	wire ppi_clk;
	wire adc_clk;

	// Instantiate the Unit Under Test (UUT)
	ppi_mux uut (
		.dataout(dataout), 
		.framesync(framesync), 
		.ppi_clk(ppi_clk), 
		.adc_clk(adc_clk), 
		.data1(data1), 
		.data2(data2), 
		.start_adc(start_adc), 
		.MINS_CLK5(MINS_CLK5), 
		.MINS_CLK2(MINS_CLK2), 
		.CLK25M(CLK25M)
	);

	initial begin
		// Initialize Inputs
		data1 = 13'b1_1111_1111_1111;
		data2 = 13'b0_0000_0000_0000;
		MINS_CLK5 = 0;
		MINS_CLK2 = 0;
		CLK25M = 0;
		start_adc = 0;
		#95.3125 data1 = 13'b1_0110_0110_0110;
					 data2 = 13'b0_1001_1001_1001;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;
		start_adc = 1;
		#95.3125 data1 = 13'b0_0000_0000_0000;
					 data2 = 13'b1_1111_1111_1111;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;		
		#95.3125 data1 = 13'b1_1110_1110_1110;
					 data2 = 13'b0_0001_0001_0001;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;
		start_adc = 0;
		#95.3125 data1 = 13'b0_0000_0000_0000;
					 data2 = 13'b1_1111_1111_1111;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;		
		#95.3125 data1 = 13'b1_1110_1110_1110;
					 data2 = 13'b0_0001_0001_0001;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;
		#95.3125 data1 = 13'b0_0000_0000_0000;
					 data2 = 13'b1_1111_1111_1111;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;		
		#95.3125 data1 = 13'b1_1110_1110_1110;
					 data2 = 13'b0_0001_0001_0001;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;
		#95.3125 data1 = 13'b0_0000_0000_0000;
					 data2 = 13'b1_1111_1111_1111;
		#295.3125 data1 = 13'hzzz;
					 data2 = 13'hzzz;		
		#95.3125 data1 = 13'b1_1110_1110_1110;
					 data2 = 13'b0_0001_0001_0001;
		// Wait 100 ns for global reset to finish
		// Add stimulus here

	end
      
		always begin
			#97.65625 MINS_CLK5 <= ~MINS_CLK5;
		end
		always begin
			#195.3125 MINS_CLK2 <= ~MINS_CLK2;
		end
		always begin
			#20 CLK25M <= ~CLK25M; //20 ns half period for 40ns period = 25mhz clock
		end
      
endmodule

