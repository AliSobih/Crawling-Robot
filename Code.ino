#include <Servo.h>
Servo servo1, servo2;	// Object of Servo Lib


  float distance;		// Var to store Distance 
  float sonarTime;		// Var to Store Sonar Time

	//prototype of myServo Method
  void myServo(Servo servo, int newAngle, int angleInc, int incDelay, int servoNum);
 
  int TRIGGER=7, ECHO=8;// sonar pins
   
  int episodes = 0;		// Var to store the number needed learn the model with
  int spos1 = 0; 		// servo1 position
  int spos2 = 0; 		// servo2 position
  int spos3 = 0; 		// servo1 position
  int spos4 = 0;  		// servo2 position
  int spos1High = 0; 	// servo1 highest position 
  int spos2High = 0;	// servo2 highest position
  int spos3High = 0;	// servo1 highest position
  int spos4High = 0;	// servo2 highest position
  int distanceHigh = 0;	// Var to store the higest distance travelled
 
// Vars to hold the distance values

  float distDifference = 0,distPrevious = 0,distCurrent = 0;

void setup (){
  // init for all
  
  Serial.begin(9600);			// start the console
  servo1.attach(9, 600, 2400);	// set the servo to pin 9, with 600 min (0 - Degree) PWM, 2400 max PWM (180 - Degree)
  servo2.attach(6, 600, 2400);	// set the servo to pin 6, with 600 min PWM, 2400 max PWM 
  
  // set servos to zero position
  myServo(servo1, 0, 1, 8, 1); 		
  delay(1000);
  myServo(servo2, 0, 1, 8, 1);
  delay(1000);
  
  // setup sonar
  pinMode(TRIGGER, OUTPUT); 
  pinMode(ECHO, INPUT);
 

  randomSeed(analogRead(0)); 	// get real random number seed
 
  distPrevious = getDistance(); //get initial distance
  Serial.println(distPrevious);
  delay(1000);
  
  } // end setup
  
void myServo(Servo servo, int newAngle, int angleInc, int incDelay, int servoNum) {
// routine to controll the servo
  
  int curAngle = 0;
  curAngle = servo.read();
 
// writing the servo new angle so that it increases by angleInc Var each time
  if (curAngle < newAngle) {
   	for(int angle = curAngle; angle < newAngle; angle += angleInc) {
         servo.write(angle);	// apply the new angle to the servo
         delay(incDelay);   
   	}
   }
   else if (curAngle > newAngle) {
      for(int angle = curAngle;angle > newAngle; angle -= angleInc) {
          servo.write(angle);	// apply the new angle to the servo
          delay(incDelay);   
      }
   }
} // end of myServo function

float getDistance() {  
// routine to measure distance = call and average it 5 times
  int numberTriggers = 5;		
  int average = 0;
  for(int i = 0; i < numberTriggers; i++) {
     digitalWrite(TRIGGER, LOW);			// write 0 to the trigger of the Ultrasonic Sensor
     delayMicroseconds(5);					// wait
     digitalWrite(TRIGGER, HIGH);			// write 1 to the trigger of the Ultrasonic Sensor
     delayMicroseconds(10);					// wait
     digitalWrite(TRIGGER, LOW);			// write 0 to the trigger of the Ultrasonic Sensor
     sonarTime = pulseIn(ECHO, HIGH);		// determine the pulse time between sending wave and recieving it
     distance = sonarTime * 340 / 20000;	// calculating the distance so far based on the duration of the pulse (Speed = Distance / Time)
     average = average + distance;			// adding to the total average distance
     delay(100);							// wait
  } // end for i
  
  average = average / numberTriggers;		// averaging 
return average;

}// end get ultrasonic distance routine


void doTraining() {  
// routine to train the model (find the best postions that will achieve its task)
  Serial.println("Do doTraining...");
  episodes = 40;  
  for (int episode = 0; episode < episodes; episode++) {
    // for 10 episodes (even 5 episodes usually works)
            spos1 = random(160);				// random number between 0 - 160
            spos2 = random(160);				// random number between 0 - 160
           
    		//why the following 2 ?
    		spos3 = random(160);				// random number between 0 - 160
            spos4 = random(160);				// random number between 0 - 160
    
            myServo(servo1, spos1, 1, 7, 1);	// moving the motor with the new postions	
            myServo(servo2, spos2, 1, 7, 1);	// moving the motor with the new postions
            myServo(servo1, spos3, 1, 7, 1);	// moving the motor with the new postions
            myServo(servo2, spos4, 1, 7, 1);	// moving the motor with the new postions
    
    // get distance - note this is not always accurate so sometimes robot will just claw the air
            
    		distCurrent = getDistance(); 
            distDifference = distCurrent - distPrevious;
            distPrevious = distCurrent;
    
    // print the Current episode (Learning iteration)
            Serial.print(" episode = ");Serial.print(episode);
    // print the Current Postion of each motor for this episode (Learning iteration)
            Serial.print(" spos1 = ");Serial.print(spos1);
            Serial.print(" spos2 = ");Serial.print(spos2);
            Serial.print(" spos3 = ");Serial.print(spos3);
            Serial.print(" spos4 = ");Serial.print(spos4);
            Serial.print(" distance = ");Serial.println(distDifference);

             if (distCurrent > distanceHigh) { 
     // if current distancee is greater than highest then replace postions
               spos1High = spos1; 				// assign new servo position 1
               spos2High = spos2; 				// assign new servo position 2
               spos3High = spos3; 				// assign new servo position 3
               spos4High = spos4; 				// assign new servo position 4
               distanceHigh = distCurrent;		// assign the new highest distance
             } // end if
         
	}  // end each episode
  
} // end doTraining

void doLearnedBehavior() {
// routine to do the learned behavior (using the best postions that will achieve its task) 
     Serial.println("Do Learned behavior... ");
     myServo(servo1,0,1,8,1);					// re-init the servo
     myServo(servo2,0,1,8,1);					// re-init the servo
     delay(2000);								// wait
     
     while(1) {  
       
 	// using the best postion to crawl
       myServo(servo1, spos1High, 1, 7, 1);			 
       myServo(servo2, spos2High, 1, 7, 1);
       myServo(servo1, spos3High, 1, 7, 1);
       myServo(servo2, spos4High, 1, 7, 1);
    
  } // doLearned
  
} // end loop

void loop(){  
  // main loop reads success table and performs actions
  // Later on will implement an Andriod app to interface with the robot
  
   doTraining();     	// trial and error training with reinforcement Learning
   
   doLearnedBehavior(); // making the robot crawl
  
  
} // end main loop
