#include <atmel_start.h>


volatile int16_t last_adc_value;
uint16_t foo = 0xDEAD;
int pos = 0;

static void adc_cb(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
	int16_t read;
	int32_t bytes = adc_async_read_channel(descr, channel, (uint8_t*)&read, sizeof(read));
	
	last_adc_value = read;
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
	uint8_t c;
	io_read(io, &c, 1);
	pos = 0;
}

static void I2C_0_tx_pending(const struct i2c_s_async_descriptor *const descr)
{
	uint8_t value = last_adc_value >> (pos*8);
	io_write(io, &value, 1);
	pos++;
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
		delay_ms(100);
	}
}
