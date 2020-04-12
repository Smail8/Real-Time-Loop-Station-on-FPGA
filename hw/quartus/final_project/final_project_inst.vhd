	component final_project is
		port (
			adc_0_sclk                    : out   std_logic;                                        -- sclk
			adc_0_cs_n                    : out   std_logic;                                        -- cs_n
			adc_0_dout                    : in    std_logic                     := 'X';             -- dout
			adc_0_din                     : out   std_logic;                                        -- din
			audio_0_ADCDAT                : in    std_logic                     := 'X';             -- ADCDAT
			audio_0_ADCLRCK               : in    std_logic                     := 'X';             -- ADCLRCK
			audio_0_BCLK                  : in    std_logic                     := 'X';             -- BCLK
			audio_0_DACDAT                : out   std_logic;                                        -- DACDAT
			audio_0_DACLRCK               : in    std_logic                     := 'X';             -- DACLRCK
			audio_and_video_config_0_SDAT : inout std_logic                     := 'X';             -- SDAT
			audio_and_video_config_0_SCLK : out   std_logic;                                        -- SCLK
			audio_clk_clk                 : out   std_logic;                                        -- clk
			clk_clk                       : in    std_logic                     := 'X';             -- clk
			pio_buttons_export            : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			pio_gpio_export               : out   std_logic_vector(15 downto 0);                    -- export
			pio_leds_export               : out   std_logic_vector(9 downto 0);                     -- export
			pio_sw_export                 : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			pll_0_outclk2_clk             : out   std_logic;                                        -- clk
			reset_reset_n                 : in    std_logic                     := 'X';             -- reset_n
			sdram_controller_0_wire_addr  : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_controller_0_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_controller_0_wire_cas_n : out   std_logic;                                        -- cas_n
			sdram_controller_0_wire_cke   : out   std_logic;                                        -- cke
			sdram_controller_0_wire_cs_n  : out   std_logic;                                        -- cs_n
			sdram_controller_0_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_controller_0_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_controller_0_wire_ras_n : out   std_logic;                                        -- ras_n
			sdram_controller_0_wire_we_n  : out   std_logic                                         -- we_n
		);
	end component final_project;

	u0 : component final_project
		port map (
			adc_0_sclk                    => CONNECTED_TO_adc_0_sclk,                    --                    adc_0.sclk
			adc_0_cs_n                    => CONNECTED_TO_adc_0_cs_n,                    --                         .cs_n
			adc_0_dout                    => CONNECTED_TO_adc_0_dout,                    --                         .dout
			adc_0_din                     => CONNECTED_TO_adc_0_din,                     --                         .din
			audio_0_ADCDAT                => CONNECTED_TO_audio_0_ADCDAT,                --                  audio_0.ADCDAT
			audio_0_ADCLRCK               => CONNECTED_TO_audio_0_ADCLRCK,               --                         .ADCLRCK
			audio_0_BCLK                  => CONNECTED_TO_audio_0_BCLK,                  --                         .BCLK
			audio_0_DACDAT                => CONNECTED_TO_audio_0_DACDAT,                --                         .DACDAT
			audio_0_DACLRCK               => CONNECTED_TO_audio_0_DACLRCK,               --                         .DACLRCK
			audio_and_video_config_0_SDAT => CONNECTED_TO_audio_and_video_config_0_SDAT, -- audio_and_video_config_0.SDAT
			audio_and_video_config_0_SCLK => CONNECTED_TO_audio_and_video_config_0_SCLK, --                         .SCLK
			audio_clk_clk                 => CONNECTED_TO_audio_clk_clk,                 --                audio_clk.clk
			clk_clk                       => CONNECTED_TO_clk_clk,                       --                      clk.clk
			pio_buttons_export            => CONNECTED_TO_pio_buttons_export,            --              pio_buttons.export
			pio_gpio_export               => CONNECTED_TO_pio_gpio_export,               --                 pio_gpio.export
			pio_leds_export               => CONNECTED_TO_pio_leds_export,               --                 pio_leds.export
			pio_sw_export                 => CONNECTED_TO_pio_sw_export,                 --                   pio_sw.export
			pll_0_outclk2_clk             => CONNECTED_TO_pll_0_outclk2_clk,             --            pll_0_outclk2.clk
			reset_reset_n                 => CONNECTED_TO_reset_reset_n,                 --                    reset.reset_n
			sdram_controller_0_wire_addr  => CONNECTED_TO_sdram_controller_0_wire_addr,  --  sdram_controller_0_wire.addr
			sdram_controller_0_wire_ba    => CONNECTED_TO_sdram_controller_0_wire_ba,    --                         .ba
			sdram_controller_0_wire_cas_n => CONNECTED_TO_sdram_controller_0_wire_cas_n, --                         .cas_n
			sdram_controller_0_wire_cke   => CONNECTED_TO_sdram_controller_0_wire_cke,   --                         .cke
			sdram_controller_0_wire_cs_n  => CONNECTED_TO_sdram_controller_0_wire_cs_n,  --                         .cs_n
			sdram_controller_0_wire_dq    => CONNECTED_TO_sdram_controller_0_wire_dq,    --                         .dq
			sdram_controller_0_wire_dqm   => CONNECTED_TO_sdram_controller_0_wire_dqm,   --                         .dqm
			sdram_controller_0_wire_ras_n => CONNECTED_TO_sdram_controller_0_wire_ras_n, --                         .ras_n
			sdram_controller_0_wire_we_n  => CONNECTED_TO_sdram_controller_0_wire_we_n   --                         .we_n
		);

