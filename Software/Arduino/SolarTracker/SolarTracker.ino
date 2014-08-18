#include <DynamixelSerial.h>

#define MOTOR_ZENITH 2
#define MOTOR_AZIMUTH 3

#define ZENITH_AUMENTO 2
#define ZENITH_DECREMENTO 0
#define AZIMUTH_AUMENTO 1
#define AZIMUTH_DECREMENTO 3

#define HOLGURA 15

double K = 0.05;
int posZenith;
int posAzimuth;

int z_Aumento=0;
int z_Decremento=0;
int a_Aumento=0;
int a_Decremento=0;

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
  
  //if( dif_Zenith > 0)
  int newPosZenith = posZenith + dif_Zenith*K;
  //else
  //    newPosZenith -= 10;
  
  if(abs(dif_Zenith) > HOLGURA)
  { 
    // Antiwinding up
    if(newPosZenith < 0)
      newPosZenith = 0;
      
    else if(newPosZenith > 1023)
      newPosZenith = 1023; 
    
    Dynamixel.move(MOTOR_ZENITH, newPosZenith);
    delay(abs(10*dif_Zenith*K));  
    posZenith = newPosZenith;
  }
  
}

void compensateAzimuth()
{
  
  int dif_Azimuth = a_Aumento - a_Decremento;
  
  //Corrige en caso de motor Zenith Invertido
  if(posZenith < 511)
    dif_Azimuth = -1*dif_Azimuth;
    
  int newPosAzimuth = posAzimuth + dif_Azimuth*K;  
  
  //Revisa si se esta dentro de holgura
  if(abs(dif_Azimuth) > HOLGURA)
  { 
    // Antiwinding up
    if(newPosAzimuth < 0)
      newPosAzimuth = 0;
      
    else if(newPosAzimuth > 1023)
      newPosAzimuth = 1023; 
    
    Dynamixel.move(MOTOR_AZIMUTH, newPosAzimuth);
    delay(abs(10*dif_Azimuth*K));
    
    posAzimuth = newPosAzimuth;    
  }
  
}
