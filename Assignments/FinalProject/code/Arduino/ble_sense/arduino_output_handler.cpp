#include "Arduino.h"
#include "constants.h"
#include "output_handler.h"
#include "tensorflow/lite/micro/micro_log.h"
#include <Wire.h>


void HandleOutput(float gesture_index) {
    // Convert float index to integer
    int gesture = static_cast<int>(gesture_index);

    static int prev_gesture = -1; // This should be static to maintain its value between calls
    int curr_gesture = gesture;

    if (curr_gesture != prev_gesture){
        Wire.beginTransmission(9); // Begin transmission to device at address 9

        // Split the integer into two bytes and send each byte separately
        Serial.println(curr_gesture & 0xFF);
        Wire.write(curr_gesture & 0xFF); // Send lower byte
        Serial.println((curr_gesture >> 8) & 0xFF);
        Wire.write((curr_gesture >> 8) & 0xFF); // Send upper byte

        Wire.endTransmission(); // End transmission

        prev_gesture = curr_gesture; // Update the previous gesture
    }

    // Map the index to the corresponding gesture name
    switch (gesture) {
      case 0:
        Serial.println("Predicted Gesture: backward");
        break;
      case 1:
        Serial.println("Predicted Gesture: backward_left");
        break;
      case 2:
        Serial.println("Predicted Gesture: backward_right");
        break;
      case 3:
        Serial.println("Predicted Gesture: diagonal_forward_left");
        break;
      case 4:
        Serial.println("Predicted Gesture: diagonal_forward_right");
        break;
      case 5:
        Serial.println("Predicted Gesture: flat");
        break;
      case 6:
        Serial.println("Predicted Gesture: forward");
        break;
      case 7:
        Serial.println("Predicted Gesture: left");
        break;
      case 8:
        Serial.println("Predicted Gesture: right");
        break;
      default:
        Serial.println("Unknown Gesture");
    }
}
