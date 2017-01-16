#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define LED_PIN 13
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 30 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;

// Define an array for holding sensor values.
#define NUM_SENSORS 6
unsigned int sensorValues[NUM_SENSORS];
bool go = true;           //Whether corridor functionality is enabled
bool room = false;        //Whether the Zumo is in a room
bool corner = false;      //Will be true during corner operation
int mins[6];              //Array holding the minimum values based on the calibration plus the allowance
int allowance = 200;      //Allowance so that the trigger values for each sensor can adapt to uneven light levels
int turnSpeed = 100;      //The speed at which the robot will turn
int numberOfRooms = 0;    //Keeps track of the number of rooms the Zumo has visited
int numberOfPeople = 0;   //Keeps track of the number of people the Zumo has found
int numberOfCorners = 0;  //Keeps track of the number of Corners the Zumo has turned
String corners[20];       //An Array to hold the direction of each corner
String rooms[20];         //An Array to hold the Direction of each room
String person[20];        //This Array holds the location of each person
bool found = false;       //Whether a person has been found during the current action
bool corridorEnd = false; //Whether the robot has reached the end of the track
String priorFeature = "Start";    //If no corner or room has been met but there is a person in the corridor then the start is the last event
String string1 = "Room ";
String string2 = "Person found!";

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  reflectanceSensors.init();

  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);               // turn on LED to indicate we are in calibration mode
  
  unsigned long startTime = millis();
  while(millis() - startTime < 5000)    // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  digitalWrite(13, LOW);                 // turn off LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on plus the allowance
  Serial.begin(9600);
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    mins[i] = (reflectanceSensors.calibratedMinimumOn[i] + allowance);  //add the allowance to the calibrated minimum
    Serial.print(reflectanceSensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
}

