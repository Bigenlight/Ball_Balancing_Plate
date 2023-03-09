#include <Arduino.h>
#include "matrix.h"

// mat::mat(double x, double y){
//   A[0][0] = x;
//   A[0][1] = y;
// }

void mat::matcal(double x, double y){
  A[0][0] = x;
  A[0][1] = y;
  
  for(i=0;i<3;i++)
  {
   for(j=0;j<1;j++)
   {
   D[i][j]=B[i][0]*A[0][0] + B[i][1]*A[1][0] + B[i][2]*A[2][0];          
   }
  } 
    
  for(i=0;i<3;i++)
  {
   for(j=0;j<1;j++)
   {
   E[i][j]=C[i][0]*D[0][0] + C[i][1]*D[1][0] + C[i][2]*D[2][0];          
   }
  }
}

void mat::update(double x, double y){
  matcal(x, y);
  Serial.print(" x : ");
  Serial.print(E[0][0]);
  Serial.print("cm, y : ");
  Serial.print(E[1][0]);
  Serial.println("cm");
}

double mat::getx(){
  return E[0][0];
}
double mat::gety(){
  return E[0][1];
}