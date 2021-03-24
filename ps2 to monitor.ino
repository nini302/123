#include <PS2X_lib.h>           //include ps2 console library 

//ps2 port setting
//from left to right
#define dataPort 8                     //port 1: data
#define commandPort 9                  //port 2: command
                                        //port 3: shock motor power

//goundPort GND                         //port 4: ground
//Vcc(3.3V) Vin                         //port 5: 3.3V power
#define attPort 10                      //port 6: attribute select

#define clockPort 11                    //port 7: clock
                                        //port 8: NULL
                                        //port 9: ACK response

//feature setting
#define supportAnalogJoystick true      //does it support anolog joystick?
#define supportVibration true           //does it support vibration?

//initializing data
PS2X ps2x;                      //create object ps2 console
int error = 0;
byte type = 0;
byte vibrate = 0;

//dead area set
#define deadArea 5







//define car wheel motor
//Left wheel
//const int wheelLeft1 = 2;
//const int wheelLeft2 = 3;
//const int wheelLeftS = 3;
const int wheelLeftSpeed = 200;

//Right wheel
//const int wheelRight1 = 4;
//const int wheelRight2 = 5;
//const int wheelRightS = 11;
const int wheelRightSpeed = 200;





void setup()
{
  	Serial.begin(9600);
  	Serial.println();
  	Serial.println("start");

  	//test ps2 console is connected or not
  	do
  	{  
    	error = ps2x.config_gamepad(clockPort, commandPort, attPort, dataPort, supportAnalogJoystick, supportVibration);
    	if(error == 0)            Serial.println("Controller found");
    	else if(error == 1)       Serial.println("ERROR: No Controller found");
    	else if(error == 2)       Serial.println("ERROR: Controller not accepting command");  
    	else if(error == 3)       Serial.println("ERROR: Controller refusing to enter Pressures Mode, may not support it");
	}while(error != 0);

  	//determine controller type
  	type = ps2x.readType();
  	switch(type)
  	{
  	  	case 0:   Serial.println("Unknown controller type");      break;
  	  	case 1:   Serial.println("DualShock controller found");   break;
    	case 2:   Serial.println("GuitarHero controller found");    break;
  	}

}

void loop()
{
	//if no controller found, stop arduino
  	if(error == 1)    return;

  	//read_gamepad( small motor(boolean), big motor(byte) )
  	//you should at least do this once for one second
  	ps2x.read_gamepad(false, vibrate);

  	//read ps2 gamepad
  	int ps2_left_analog_Y = 255 - ps2x.Analog(PSS_LY);
  	int ps2_right_analog_X = ps2x.Analog(PSS_RX);
  	Serial.print("ps2 left y = ");				Serial.println(ps2_left_analog_Y);
  	Serial.print("ps2 right x = ");				Serial.println(ps2_right_analog_X);

  	bool front = (ps2_left_analog_Y  > 256/2 + deadArea/2);
  	bool back  = (ps2_left_analog_Y  < 256/2 - deadArea/2);
  	bool right = (ps2_right_analog_X > 256/2 + deadArea/2);
  	bool left  = (ps2_right_analog_X < 256/2 - deadArea/2);

  	if( front && !back && !right && !left)			//foward
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * 0.8);
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * 0.8);
	}
	if(!front &&  back && !right && !left)			//backward
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * (-0.8));
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * (-0.8));
	}
	if(!front && !back &&  right && !left)			//turn right
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * (-0.5));
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * 0.5);
	}
	if(!front && !back && !right &&  left)			//turn left
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * 0.5);
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * (-0.5));
	}
	if( front && !back &&  right && !left)			//front right
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * 0.6);
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * 1);
	}
	if( front && !back && !right &&  left)			//front left
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * 1);
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * 0.6);
	}
	if(!front &&  back &&  right && !left)			//back right
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * (-0.6));
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * (-1));
	}
	if(!front &&  back && !right &&  left)			//back left
  	{
  		Serial.print("right motor speed = ");	Serial.println(wheelRightSpeed * (-1));
  		Serial.print("left  motor speed = ");	Serial.println(wheelLeftSpeed  * (-0.6));
	}
	if(!front && !back && !right && !left)			//stop
  	{
  		Serial.println("right motor speed = 0");
  		Serial.println("left  motor speed = 0");
	}

	Serial.println("");

  	delay(500);
}