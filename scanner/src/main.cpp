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

const int x_servo_pin = 5;
const int y_servo_pin = 6;
const int ir_sensor_pin = A0;

int servo_pos[2] = { 0, 0 };
int servo_dir = 1; // 1 or -1 for right or left
const int servo_min[2] = { 30, 85 };
// const int servo_max[2] = { 10, 80 };
const int servo_max[2] = { 70, 100 };
Servo x_servo;
Servo y_servo;

long previous_ms = 0;
long current_ms = 0;
const int ms_delay = 100;

int i = 0;


void setup()
{
  x_servo.attach(x_servo_pin);
  y_servo.attach(y_servo_pin);

  Serial.begin(100000);
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
        i++;
        previous_ms = current_ms;

        // Move servos
        servo_pos[0] += servo_dir; // Increment x_servo
        x_servo.write(servo_pos[0]);

        // Read sensor and send data
        Serial.print(x_servo.read());
        Serial.print(" ");
        Serial.print(y_servo.read());
        Serial.print(" ");
        Serial.println(analogRead(ir_sensor_pin));

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
          i++;

          delay(500);

          // Read sensor and send data
          Serial.print(x_servo.read());
          Serial.print(" ");
          Serial.print(y_servo.read());
          Serial.print(" ");
          // Serial.println(i);
          Serial.println(analogRead(ir_sensor_pin));
        }
      }

      break;

    case STOP:
      // Reset the scanner
      Serial.println("Stopping...");
      x_servo.write(90);
      y_servo.write(90);
      cur_state = STOPPED;
      Serial.println("*** Stopped ***");
      break;

    case START:
      Serial.println("*** Starting ***");
      Serial.print(servo_min[0]);
      Serial.print(" ");
      Serial.print(servo_min[1]);
      Serial.print(" ");
      Serial.print(servo_max[0]);
      Serial.print(" ");
      Serial.print(servo_max[1]);
      Serial.println();

      // Init scanning
      memcpy(servo_pos, servo_min, sizeof servo_min);
      servo_dir = 1;
      i = 0;

      x_servo.write(servo_pos[0]);
      y_servo.write(servo_pos[1]);

      // wait for servos to move to start
      delay(500);

      cur_state = RUNNING;
      break;
    }
}
