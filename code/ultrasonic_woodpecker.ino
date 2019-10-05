
// Woodpecker  http://woodpecker.mobi  

/*                   Arduino I2C for a MaxSonar                         */
//////////////////////////////////////////////////////////////////////////
//  Arduino I2C for a MaxSonar by Carl Myhre is licensed under a        //
//  Creative Commons Attribution-ShareAlike 4.0 International License.  //
//  Original Author:  Carl Myhre, 10-02-2014, Revision: 1.0             //
//  Modifications by:                                                   //
//                                                                      //
//  Revision History: 1.0 -- 10-02-2014 -- Created initial code build   //
//                                                                      //
//  The original I2C libraries were created by Peter Fleury             //
//    http://homepage.hispeed.ch/peterfleury/avr-software.html          //
//                                                                      //
//  These libraries were adapted by Bernhard Nebel for use on Arduino   //
//    https://github.com/felias-fogg/SoftI2CMaster                      //
//                                                                      //
//  Special Thanks to MaxBotix Inc. for sponsoring this project!        //
//    http://www.maxbotix.com -- High Performance Ultrasonic Sensors    //
//                                                                      //
//  For more information on installing the I2C libraries for Arduino    //
//    visit http://playground.arduino.cc/Main/SoftwareI2CLibrary        //
//////////////////////////////////////////////////////////////////////////

//Hints on installing this code:
// 1. You will need to install the <SoftI2CMaster.h> library before using this code.
//      On Windows, the files are placed in C:\Program Files (x86)\Arduino\libraries\SoftI2CMaster\
// 2. As of 10-02-14 the Arduino library page (reference above) has the wrong name for the include file
//      it lists <SoftI2C.h> instead of <SoftI2CMaster.h> -- use the one that matches your installation.
// 3. Make sure to load the library into the Arduino compiler. 
//      To do this go to: SKETCH >> IMPORT LIBRARY... >> ADD LIBRARY...
//      Then navigate to C:\Program Files (x86)\Arduino\libraries\SoftI2CMaster\SoftI2CMaster.h
// 4. Be sure to set the SCL and SDA pins so that they match the pins you are using.
// 5. I have included 3 working "code examples" which differ from the 3 "functions" I included.
//      The functions are all that should be required to quickly use the I2C library to talk to a MaxSonar.
//      The three code examples show how I would implement each of the common tasks you may wish to do.
// 6. The included functions are as follows:
//      A. start_sensor(addr)
//      B. read_sensor(addr)
//      C. change_address(oldaddr,newaddr)
// 7. The included code examples are as follows:
//      A. read_the_sensor_example()
//      B. address_polling_example()
//      C. default_address_change_example()
// 8. You do not have to keep track of the error codes passed out by the installed functions if you do not want to.
//      I inluded the error tracking so that it was easy for others to build a reliable system -- and to ease
//      troubleshooting. (not using it makes for cleaner code if you trust your interface)

/*
Below, I define the SCL and SDA pins by their ATMEGA pins I have included links to common mappings below.
    UNO:  http://arduino.cc/en/Hacking/PinMapping168
    NANO: (matches UNO but has fewer pins)
    MEGA 2560: http://arduino.cc/en/Hacking/PinMapping2560
The current data matches the setup for the Arduino Uno -- they may need to be changed if the hardware changes.
You can also switch the I2C interface
to any of the tristate pins that you want (not just the SDA or SCL pins).
*/
#define SCL_PIN 5              //Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTC 
#define SDA_PIN 4              //Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTC
#define I2C_TIMEOUT 100        //Define a timeout of 100 ms -- do not wait for clock stretching longer than this time

/*
I have included a couple of extra useful settings for easy reference.
//#define I2C_CPUFREQ (F_CPU/8)//Useful if you plan on doing any clock switching
#define I2C_FASTMODE 1         //Run in fast mode (400 kHz)
#define I2C_SLOWMODE 1         //If you do not define the mode it will run at 100kHz with this define set to 1 it will run at 25kHz
*/
#include <SoftI2CMaster.h>     //You will need to install this library
#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN); 
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char Distance[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00};
unsigned char Distance1m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
unsigned char Distance2m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
unsigned char Distance3m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
unsigned char Distance4m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04};
unsigned char Distance5m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05};
unsigned char Distance6m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06};
unsigned char Distance7m[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07};
unsigned char DistanceFull[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09};
  int range;
  int range2;

void setup(){
  Serial.begin(115200);
  i2c_init();
      while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");

}

