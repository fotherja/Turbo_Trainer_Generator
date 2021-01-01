#include "Arduino.h"

class Average 
{
  public:
    Average(unsigned char Size);
    int Rolling_Average(int Value);
    float Fraction_Filter(float Value);
    void Set_Fraction_Filter(float Value);
    void Set_Fraction_Filter_Const(float A, float B);
  
  private:
    int Num_Buffer[64];
    unsigned char Index;
    unsigned char FILTER_SIZE;
    float FF_Current = 0;
    float FF_A = 0.5;
    float FF_B = 0.5;
};

Average::Average(unsigned char Size)
{
  this->FILTER_SIZE = Size;
  this->Index = 0; 

  unsigned char i;
  for(i = 0;i < FILTER_SIZE;i++)
    this->Num_Buffer[i] = 0;
}

int Average::Rolling_Average(int Value)
{
  this->Num_Buffer[this->Index] = Value;                                // Put new value into buffer array.
  this->Index++;
    
  if(this->Index == FILTER_SIZE)                                        // Roll back Index if it overflows
    this->Index = 0;

  unsigned char i;
  long sum = 0;
  for(i = 0;i < FILTER_SIZE;i++)
    sum += this->Num_Buffer[i];

  return(sum/FILTER_SIZE);
}

float Average::Fraction_Filter(float Value)
{
  FF_Current = (FF_A*FF_Current) + (FF_B*Value);

  return(FF_Current);
}

void Average::Set_Fraction_Filter_Const(float A, float B)
{
  FF_A = A;
  FF_B = B;
}

void Average::Set_Fraction_Filter(float Value)
{
  FF_Current = Value;  
}
