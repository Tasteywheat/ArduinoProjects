
#ifndef MotorShield_h
#define MotorShield_h
#include "Arduino.h"

#define M1E 5
#define M2E 6
#define M1 8
#define M2 7
#define MAX_SPEED 250
#define M1OFFSET -20
#define M2OFFSET 0


class MotorShield {
  private:
	bool m1Dir;
	bool m2Dir;
  public:
    MotorShield();
    void motorsRun(int m1Spd, int m2Spd);
    void motorsStop();
};


#endif