void loop()
{
    if (Serial.available() > 0) {
    int inByte = Serial.read();

     switch (inByte) {
       case '1':
        Serial.println("1m");
       range=1;
       range2=1;
       break; 
       case '2':
        Serial.println("2m");
       range=2;
       range2=2;
       break; 
      }  }

  read_the_sensor_example();

           if ((range >= 20 ) && (range <= 40)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance1m);
               } 
          else if ((range >= 41 ) && (range <= 80)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance2m);
               }     
          else if ((range >= 81 ) && (range <= 120)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance3m);
               }   
          else if ((range >= 121 ) && (range <= 160)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance4m);
               }   
          else if ((range >= 161 ) && (range <= 200)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance5m);
               }  
          else if ((range >= 201 ) && (range <= 240)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance6m);
               }  
          else if ((range >= 241 ) && (range <= 280)){
          CAN.sendMsgBuf(0x201, 0, 8, Distance7m);
               }     
         else if (range == 765 ){
          CAN.sendMsgBuf(0x201, 0, 8, DistanceFull);
               }     
          else {
           CAN.sendMsgBuf(0x201, 0, 8, Distance);
               }
         
           
           if ((range2 >= 20 ) && (range2<= 40)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance1m);
               } 
          else if ((range2 >= 41 ) && (range2 <= 80)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance2m);
               }     
          else if ((range2 >= 81 ) && (range2 <= 120)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance3m);
               }   
          else if ((range2 >= 121 ) && (range2 <= 160)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance4m);
               }   
          else if ((range2 >= 161 ) && (range2 <= 200)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance5m);
               }  
          else if ((range2 >= 201 ) && (range2 <= 240)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance6m);
               }  
          else if ((range2 >= 241 ) && (range2 <= 280)){
          CAN.sendMsgBuf(0x202, 0, 8, Distance7m);
               }     
         else if (range2 == 765 ){
          CAN.sendMsgBuf(0x202, 0, 8, DistanceFull);
               }     
         else {
           CAN.sendMsgBuf(0x202, 0, 8, Distance);
               }   
  //  delay(50);                       // send data per 100ms
}



///////////////////////////////////////////////////
// Function: Start a range reading on the sensor //
///////////////////////////////////////////////////
//Uses the I2C library to start a sensor at the given address
//Collects and reports an error bit where: 1 = there was an error or 0 = there was no error.
//INPUTS: byte bit8address = the address of the sensor that we want to command a range reading
//OUPUTS: bit  errorlevel = reports if the function was successful in taking a range reading: 1 = the function
//  had an error, 0 = the function was successful
boolean start_sensor(byte bit8address){
  boolean errorlevel = 0;
  bit8address = bit8address & B11111110;               //Do a bitwise 'and' operation to force the last bit to be zero -- we are writing to the address.
  errorlevel = !i2c_start(bit8address) | errorlevel;   //Run i2c_start(address) while doing so, collect any errors where 1 = there was an error.
  errorlevel = !i2c_write(81) | errorlevel;            //Send the 'take range reading' command. (notice how the library has error = 0 so I had to use "!" (not) to invert the error)
  i2c_stop();
  return errorlevel;
}



///////////////////////////////////////////////////////////////////////
// Function: Read the range from the sensor at the specified address //
///////////////////////////////////////////////////////////////////////
//Uses the I2C library to read a sensor at the given address
//Collects errors and reports an invalid range of "0" if there was a problem.
//INPUTS: byte  bit8address = the address of the sensor to read from
//OUPUTS: int   range = the distance in cm that the sensor reported; if "0" there was a communication error
int read_sensor(byte bit8address){
  boolean errorlevel = 0;
  int range = 0;
  byte range_highbyte = 0;
  byte range_lowbyte = 0;
  bit8address = bit8address | B00000001;  //Do a bitwise 'or' operation to force the last bit to be 'one' -- we are reading from the address.
  errorlevel = !i2c_start(bit8address) | errorlevel;
  range_highbyte = i2c_read(0);           //Read a byte and send an ACK (acknowledge)
  range_lowbyte  = i2c_read(1);           //Read a byte and send a NACK to terminate the transmission
  i2c_stop();
  range = (range_highbyte * 256) + range_lowbyte;  //compile the range integer from the two bytes received.
  if(errorlevel){
    return 0;
  }
  else{
    return range;
  }
}



