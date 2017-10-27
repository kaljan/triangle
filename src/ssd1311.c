#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ssd1311.h"

extern void i2c_init(void);
extern int i2c_send_data(uint8_t addr, const uint8_t *buf, int size);

extern volatile int i2c_rdy;

#define SSD1311_INIT_SEQ_SIZE	24

static const uint8_t ssd1311_init_seq[SSD1311_INIT_SEQ_SIZE] = {
	// Set Sleep Mode On
	0x80, SSD1311_CTRL,

	// **** Set "RE"= 1
	0x80, SSD1311_FNSET | SSD1311_FNSET_N | SSD1311_FNSET_RE,

	// Set "SD"=1
	0x80, SSD1311_EX_CHZ | SSD1311_EX_CHZ_SD,
	
	// Set Display Clock Divide Ratio/Oscillator Frequency
	0x80, SSD1311_EX_CLKCTRL,
	0x80, 0x70, 
	
	// Set Contrast
	0x80, SSD1311_EX_SETCC,
	0x80, 0xFF,

	// Exiting Set OLED Characterization
	0x80, SSD1311_EX_CHZ,
	
	// Set Entry Mode
	0x80, SSD1311_EXFNSET,
	
	// Set "IS"=0 , "RE" = 0
	0x80, SSD1311_FNSET | SSD1311_FNSET_N,
	0x80, SSD1311_CLEAR,
	
	// Set DDRAM Address to 0x80 (line 1 start)
	0x00, SSD1311_DDRAM_ADDR,
};

struct str_db {
	uint8_t *str;
	struct str_db *next;
};

static struct str_db str_db1;
static struct str_db str_db2;
static struct str_db *db;

static uint8_t str1[68];
static uint8_t str2[68];

char ssd1311_str[33];

int ssd1311_init(void)
{
	str_db1.str = str1;
	str_db2.str = str2;
	str_db1.next = &str_db2;
	str_db2.next = &str_db1;
	db = &str_db2;
	
	memset(ssd1311_str, ' ', 32);
	
	i2c_init();
	
	i2c_send_data(0x78, ssd1311_init_seq, SSD1311_INIT_SEQ_SIZE);
	while (i2c_rdy == 0);

	return 0;
}

static uint8_t ssd1311_on_seq[2] = {0x00, 0x0C};

void ssd1311_on(void)
{
	i2c_send_data(0x78, ssd1311_on_seq, 2);
	while (i2c_rdy == 0);
}

static void prepare_str(uint8_t *packet, const char *str)
{
	int i;
	*packet = 0x80;
	packet++;
	*packet = 0x80;
	
	for (i = 0; i < 16; i++) {
		packet++;
		*packet = 0xC0;
		packet++;
		*packet = *str;
		str++;
	}
	
	packet++;
	*packet = 0x80;
	packet++;
	*packet = 0xC0;
	
	for (i = 0; i < 16; i++) {
		packet++;
		*packet = ((i < 15) ? 0xC0 : 0x40);
		packet++;
		*packet = *str;
		str++;
	}
}

void ssd1311_swap(void)
{
	prepare_str(db->str, ssd1311_str);
	while (i2c_rdy == 0);
	i2c_send_data(0x78, db->str, 68);
	memset(ssd1311_str, ' ', 32);
	db = db->next;
}

void ssd1311_set_string(char *str, int start)
{
	int i;
	char *sptr;
	
	i = start;
	sptr = &ssd1311_str[start];
	while (*str != 0) {
		*sptr = *str;
		sptr++;
		str++;
		i++;
		if (i >= 32) {
			break;
		}
	}
}



/*
uint8_t ssd1311_init_seq[64] = {
	//0x78,
	0x80, 0x2A, 0x80, 0x71,  0x80, 0x5C, 0x80, 0x28,
	0x80, 0x08, 0x80, 0x2A,  0x80, 0x79, 0x80, 0xD5,
	0x80, 0x70, 0x80, 0x78,  0x80, 0x08, 0x80, 0x06,
	0x80, 0x2A, 0x80, 0x79,  0x80, 0x72, 0x80, 0x00,
	
	0x80, 0xDA, 0x80, 0x10,  0x80, 0x81, 0x80, 0xFF,
	0x80, 0xDB, 0x80, 0x30,  0x80, 0xDC, 0x80, 0x03,
	0x80, 0x78, 0x80, 0x28,  0x80, 0x2A, 0x80, 0x06,
	0x80, 0x08, 0x80, 0x28,  0x80, 0x01, 0x00, 0x80
};
*/
