
module final_project (
	adc_0_sclk,
	adc_0_cs_n,
	adc_0_dout,
	adc_0_din,
	audio_0_ADCDAT,
	audio_0_ADCLRCK,
	audio_0_BCLK,
	audio_0_DACDAT,
	audio_0_DACLRCK,
	audio_and_video_config_0_SDAT,
	audio_and_video_config_0_SCLK,
	audio_clk_clk,
	clk_clk,
	pio_buttons_export,
	pio_gpio_export,
	pio_leds_export,
	pio_sw_export,
	pll_0_outclk2_clk,
	reset_reset_n,
	sdram_controller_0_wire_addr,
	sdram_controller_0_wire_ba,
	sdram_controller_0_wire_cas_n,
	sdram_controller_0_wire_cke,
	sdram_controller_0_wire_cs_n,
	sdram_controller_0_wire_dq,
	sdram_controller_0_wire_dqm,
	sdram_controller_0_wire_ras_n,
	sdram_controller_0_wire_we_n);	

	output		adc_0_sclk;
	output		adc_0_cs_n;
	input		adc_0_dout;
	output		adc_0_din;
	input		audio_0_ADCDAT;
	input		audio_0_ADCLRCK;
	input		audio_0_BCLK;
	output		audio_0_DACDAT;
	input		audio_0_DACLRCK;
	inout		audio_and_video_config_0_SDAT;
	output		audio_and_video_config_0_SCLK;
	output		audio_clk_clk;
	input		clk_clk;
	input	[3:0]	pio_buttons_export;
	output	[15:0]	pio_gpio_export;
	output	[9:0]	pio_leds_export;
	input	[9:0]	pio_sw_export;
	output		pll_0_outclk2_clk;
	input		reset_reset_n;
	output	[12:0]	sdram_controller_0_wire_addr;
	output	[1:0]	sdram_controller_0_wire_ba;
	output		sdram_controller_0_wire_cas_n;
	output		sdram_controller_0_wire_cke;
	output		sdram_controller_0_wire_cs_n;
	inout	[15:0]	sdram_controller_0_wire_dq;
	output	[1:0]	sdram_controller_0_wire_dqm;
	output		sdram_controller_0_wire_ras_n;
	output		sdram_controller_0_wire_we_n;
endmodule
