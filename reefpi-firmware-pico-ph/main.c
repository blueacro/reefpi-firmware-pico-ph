#include <atmel_start.h>


volatile uint8_t last_adc_value[2];

volatile int pos = 1;

static void adc_cb(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
	adc_async_read_channel(descr, channel, (uint8_t*)&last_adc_value, 2);
	adc_async_start_conversion(&ADC_0);
}
void adc_start(void)
{
	adc_async_register_callback(&ADC_0, 0, ADC_ASYNC_CONVERT_CB, adc_cb);
	adc_async_enable_channel(&ADC_0, 0);
	adc_async_start_conversion(&ADC_0);
}


static struct io_descriptor *io;
static void I2C_0_rx_complete(const struct i2c_s_async_descriptor *const descr)
{
	static int mode;
	uint8_t c;
	io_read(io, &c, 1);
	switch(mode) {
		case 0x0:
			switch(c) {
				case 0:
				case 0xa5:
				// This is addressing the conversion register - do node mode switches
					pos = 1;
					return;
				case 0x02:
				// Set gain
					mode = 0x02;
					return;
				default:
					return;
			}
		case 0x02:
		// Set gain
			adc_async_set_channel_gain(&ADC_0, 0, c);	
			mode = 0x0;
			return;
		
		default:
			mode = 0;
			return;
	}
		
}

static void I2C_0_tx_pending(const struct i2c_s_async_descriptor *const descr)
{
	io_write(io, (uint8_t*)&last_adc_value[pos], 1);
	pos--;
	if (pos < 0) 
		pos = 0;
}

static void I2C_0_tx_complete(const struct i2c_s_async_descriptor *const descr)
{
}

void i2c_start(void)
{
	i2c_s_async_get_io_descriptor(&I2C_0, &io);
	i2c_s_async_register_callback(&I2C_0, I2C_S_RX_COMPLETE, I2C_0_rx_complete);
	i2c_s_async_register_callback(&I2C_0, I2C_S_TX_PENDING, I2C_0_tx_pending);
	i2c_s_async_register_callback(&I2C_0, I2C_S_TX_COMPLETE, I2C_0_tx_complete);
	i2c_s_async_enable(&I2C_0);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	adc_start();
	i2c_start();
	
	/* Replace with your application code */
	while (1) {
		sleep(1);
	}
}
