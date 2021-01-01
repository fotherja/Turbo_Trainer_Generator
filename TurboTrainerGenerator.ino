/*
  Created: 29/12/2020
  Author:  James Fotherby
  Description: Turbo Trainer generator

Motor current is proportional to motor torque. Torque equates to acceleration via F = MA.

RPM = Integral(Torque - Drag)dt                                         *Drag = (b x V) + (c x V^2)
RPM = Integral([a x Motor_Current] - [(b x RPM) + (c x RPM^2)])dt
*/

#include <VescUart.h>
#include "Average.h"

#define       a         -20.0
#define       b         0.015
#define       c         0.75e-6

VescUart        UART;
Average         FilterI(0);

void setup() {
  FilterI.Set_Fraction_Filter_Const(0.92, 0.08);                            // Exponential filter
  FilterI.Set_Fraction_Filter(0.0);                                         
  
  Serial.begin(115200);  
  while (!Serial) {;}
 
  UART.setSerialPort(&Serial);
}

void loop() {
  static float RPM = 2000.0;
  static float Motor_RPM_Measured = 0.0;
  static float Motor_Current = 0.0;
  
  delay(40);                                                                // Run the loop about 25 times/second
  
  if(UART.getVescValues()) {
    float Motor_Current_Unfiltered = min(0.0, UART.data.avgMotorCurrent);   // Read the regen motor current from the VESC
    Motor_Current = FilterI.Fraction_Filter(Motor_Current_Unfiltered);      // perform exponential filtering on the regen current
    Motor_RPM_Measured = UART.data.rpm;
  }

  if(Motor_RPM_Measured > 2000.0)  {                                        // If the motor ERPM > 2000 operate in speed control mode                                              
    RPM = RPM + (a*Motor_Current - b*RPM - c*RPM*RPM);                        
    UART.setRPM(RPM);  
  }
  else  {                                                                   // Essentially disengage the motor if it is spinning very slowly
    UART.setBrakeCurrent(0.0);    
    UART.setCurrent(0.0);
    RPM = 2035.0;                                                      
  }
}
