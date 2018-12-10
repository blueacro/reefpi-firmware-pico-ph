/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

#include <hpl_adc_base.h>
#include <hpl_rtc_base.h>

/* The channel amount for ADC */
#define ADC_0_CH_AMOUNT 1

/* The buffer size for ADC */
#define ADC_0_BUFFER_SIZE 16

/* The maximal channel number of enabled channels */
#define ADC_0_CH_MAX 0

struct adc_async_descriptor         ADC_0;
struct adc_async_channel_descriptor ADC_0_ch[ADC_0_CH_AMOUNT];
struct timer_descriptor             TIMER_0;

static uint8_t ADC_0_buffer[ADC_0_BUFFER_SIZE];
static uint8_t ADC_0_map[ADC_0_CH_MAX + 1];

struct i2c_s_async_descriptor I2C_0;
uint8_t                       SERCOM0_i2c_s_buffer[SERCOM0_I2CS_BUFFER_SIZE];

/**
 * \brief ADC initialization function
 *
 * Enables ADC peripheral, clocks and initializes ADC driver
 */
void ADC_0_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, ADC);
	_gclk_enable_channel(ADC_GCLK_ID, CONF_GCLK_ADC_SRC);
	adc_async_init(&ADC_0, ADC, ADC_0_map, ADC_0_CH_MAX, ADC_0_CH_AMOUNT, &ADC_0_ch[0], (void *)NULL);
	adc_async_register_channel_buffer(&ADC_0, 0, ADC_0_buffer, ADC_0_BUFFER_SIZE);

	// Disable digital pin circuitry
	gpio_set_pin_direction(PHPIN, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PHPIN, PINMUX_PA05B_ADC_AIN3);

	// Disable digital pin circuitry
	gpio_set_pin_direction(VREF2PIN, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(VREF2PIN, PINMUX_PA06B_ADC_AIN4);
}

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA14,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA14, PINMUX_PA14C_SERCOM0_PAD0);

	gpio_set_pin_pull_mode(PA15,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA15, PINMUX_PA15C_SERCOM0_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	_gclk_enable_channel(SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_s_async_init(&I2C_0, SERCOM0, SERCOM0_i2c_s_buffer, SERCOM0_I2CS_BUFFER_SIZE);
	I2C_0_PORT_init();
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
	timer_init(&TIMER_0, RTC, _rtc_get_timer());
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA03

	// Disable digital pin circuitry
	gpio_set_pin_direction(VREF, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(VREF, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA08

	// Set pin direction to input
	gpio_set_pin_direction(BUTTON, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(BUTTON,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(BUTTON, GPIO_PIN_FUNCTION_OFF);

	ADC_0_init();

	I2C_0_init();

	TIMER_0_init();
}
