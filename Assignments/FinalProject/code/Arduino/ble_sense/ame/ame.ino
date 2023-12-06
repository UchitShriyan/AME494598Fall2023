#include <TensorFlowLite.h>
#include <Arduino_LSM9DS1.h>
#include <Wire.h>

#include "constants.h"
#include "main_functions.h"
#include "model.h"
#include "output_handler.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 4000;
// Keep aligned to 16 bytes for CMSIS
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

float accX = 0, accY = 0, accZ = 0;
float gyroX = 0, gyroY = 0, gyroZ = 0;

// Define the mean and standard deviation for each axis
const float mean[] = {-0.01913587, -0.04640962, 0.60258288, 0.62590743, -0.59912757, 0.39035713};
const float std_dev[] = {0.54834297, 0.48826721, 0.25168263, 10.48656622, 11.68220899, 6.19663301};

}  // namespace


void setup() {
  Wire.begin(); 

  Serial.begin(115200);

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  tflite::InitializeTarget();

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(gesture_recognition_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf(
        "Model provided is schema version %d not equal "
        "to supported version %d.",
        model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    MicroPrintf("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;
}

void loop() {
  // Read from accelerometer and gyroscope
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      IMU.readAcceleration(accX, accY, accZ);
      IMU.readGyroscope(gyroX, gyroY, gyroZ);

      // Normalize the sensor data
      float normalized_accX = (accX - mean[0]) / std_dev[0];
      float normalized_accY = (accY - mean[1]) / std_dev[1];
      float normalized_accZ = (accZ - mean[2]) / std_dev[2];
      float normalized_gyroX = (gyroX - mean[3]) / std_dev[3];
      float normalized_gyroY = (gyroY - mean[4]) / std_dev[4];
      float normalized_gyroZ = (gyroZ - mean[5]) / std_dev[5];


      // Now, 'normalized_accX', 'normalized_accY', etc., hold the normalized values
    

      Serial.print("Raw Acc: ");
      Serial.print(accX); Serial.print(", ");
      Serial.print(accY); Serial.print(", ");
      Serial.print(accZ); Serial.println();

      Serial.print("Normalized: ");
      Serial.print(normalized_accX); Serial.print(", ");
      Serial.print(normalized_accY); Serial.print(", ");
      Serial.print(normalized_accZ); Serial.println();

      Serial.print("Raw Gyro: ");
      Serial.print(gyroX); Serial.print(", ");
      Serial.print(gyroY); Serial.print(", ");
      Serial.print(gyroZ); Serial.println();

      Serial.print("Normalized: ");
      Serial.print(normalized_gyroX); Serial.print(", ");
      Serial.print(normalized_gyroY); Serial.print(", ");
      Serial.print(normalized_gyroZ); Serial.println();


    // Copy data to the model's input tensor
    input->data.f[0] = normalized_accX;
    input->data.f[1] = normalized_accY;
    input->data.f[2] = normalized_accZ;
    input->data.f[3] = normalized_gyroX;
    input->data.f[4] = normalized_gyroY;
    input->data.f[5] = normalized_gyroZ;

    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      Serial.print("Invoke failed: ");
      Serial.println(invoke_status);
      return;
    }

    for (int i = 0; i < 9; i++) {
      Serial.print(output->data.f[i]); Serial.print(", ");
    }
    Serial.println();

    // // Obtain and handle the output from the model
    // float gesture_index = output->data.f[0];
    // HandleOutput(gesture_index);
    // }

    // Find the index of the maximum value in the output array
    int maxIndex = 0;
    float maxValue = output->data.f[0];
    for (int i = 1; i < 9; i++) {
      if (output->data.f[i] > maxValue) {
        maxValue = output->data.f[i];
        maxIndex = i;
      }
    }
    
    // Now maxIndex holds the index of the predicted class
    HandleOutput(maxIndex);
    }
    // Delay between readings
    delay(2000);
}

