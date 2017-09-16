/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include "Servo.h"

// Variables to control state
#define STOPPED 0
#define RUNNING 1
#define STOP 2
#define START 3

int cur_state = STOP;

const int x_servo_pin = 9;
const int y_servo_pin = 10;
const int ir_sensor_pin = A0;

int servo_pos[2] = { 0, 0 };
int servo_dir = 1; // 1 or -1 for right or left
const int servo_min[2] = { 45, 45 };
const int servo_max[2] = { 135, 135 };
Servo x_servo;
Servo y_servo;

long previous_ms = 0;
long current_ms = 0;
const int ms_delay = 100;


void setup()
{
  x_servo.attach(x_servo_pin);
  y_servo.attach(y_servo_pin);
  Serial.begin(9600);
  Serial.println("Begin");
}

void loop()
{
  switch (cur_state) {
    case STOPPED:
      if(Serial.read() == 's'){
        cur_state = START;
        break;
      }
      break;

    case RUNNING:
      current_ms = millis();
      if(current_ms > previous_ms + ms_delay){
        previous_ms = current_ms;

        // Read sensor and send data
        Serial.print(x_servo.read() + "\t");
        Serial.print(y_servo.read() + "\t");
        Serial.println(analogRead(ir_sensor_pin));

        // Move servos
        servo_pos[0] += servo_dir; // Increment x_servo
        x_servo.write(servo_pos[0]);

        // If x_servo is on bounds increment y_servo
        if (servo_pos[0] >= servo_max[0] || servo_pos[0] <= servo_min[0]){
          servo_dir = servo_dir * -1; // Flip direction

          servo_pos[1] +=1;
          y_servo.write(servo_pos[1]);

          // If y_servo is on bounds, scanning is complete
          if (servo_pos[1] > servo_max[1]){
            Serial.println("Complete!");
            cur_state = STOP;
            break;
          }
        }
      }

      break;

    case STOP:
      // Reset the scanner
      Serial.println("Stopping...");
      x_servo.write(90);
      cur_state = STOPPED;
      Serial.println("Stopped");
      break;

    case START:
      // Init scanning
      servo_pos = servo_min;
      x_servo.write(servo_pos[0]);
      y_servo.write(servo_pos[1]);
      
      // wait for servos to move to start
      sleep(500);

      cur_state = RUNNING;
      break;
    }
}
