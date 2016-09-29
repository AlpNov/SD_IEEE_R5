/*
 * cmd_int.h
 *
 *  Created on: Feb 3, 2015
 *      Author: cmahn
 */

#ifndef CMD_INT_H_
#define CMD_INT_H_

struct cmd_to_funct{
    char cmd[3]; //command characters
    void (*fn)(void); //command function
    char explain[50];
};
void Forward(void);
void ReverseV(void);
void Left1Test(void);
void Left2Test(void);
void Right1Test(void);
void Right2Test(void);
void Brake(void);
void Test(void);
void TestClear(void);
void Drive_Start(void);
void Drive_Clock(void);
void LineStop(void);
void DistancePrint(void);
void sideLinePrint(void);
void picTest(void);
void Help(void);
void Nothin(void);
#endif /* CMD_INT_H_ */
