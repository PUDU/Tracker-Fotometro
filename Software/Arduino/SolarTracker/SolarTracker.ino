#include <DynamixelSerial.h>

#define MOTOR_ZENITH 2
#define MOTOR_AZIMUTH 3

#define ZENITH_AUMENTO 2
#define ZENITH_DECREMENTO 0
#define AZIMUTH_AUMENTO 1
#define AZIMUTH_DECREMENTO 3

#define HOLGURA 10
#define COMPENSADOR 1
int posZenith;
int posAzimuth;

int z_Aumento;
int z_Decremento;
int a_Aumento;
int a_Decremento;

void setup()
{
  Dynamixel.begin(1000000,2);  // Inicialize the servo at 1Mbps and Pin Control 2
  posZenith = Dynamixel.readPosition(MOTOR_ZENITH);
  posAzimuth = Dynamixel.readPosition(MOTOR_AZIMUTH);
  Dynamixel.move(MOTOR_ZENITH,posZenith);
  Dynamixel.move(MOTOR_AZIMUTH, posAzimuth);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(1000);
}

void loop()
{
  readSensor();
  compensateZenith();
  compensateAzimuth();
  delay(500); // PARA QUE MOTORES ALCANCEN A MOVERSE
}

void readSensor()
{
  z_Aumento = analogRead(ZENITH_AUMENTO);
  z_Decremento = analogRead(ZENITH_DECREMENTO);
  a_Aumento = analogRead(AZIMUTH_AUMENTO);
  a_Decremento = analogRead(AZIMUTH_DECREMENTO);
  
  
  
 // z_Aumento = random(300,600);
 // z_Decremento = random(300,600);
 // a_Aumento = random(300,600);
 // a_Decremento = random(300,600);
}

void compensateZenith()
{
  int dif_Zenith = z_Aumento - z_Decremento;
  
  if( dif_Zenith > 0)
      newPosZenith += 10;
  else
      newPosZenith -= 10;

  // Antiwinding up
  
  if(newPosZenith < 0)
    newPosZenith = 0;
    
  else if(newPosZenith > 1023)
    newPosZenith = 1023;
  
  //if(abs(dif_Zenith) > HOLGURA)  
    Dynamixel.move(MOTOR_ZENITH, newPosZenith);
  
  posZenith = newPosZenith;
  
  delay(500);
}

void compensateAzimuth()
{
  int dif_Azimuth = a_Aumento - a_Decremento;
  
  if( dif_Azimuth > 0)
      newPosAzimuth += 10;
  else
      newPosAzimuth -= 10;
  // Antiwinding up
  
  if(newPosAzimuth < 0)
    newPosAzimuth = 0;
    
  else if(newPosAzimuth > 1023)
    newPosAzimuth = 1023;
  
  //Revisa si se esta dentro de holgura
 // if(abs(dif_Azimuth) > HOLGURA)  
    Dynamixel.move(MOTOR_AZIMUTH, newPosAzimuth);
  
  posAzimuth = newPosAzimuth;
  
  delay(500);
}
