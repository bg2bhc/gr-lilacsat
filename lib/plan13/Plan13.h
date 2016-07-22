/* 
 *
 * This file is a C port by Edson Pereira, PY2SDR 
 * <http://www.qsl.net/n1vtn/>
 *
 * based on the Plan 13 keplerian propagator developed by James Miller, G3RUH
 * <http://www.amsat.org/amsat/articles/g3ruh/111.html>
 *
 */



#ifndef plan13_h
#define plan13_h
//#include "Arduino.h"

class Plan13 {

public:
double rad(double deg);
double deg(double rad);
double FNatn(double y, double x);
double FNday(int year, int month, int day);
double myFNday(int year, int month, int day, int uh, int um, int us);
double getElement(char *gstr, int gstart, int gstop);
void readElements(char *satellite);
void initSat(void);
void satvec(void);
void rangevec(void);
void sunvec(void);
void calculate(void);
float *footprintOctagon(float slat, float slon);
void printdata(void);
void setFrequency(unsigned long rx_frequency, unsigned long tx_frequency);
void setLocation(double lon, double lat, int height);
void setTime(int yearIn, int monthIn, int mDayIn, int hourIn, int minIn, int secIn);
void setElements(double YE_in, double TE_in, double IN_in, double
         RA_in, double EC_in, double WP_in, double MA_in, double MM_in,
		double M2_in, double RV_in, double ALON_in );
	int getDoppler(unsigned long freq);
	int getDoppler64(unsigned long freq);

double rx, tx;
double observer_lon;
double observer_lat;
int observer_height;
unsigned long rxOutLong;
unsigned long txOutLong;
unsigned long rxFrequencyLong;
unsigned long txFrequencyLong;
float dopplerFactor;
const static double   YM = 365.25;                           /* Days in a year                     */
double   EL;                           /* Elevation                          */
double   TN;                           /*                                    */

double   E;
double   N;
double   AZ;
double   SLON;
double   SLAT;
double   RR;

double   CL;
double   CS;
double   SL;
double   CO;
double   SO;
double   RE;
double   FL;
double   RP;
double   XX;
double   ZZ;
double   D;
double   R;
double   Rx;
double   Ry;
double   Rz;
double   Ex;
double   Ey;
double   Ez;
double   Ny;
double   Nx;
double   Nz;
double   Ox;
double   Oy;
double   Oz;
double   U;
double   Ux;
double   Uy;
double   Uz;
const static double   YT = 365.2421970;
double   WW;
double   WE;
double   W0;
double   VOx;
double   VOy;
double   VOz;
double   DE;
double   GM;
double   J2;
double   N0;
double   A0;
double   b0;
double   SI;
double   CI;
double   PC;
double   QD;
double   WD;
double   DC;
double   YG;
double   G0;
double   MAS0;
double   MASD;
double   INS;
double   CNS;
double   SNS;
double   EQC1;
double   EQC2;
double   TEG;
double   GHAE;
double   MRSE;
double   MASE;
double   ax;
double   ay;
double   az;
int      OLDRN;

double   T;
double   DT;
double   KD;
double   KDP;
double   M;
int      DR;
long     RN;
double   EA;
double   C;
double   S;
double   DNOM;
double   A;
double   B;
double   RS;
double   Sx;
double   Sy;
//double   Sz;
double   Vx;
double   Vy;
double   Vz;
double   AP;
double   CW;
double   SW;
double   RAAN;
double   CQ;
double   SQ;
double   CXx;
double   CXy;
double   CXz;
double   CYx;
double   CYy;
double   CYz;
double   CZx;
double   CZy;
double   CZz;
double   SATx;
double   SATy;
double   SATz;
double   ANTx;
double   ANTy;
double   ANTz;
double   VELx;
double   VELy;
double   VELz;
double   Ax;
double   Ay;
double   Az;
double   Sz;
//double   Vz;
double   GHAA;

double   DS;
double   DF;

/* keplerians */

char     SAT[20];
long     SATNO;
double   YE;
double   TE;
double   IN;
double   RA;
double   EC;
double   WP;
double   MA;
double   MM;
double   M2;
long     RV;
double   ALON;
double   ALAT;
double   rxOut;
double   txOut;

/* location */
char     LOC[20];
double   LA;
double   LO;
double   HT;

double      HR;                        /* Hours */
double      DN;
private:
void	foo();
};
#endif
