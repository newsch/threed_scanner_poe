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
const int servo_min[2] = { 30, 85 };
const int servo_max[2] = { 70, 100 };
Servo x_servo;
Servo y_servo;

long previous_ms = 0;
long current_ms = 0;
int cur_ms_delay = 0;
const int x_ms_delay = 100;
const int y_ms_delay = 400;

// int i = 0;

void sendReadout()
{
  Serial.print(x_servo.read());
  Serial.print(" ");
  Serial.print(y_servo.read());
  Serial.print(" ");
  Serial.println(analogRead(ir_sensor_pin));
  // Serial.println(i);
  // i++;
}

void sendConfig()
{
  // Send x min, y min, x max, y max
  Serial.print(servo_min[0]);
  Serial.print(" ");
  Serial.print(servo_min[1]);
  Serial.print(" ");
  Serial.print(servo_max[0]);
  Serial.print(" ");
  Serial.print(servo_max[1]);
  Serial.println();
}

void purgeReceiveBuffer()
{
  while(Serial.available()){
    Serial.read();
  }
}

void setup()
{
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
      // If time has passed
      if(current_ms > previous_ms + cur_ms_delay){
        previous_ms = current_ms;

        // Read sensor and send data
        sendReadout();

        // Increment x servo
        servo_pos[0] += 1;
        x_servo.write(servo_pos[0]);
        cur_ms_delay = x_ms_delay;

        // If x_servo is on bounds (This row has completed)
        if (servo_pos[0] > servo_max[0]){
          // Reset x servo and increment y servo
          servo_pos[0] = servo_min[0];
          servo_pos[1] += 1;

          x_servo.write(servo_pos[0]);
          y_servo.write(servo_pos[1]);

          // Set next delay to longer for x_servo to have time to reset to home
          cur_ms_delay = y_ms_delay;

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
      y_servo.write(90);

      x_servo.detach();
      y_servo.detach();

      cur_state = STOPPED;
      Serial.println("*** Stopped ***");

      purgeReceiveBuffer();
      break;

    case START:
      Serial.println("*** Starting ***");

      x_servo.attach(x_servo_pin);
      y_servo.attach(y_servo_pin);

      sendConfig();

      // Init scanning
      // Set servo_pos to servo_min
      memcpy(servo_pos, servo_min, sizeof servo_min);

      // Move servos to starting position
      x_servo.write(servo_pos[0]);
      y_servo.write(servo_pos[1]);

      // set delay before first readout
      cur_ms_delay = 500;

      cur_state = RUNNING;
      break;
    }
}
