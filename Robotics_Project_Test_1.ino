
#include<Wire.h>
#include <Servo.h>
Servo servoX;
const int MPU_addr=0x68;//Register for reseting the mpu6050
int16_t axis_X,axis_Y,axis_Z;//MPU spits things out in 16 bit so voila

int minVal=265;//the raw data tends to fall within here
int maxVal=402;

double x;
double y;
double z;
int previous_degX = 0;
int previous_degY = 0;
int current_degX = 0;//buffer variables 1/2 for tracking difference for the servo movement
int current_degY = 0;//buffer variables 1/2 for tracking difference for the servo movement
int movX = 0;
int movY = 0;
int abs_diffX = 0;//to cover cases where the MPU changes from something like 359 to 1, and thinks they are like 358 degrees apart
int abs_diffY = 0;//when i really care if they are 2 degrees apart
int current_servo_posX = 90;
int current_servo_posY = 90;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);//sends the command to wake the gyro up (power management)
  Wire.write(0);
  Wire.endTransmission(true);
  servoX.attach(9);
  Serial.begin(9600);
  servoX.write(current_servo_posX);
}
void loop(){
  Wire.beginTransmission(MPU_addr);//start talking to mpu again
  Wire.write(0x3B);//X out Register Accelerator
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);//to ask the MPU 14 bytes and close connection
  axis_X=Wire.read()<<8|Wire.read();//get the value from the X register then shift over to next for Y and Z
  // Serial.print("Raw X value = ");
  // Serial.println(axis_X);

  axis_Y=Wire.read()<<8|Wire.read();
  // Serial.print("Raw Y value = ");
  // Serial.println(axis_Y);

  axis_Z=Wire.read()<<8|Wire.read();
  // Serial.print("Raw Z value = ");
  // Serial.println(axis_Z);

    int xAng = map(axis_X,minVal,maxVal,-90,90);
    int yAng = map(axis_Y,minVal,maxVal,-90,90);
    int zAng = map(axis_Z,minVal,maxVal,-90,90);

       x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
       y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
       z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

    current_degX = x;
    current_degY = y;
    
    
     Serial.print("Angle of inclination in X axis = ");//tilting the breadboard up or down
    Serial.println(x);

     Serial.print("Angle of inclination in Y axis = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
     Serial.println(y);

     //Serial.print("Angle of inclination in Z axis= ");my brain cant figure out which way to rotate the breadboard that affects these numbers
     //Serial.println(z);
     
     
    abs_diffX = current_degX + previous_degX;
    abs_diffY = current_degY + previous_degY;
    
    
    if(abs_diffX > 300){
    Serial.print("abs_diffX = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
    Serial.println(abs_diffX);
      if(current_degX > previous_degX){
        movX = (360-current_degX)+previous_degX;

      }
      if(previous_degX > current_degX){
        movX = (360-previous_degX)+current_degX;

      }
    }
    if(abs_diffY > 300){
    Serial.print("abs_diffY = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
    Serial.println(abs_diffY);
      if(current_degY > previous_degY){
        movX = (360-current_degY)+previous_degY;

      }
      if(previous_degY > current_degY){
        movX = (360-previous_degY)+current_degY;

      }
    }    
    movX = current_degX - previous_degX;
    movY = current_degY - previous_degY;

    Serial.print("movX = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
     Serial.println(movX);
    // Serial.print("movY = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
    //  Serial.println(movY);
    
    
     if(movX > 0){
    //   Serial.print("MovX = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
    //  Serial.println(movX)
    current_servo_posX = 90-movX;
      servoX.write(current_servo_posX);
     }
     if(movX < 0){
    //   Serial.print("MovX = ");//rotating breadboard L/R when looking down towards the hole and the arduino is on bottom
    //  Serial.println(movX);
    current_servo_posX = 90+movX;
      servoX.write(current_servo_posY);
     }
     previous_degX = current_degX;//buffer variables 2/2 for tracking difference for the servo movement
     previous_degY = current_degY;//buffer variables 2/2 for tracking difference for the servo movement
     Serial.println("-------------------------------------------");
     delay(250);
}