/////////////////////////////////////////
// Function: Change the sensor address //
/////////////////////////////////////////
//Uses the I2C library to change the address of a sensor at a given address
//Collects and reports an error bit where: 1 = there was an error or 0 = there was no error.
//INPUTS: byte oldaddress = the current address of the sensor that we want to change
//INPUTS: byte newddress  = the address that we want to change the sensor to
//OUPUTS: bit  errorlevel = reports if the function was successful in changing the address: 1 = the function had an
//      error, 0 = the function was successful
boolean change_address(byte oldaddress,byte newaddress){
  //note that the new address will only work as an even number (odd numbers will round down)
  boolean errorlevel = 0;
  oldaddress = oldaddress & B11111110;  //Do a bitwise 'and' operation to force the last bit to be zero -- we are writing to the address.
  errorlevel = !i2c_start(oldaddress) | errorlevel; //Start communication at the new address and track error codes
  errorlevel = !i2c_write(170) | errorlevel;        //Send the unlock code and track the error codes
  errorlevel = !i2c_write(165) | errorlevel;        //Send the unlock code and track the error codes
  errorlevel = !i2c_write(newaddress) | errorlevel; //Send the new address
  i2c_stop();
  return errorlevel;
}



//////////////////////////////////////////////////////////
// Code Example: Read the sensor at the default address //
//////////////////////////////////////////////////////////
void read_the_sensor_example(){
  boolean error = 0;  //Create a bit to check for catch errors as needed.

  
  //Take a range reading at the default address of 224
  error = start_sensor(10); 
  error = start_sensor(12); //Start the sensor and collect any error codes.
  if (!error){                  //If you had an error starting the sensor there is little point in reading it as you will get old data.
    delay(50);
    range = read_sensor(10);
    range2 = read_sensor(12);//reading the sensor will return an integer value -- if this value is 0 there was an error
  //  Serial.print("R10:");Serial.print(range);Serial.print("  R12:");Serial.println(range2);
  }
}



////////////////////////////////////////////////////////////////
// Code Example: Poll all possible addresses to find a sensor //
////////////////////////////////////////////////////////////////
void address_polling_example(){
  boolean error = 0;  //Create a bit to check for catch errors as needed.
  int range = 0;
  Serial.println("Polling addresses...");
 
  //Walk through all possible addresses and check for a device that can receive the range command and will
  //    return two bytes.
  for (byte i=2; i!=0; i+=2){   //start at 2 and count up by 2 until wrapping to 0. Checks all addresses (2-254) except 0 (which cannot be used by a device)
    error = 0;
    error = start_sensor(i);    //Start the sensor and collect any error codes.
    if (!error){                //If you had an error starting the sensor there is little point in reading it.
      delay(100);
      range = read_sensor(i);   //reading the sensor will return an integer value -- if this value is 0 there was an error
      Serial.println(i);
      if (range != 0){
        Serial.print("Device found at:");Serial.print(i);Serial.print(" Reported value of:");Serial.println(range);
      }  
    }
    else{
      Serial.print("Couldn't start:");Serial.println(i);
    }
  }

  Serial.println("Address polling complete.");
}



//////////////////////////////////////////////
// Code Example: Change the default address //
//////////////////////////////////////////////
void default_address_change_example(){
  boolean error = 0;  //Create a bit to check for catch errors as needed.
  int range;
  
  Serial.println("Take a reading at the default address");
  
  //Take a range reading at the default address of 224
  error = start_sensor(224);    //Start the sensor and collect any error codes.
  if (!error){                  //If you had an error starting the sensor there is little point in reading it.
    delay(100);
    range = read_sensor(224);   //reading the sensor will return an integer value -- if this value is 0 there was an error
    Serial.print("R:");Serial.println(range);
  }
  
   Serial.println("Change the sensor at the default address to 222");
  //Change the address from 224 to 222
  error = 0;
//  error = change_address(224,222);  //Change the address -- I don't do anything with the error handler at this point but you can if you want.
  error = change_address(224,12);
  delay(200);    //Wait 125ms for the sensor to save the new address and reset
  
   Serial.println("Take a reading at the new address");
  
  //Take a range reading at the new address of 222
  error = 0;
  error = start_sensor(12);     //Same as above but at the new address
  if (!error){
    delay(100);
    range = read_sensor(12);
    Serial.print("N:");Serial.println(range);
  }  
  
 //  Serial.println("Change the sensor back to the default address");  
  
  //Change the address from 222 to 224
 // error = 0;
//  error = change_address(222,224);
//error = change_address(222,2);
 // delay(200);    //Wait 125ms for the sensor to save the new address and reset
}
