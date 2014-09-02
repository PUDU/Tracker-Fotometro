#include <DynamixelSerial.h>

#define MOTOR_ZENITH 2
#define MOTOR_AZIMUTH 3

#define ZENITH_AUMENTO 2
#define ZENITH_DECREMENTO 0
#define AZIMUTH_AUMENTO 1
#define AZIMUTH_DECREMENTO 3



#define HOLGURA 10

#define DEFAULT_ZENITH 511
#define DEFAULT_AZIMUTH 511


#define PASO 1
#define DANCE 1
//double K = 0.05;
int posZenith;
int posAzimuth;

int z_Aumento;
int z_Decremento;
int a_Aumento;
int a_Decremento;

void setup()
{
  Dynamixel.begin(1000000,2);  // Inicialize the servo at 1Mbps and Pin Control 2
  posZenith = DEFAULT_ZENITH;
  posAzimuth = DEFAULT_AZIMUTH;
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
  delay(50);
  
  
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
  
  if(abs(dif_Zenith) > HOLGURA)
  { 
    if(dif_Zenith > 0)
      posZenith += PASO;
      
    else
      posZenith -= PASO; 
  }
  
  else
  {
      posZenith += ( (int) random(-DANCE, DANCE) );
  }
  
  // Antiwinding up
   posZenith = antiwindingUp( posZenith );
    
   Dynamixel.move(MOTOR_ZENITH, posZenith); 
  
}

void compensateAzimuth()
{
  
  int dif_Azimuth = a_Aumento - a_Decremento;
  int pasoA = PASO;  
    
  if(abs(dif_Azimuth) > HOLGURA)
  {
      
      if(posZenith < 511)
        pasoA = -1*pasoA;
      
    
      if(dif_Azimuth > 0)
        posAzimuth += pasoA;
        
      else
        posAzimuth -= pasoA;
  }
  
  else
  {
      posAzimuth += ( (int) random(-DANCE*2, DANCE*2) );
  }
     
     // Antiwinding up
      posAzimuth = antiwindingUp( posAzimuth ); 
     
      Dynamixel.move(MOTOR_AZIMUTH, posAzimuth);

}

int antiwindingUp(int x)
{
    if(x < 0)
        x = 0;
        
    else if(x > 1023)
        x = 1023;
       
    return x; 
}

