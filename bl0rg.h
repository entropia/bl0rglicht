#ifndef BL0RG_H
#define BL0RG_H

#include <inttypes.h>

#define BAUDRATE 19200
// #define XTAL 16000000L

#define VERSION_STRING "bl0rgware firmware 0.1\n\r"

/* eeprom functions */
uint8_t eeprom_ok(void);
void play_eeprom(void);
void record_eeprom(void);
void clear_eeprom(void);
void random_eeprom(void);

/* led handling functions */
void SetLed(uint8_t,uint16_t);
inline void HandleLED(uint8_t);

/* interrupt/timer functions */
void InitTimer(void);
void WaitTimerTicks(uint32_t);

/* project global data */
extern volatile uint32_t timerticks;
extern volatile uint8_t lready;
extern volatile uint8_t leds[33];
extern volatile uint8_t leds_copy[33];
extern volatile uint8_t leds_changed;
extern volatile char buffer[128];
extern volatile uint16_t encoder_value;
extern volatile uint16_t old_encoder_value;
extern volatile uint8_t errflag;

/* laststate from eeprom.h - .section "EEPROM" */
extern uint8_t laststate[24];

/* serialio functions */
void sendchar(char);
void sendstring(char*);
void sendint(uint8_t);
char *recline(void);

/* delayms */
void delay_ms(uint16_t);

/* util functions */
uint8_t hextoint(char);

uint32_t idle_anim;
int16_t anim_leds[24];
int16_t  anim_leds_dir[24];

/* protothreads */
#include "pt.h"
int serloop(struct pt*);
int encloop(struct pt*);

#endif /* BL0RG_H */
