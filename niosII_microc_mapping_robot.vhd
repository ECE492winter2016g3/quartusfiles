-- Joel Keeling
-- jkeeling@ualberta.ca
-- This file makes extensive use of Altera template structures.
-- This file is the top-level file for our Mapping Robot Capstone project

-- A library clause declares a name as a library.  It 
-- does not create the library; it simply forward declares 
-- it. 
library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
	use ieee.std_logic_1164.all;

	-- SIGNED and UNSIGNED types, and relevant functions
	use ieee.numeric_std.all;

	-- Basic sequential functions and concurrent procedures
	use ieee.VITAL_Primitives.all;	

	use work.DE0_CONSTANTS.all;
	
	entity niosII_microc_mapping_robot is
	
	port
	(
		-- Input ports and 50 MHz Clock
		KEY		: in  std_logic_vector (1 downto 0);
		SW			: in 	std_logic_vector (4 downto 0);
		CLOCK_50	: in  std_logic;
		
		-- leds on board
		LED 		: out DE0_LEDS;		
		
		-- SDRAM on board		
		DRAM_ADDR	:	out	DE0_SDRAM_ADDR_BUS;
		DRAM_BA	   : 	out	std_logic_vector (1 downto 0);
		DRAM_CAS_N	:	out	std_logic;
		DRAM_CKE		:	out	std_logic;
		DRAM_CLK		:	out	std_logic;
		DRAM_CS_N	:	out	std_logic;	
		
		DRAM_DQ		:	inout DE0_SDRAM_DATA_BUS;
		DRAM_DQM		: 	out	std_logic_vector (1 downto 0);

		DRAM_RAS_N	: 	out	std_logic;
		DRAM_WE_N	: 	out 	std_logic;
		
		-- I2C lines for EEPROM
		I2C_SCLK		:  inout std_logic;
		I2C_SDAT		:  inout	std_logic;
		
		-- GPIO lines 
		GPIO_1		: 	inout std_logic_vector(35 downto 0) := (others=>'X');
		
		--Flash 
		EPCS_ASDO	:	out std_logic;
		EPCS_DATA0	:	in std_logic;
		EPCS_DCLK	:	out std_logic;
		EPCS_NCSO	:	out std_logic
	);
end niosII_microc_mapping_robot;


architecture structure of niosII_microc_mapping_robot is

	-- Declarations (optional)
	
	component niosII_system is
		port (
			altpll_0_c0_clk                         	: out   std_logic;                                        -- clk
			reset_reset_n                           	: in    std_logic                     := 'X';             -- reset_n
			i2c_opencores_0_export_scl_pad_io			: inout std_logic;
			i2c_opencores_0_export_sda_pad_io			: inout std_logic;
			clk_clk                                 	: in    std_logic                     := 'X';             -- clk
			sdram_0_wire_addr                       	: out   DE0_SDRAM_ADDR_BUS;                    -- addr
         sdram_0_wire_ba                         	: out   std_logic_vector(1 downto 0);                     -- ba
         sdram_0_wire_cas_n                      	: out   std_logic;                                        -- cas_n
			sdram_0_wire_cke                        	: out   std_logic;                                        -- cke
			sdram_0_wire_cs_n                       	: out   std_logic;                                        -- cs_n
			sdram_0_wire_dq                         	: inout DE0_SDRAM_DATA_BUS := (others => 'X'); -- dq
			sdram_0_wire_dqm                        	: out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_0_wire_ras_n                      	: out   std_logic;                                        -- ras_n
			sdram_0_wire_we_n                       	: out   std_logic;                                        -- we_n
			pio_leds_external_connection_export			: out   DE0_LEDS;
			uart_bluetooth_external_connection_rxd    : in 	  std_logic;
			uart_bluetooth_external_connection_txd    : out   std_logic;
			epcs_flash_controller_0_external_dclk		: out	  std_logic;
			epcs_flash_controller_0_external_sce		: out	  std_logic;			
			epcs_flash_controller_0_external_sdo		: out	  std_logic;
			epcs_flash_controller_0_external_data0		: in	  std_logic;
			i2c_scl_external_connection_export			: out	  std_logic;
			i2c_sda_external_connection_export			: inout std_logic :='X'
		);		
	
    end component niosII_system;

--	These signals are for matching the provided IP core to
-- The specific SDRAM chip in our system	 
	 signal BA	: std_logic_vector (1 downto 0);
	 signal DQM	:	std_logic_vector (1 downto 0);
	
-- I2C
	 signal I2C_SCL_S : std_logic;
	 signal I2C_SDA_S : std_logic;	 

begin

	DRAM_BA <= BA;	
	
	DRAM_DQM <= DQM;	
	
	--I2C_SCLK <= I2C_SCL_S;
	--I2C_SDAT <= I2C_SDA_S;
	--GPIO_1(7) <= I2C_SCL_S;
	--GPIO_1(3) <= I2C_SDA_S;
	
	
	-- Component Instantiation Statement (optional)
	
	  u0 : component niosII_system
			port map(
				altpll_0_c0_clk                         => DRAM_CLK,                        
				reset_reset_n                           => KEY(0),                          
				i2c_opencores_0_export_scl_pad_io		 => GPIO_1(9),	
				i2c_opencores_0_export_sda_pad_io		 => GPIO_1(11),	
				clk_clk                                 => CLOCK_50,                                
				sdram_0_wire_addr                       => DRAM_ADDR,                      
				sdram_0_wire_ba                         => BA,                        
				sdram_0_wire_cas_n                      => DRAM_CAS_N,                      
				sdram_0_wire_cke                        => DRAM_CKE,                       
				sdram_0_wire_cs_n                       => DRAM_CS_N,                      
				sdram_0_wire_dq                         => DRAM_DQ,                         
				sdram_0_wire_dqm                        => DQM,                        
				sdram_0_wire_ras_n                      => DRAM_RAS_N,                     
				sdram_0_wire_we_n                       => DRAM_WE_N,                      
				pio_leds_external_connection_export		 => LED,
				uart_bluetooth_external_connection_rxd  => GPIO_1(0),
				uart_bluetooth_external_connection_txd  => GPIO_1(1),
				epcs_flash_controller_0_external_dclk	 => EPCS_DCLK,	
				epcs_flash_controller_0_external_sce	 => EPCS_NCSO,				
				epcs_flash_controller_0_external_sdo	 => EPCS_ASDO,	
				epcs_flash_controller_0_external_data0	 => EPCS_DATA0,
				i2c_scl_external_connection_export      => GPIO_1(7),
				i2c_sda_external_connection_export      => GPIO_1(3)
			);

end structure;


library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
	use ieee.std_logic_1164.all;

package DE0_CONSTANTS is
	type DE0_SDRAM_ADDR_BUS is array(11 downto 0) of std_logic;
	type DE0_SDRAM_DATA_BUS is array(15 downto 0) of std_logic;
	
	type DE0_LEDS 				is array(7 downto 0)  of std_logic;	
	
	type DE0_GPIO_1			is array(33 downto 0) of std_logic;	
end DE0_CONSTANTS;


