// this sketch permit users to send AT command via Serial USB.
//please install modified linraries because they don't use Software Serial
//Tested with a DFRobot7000e Shiel and a Arduino Nano 33 BLE 

#include <DFRobot_SIM7000.h>

#define PIN_TX 8
#define PIN_RX 7
UART mySerial(PIN_RX, PIN_TX, NC, NC);
DFRobot_SIM7000 sim7000(&mySerial);

void setup() {
  Serial.begin(19200);
  mySerial.begin(19200);
  Serial.println("Turn ON SIM7000......");
  if (sim7000.turnON()) {  //Turn ON SIM7000
    Serial.println("Turn ON !");
  }

  Serial.println("Set baud rate......");
  while (1) {
    if (sim7000.setBaudRate(19200)) {  //Set SIM7000 baud rate from 115200 to 19200 reduce the baud rate to avoid distortion
      Serial.println("Set baud rate:19200");
      break;
    } else {
      Serial.println("Faile to set baud rate");
      delay(1000);
    }
  }
  Serial.println("Check SIM card......");
  if (sim7000.checkSIMStatus()) {  //Check SIM card
    Serial.println("SIM card READY");
  } else {
    Serial.println("SIM card ERROR, Check if you have insert SIM card and restart SIM7000");
    while (1)
      ;
  }
  
  Serial.println("For example, if you type AT\\r\\n, OK\\r\\n will be responsed!");
  Serial.println("Enter your AT command :");
}

char inputBuffer[64];  // Buffer to store input from the Serial Monitor
int inputIndex = 0;    // Index to keep track of the number of characters in the buffer

void loop() {
  // Read input from Serial Monitor and send it as AT command to SIM7000
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputBuffer[inputIndex] = '\0';  // Null-terminate the input buffer
      mySerial.println(inputBuffer);   // Send the command to SIM7000
      inputIndex = 0;                  // Reset the input buffer index
    } else {
      if (inputIndex < 63) {
        inputBuffer[inputIndex] = c;  // Store the character in the input buffer
        inputIndex++;
      }
    }
  }

  // Read and print response from SIM7000
  while (mySerial.available()) {
    char responseChar = mySerial.read();
    Serial.write(responseChar);
  }
}