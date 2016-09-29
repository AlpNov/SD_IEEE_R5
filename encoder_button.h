/*
 * encoder_button.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Christopher
 */

#ifndef ENCODER_H_
#define ENCODER_H_


void encoderInit(void);
void ButtonInit(void);
void Button_reenable(void);
void encoder_ISR(void);
void clearCount(void);
void encoderCounter(void);
void encoderCounter_5(void);
void encoderCounter_20(void);
void encoderCounter_50(void);
void encoderCounter_500(void);
void encoderCounter_N(int n);
int32_t getencoder(void);





#endif /* ENCODER_H_ */