void loop()
{

  unsigned int position = reflectanceSensors.readLine(sensorValues);
  digitalWrite(LED_PIN, HIGH);
  char getData = Serial.read();
  
  if (room) {                                         //If the 'r' key has been pressed to signify a room
    if (getData == 'a') {                             //Seperate turning keys so that the direction can be recorded
      motors.setLeftSpeed(-turnSpeed);
      motors.setRightSpeed(turnSpeed);
      rooms[numberOfRooms] = "Left";
    } else if (getData == 'd') {
      motors.setLeftSpeed(turnSpeed);
      motors.setRightSpeed(-turnSpeed);
      rooms[numberOfRooms] = "Right";
    } else if (getData == ' ') {                      //Once the user has pressed ' ' to show the Zumo is facing the room
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
      delay(500);
      Serial.println();
      Serial.print(string1 + (numberOfRooms + 1));    //Display the room number
      Serial.println();
      Serial.print(rooms[numberOfRooms]);             //Display the direction of the room
      Serial.println();
      motors.setLeftSpeed(100);                       //Enter the Room
      motors.setRightSpeed(100);
      delay(400);
      motors.setLeftSpeed(-turnSpeed);                //Turn towards the left side of the room
      motors.setRightSpeed(turnSpeed);
      for(int i=0;i<16;i++) {                         //While turning use the U/s to search for people
        if(sonar.ping_cm()>0) {
          found = true;
        }
        delay(50);
      }
      motors.setLeftSpeed(turnSpeed);                 //Turn (Right) through 180 degrees to search the entire room
      motors.setRightSpeed(-turnSpeed);
      for(int i=0;i<32;i++) {
        if(sonar.ping_cm()>0) {
          found = true;
        }
        delay(50);
      }
      motors.setLeftSpeed(-turnSpeed);                //Turn (Left) back to face straight
      motors.setRightSpeed(turnSpeed);
      for(int i=0;i<16;i++) {                         //By seaching 3 times it helps to ensure that any people will be found
        if(sonar.ping_cm()>0) {
          found = true;
        }
        delay(50);
      }
      motors.setLeftSpeed(0);                         //Stop the robot in preperation for the user to control the robot out of the room
      motors.setRightSpeed(0);
      numberOfRooms++;                                //Increase the number of rooms
      room = false;                                   //Signify the room behaviour is finished
      if (found) {                                                  //If a person has been found
        person[numberOfPeople] = string1 + String(numberOfRooms);   //Set the room as the location of the person
        numberOfPeople++;                                           //Increase the number of people
        Serial.print(string2);                                      //Display "Person found!" on serial
        Serial.println();
        found = false;                                              //Reset found to false
      }
      priorFeature = "Room " + String(numberOfRooms);               //Set the room as the latest thing that has happened
    }
  } else {
    if (corner) {                                         //If the corner behaviour has been activated
      if (getData == 'a') {                               //Seperate turning actions so that the direction of the corner can be set
        corners[numberOfCorners] = "Left";                //Set the direction of this corner
        corner = false;                                   //Show that the corner behaviour is finished
        numberOfCorners++;                                //Increase the number of corners by 1
        priorFeature = "Turn " + String(numberOfCorners); //Set this corner as the latest thing that has happened
        Serial.println("Turned Left")                     //Display which way the corner was turned
      } else if (getData == 'd') {
        corners[numberOfCorners] = "Right";
        corner = false;
        numberOfCorners++;
        priorFeature = "Turn " + String(numberOfCorners);
        Serial.println("Turned Right")
      }
    }
    if (getData == 'w') {                       //Direction keys for controlling the robot
      motors.setLeftSpeed(100);
      motors.setRightSpeed(100);
    } else if (getData == 'a') {
      motors.setLeftSpeed(-turnSpeed);
      motors.setRightSpeed(turnSpeed);
    } else if (getData == 's') {
      motors.setLeftSpeed(-100);
      motors.setRightSpeed(-100);
    } else if (getData == 'd') {
      motors.setLeftSpeed(turnSpeed);
      motors.setRightSpeed(-turnSpeed);
    } else if (getData == ' ') {                //Stop the Zumo
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
    } else if (getData == 'r') {                //Signal to the Zumo that it is outside a room
      motors.setLeftSpeed(0);                   //Stop the robot outside the room
      motors.setRightSpeed(0);
      room = true;                              //Activate the room actions
      go = false;                               //Disable corridor actions
    }
    if (!go) {                                  //If corridor actions are currently disabled
      if (getData == 'c') {                     //Key to reset corridor actions
        motors.setLeftSpeed(0);
        motors.setRightSpeed(0);
        go = true;                              //Enable corridor behaviour
        found = false;                          //Ensure that every time the Zumo returns to the corridor it is ready to detect a person
      }
    } else {
      if(!found) {
        for(int i=0;i<10;i++) {                                           //Ping 10 times to accoutn for error
          if (sonar.ping_cm()>0){                                         //If a person is detected
            go = false;                                                   //Disable Corridor behaviour
            found = true;                                                 //Signal that a person has been found
            person[numberOfPeople] = "Corridor after " + priorFeature;    //Set the location of the person to be the corridor after that last event
            numberOfPeople++;                                             //Increase the number of people found
            motors.setLeftSpeed(0);                                       //Stop the Zumo so that the person can be removed
            motors.setRightSpeed(0);
            Serial.println();
            Serial.print("Person Found in Corridor after " + priorFeature); //Display a message stating that a person has been found in the corridor and the event it was after
            Serial.println();
          }
        }
      }
      if (sensorValues[1] > mins[1] || sensorValues[2] > mins[2] || sensorValues[3] > mins[3] || sensorValues[4] > mins[4]) {  //Any of the middle 4 sensors detect a line
        go = false;                       //Disable the corridor behaviour
        corner = true;                    //Signal a corner has been found
        motors.setLeftSpeed(-100);        //Reverse away from the line
        motors.setRightSpeed(-100);
        delay(300);
        motors.setLeftSpeed(0);           //Stop the Zumo
        motors.setRightSpeed(0);
        Serial.println("Found Corner")    //Display a message showing a corner has been found
      } else if (sensorValues[0] > (mins[0] + 50)) {    //The outside sensors are less sensitive so that corners are detected more easily
        motors.setLeftSpeed(0);
        motors.setRightSpeed(0);
        delay(200);
        motors.setLeftSpeed(100);         //Turn the Zumo back towards the corridor
        motors.setRightSpeed(-100);
        delay(500);
        motors.setLeftSpeed(100);
        motors.setRightSpeed(100);
      } else if (sensorValues[5] > (mins[5] + 50)) {
        motors.setLeftSpeed(0);
        motors.setRightSpeed(0);
        delay(200);
        motors.setLeftSpeed(-100);
        motors.setRightSpeed(100);
        delay(500);
        motors.setLeftSpeed(100);
        motors.setRightSpeed(100);
      }
    }
  }
}
