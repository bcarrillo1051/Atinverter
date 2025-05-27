/* 
  Modified: 4/22/2025
  Modified By: Zach Kwast
  Based on code created by Elijah Gordon and Joshua Hutchinson
  Main program for Atverter DC DC converter
  
*/

#include <AtverterE.h>

#define INTERRUPT_TIME 1

#define INPUT_VOLTAGE_JUMP 999
#define OUTPUT_BOOST_VOLTAGE_STEADY_STATE 499
#define OUTPUT_BUCK_VOLTAGE_STEADY_STATE 499
#define NUM_SAMPLES 10


AtverterE atverterE;

int ledState = HIGH;

double dutyCycle = 512;
double lowVoltage;
double highVoltage;

int32_t prevVoltageError = 0;

char message[20];
int messagePosition = 0;
String state;
double setPoint = 5000;
bool PWM_on = true;

void setup(void) {

  atverterE.setupPinMode();        //Get pins setup
  atverterE.initializePWMTimer();  //Setup Timers

  atverterE.initializeInterruptTimer(INTERRUPT_TIME, &controlUpdate);  //Get interrupts enabled
  Serial.begin(9600);

  atverterE.setDutyCycle(dutyCycle);
  atverterE.startPWM();

}


void loop(void) {
  serialControl();
}


void controlUpdate(void) {

  if (state == "idle") {
    if (PWM_on == true) {
      atverterE.stopPWM();
      PWM_on = false;
    }
  }
  if (state == "boost") {
    if (PWM_on == false) {
      atverterE.startPWM();
      PWM_on = true;
    }
    boostControl(setPoint);
  }
  if (state == "buck") {
    if (PWM_on == false) {
      atverterE.startPWM();
      PWM_on = true;
    }
    buckControl(setPoint);
  }
  if (state == "current") {
    if (PWM_on == false) {
      atverterE.startPWM();
      PWM_on = true;
    }
    currentControl(setPoint);
  }
}


void buckControl(double lowVoltage) {

  // --- Moving Average Filter Setup ---
  static double lowVoltageSamples[NUM_SAMPLES];
  static int sampleIndex = 0;
  static double lowVoltageSum = 0;
  static bool bufferFilled = false;
  static int numSamplesCollected = 0;

  // Raw voltage measurement
  double rawLV = ((double)atverterE.getActualVL() * 0.92) + 104;

  // Remove oldest sample from sum, add new sample
  lowVoltageSum -= lowVoltageSamples[sampleIndex];
  lowVoltageSamples[sampleIndex] = rawLV;
  lowVoltageSum += lowVoltageSamples[sampleIndex];

  sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;
  if (numSamplesCollected < NUM_SAMPLES) {
    numSamplesCollected++;
  }

  // Use average once buffer is filled
  double actualLowVoltage;
  if (numSamplesCollected >= NUM_SAMPLES) {
    actualLowVoltage = lowVoltageSum / NUM_SAMPLES;
  } else {
    actualLowVoltage = rawLV;  // Fallback for first few iterations
  }

  if (lowVoltage < 1000) {
    lowVoltage = 1000;
  }

  // --- PID Control ---
  const double kp = 0.03;
  const double ki = 0.001;
  const double kd = 0.0;

  static double integralControl = 0.0;

  double voltageError = actualLowVoltage - lowVoltage;  // Instantaneous error of the desired output versus actual output voltage

  if (abs(voltageError) > OUTPUT_BUCK_VOLTAGE_STEADY_STATE) {
    
    // Proportional term
    double proportionalControl = -(kp * (voltageError));

    // Integral term
    integralControl += -(ki * (voltageError));

    // Clamp integral to prevent wind-up
    integralControl = constrain(integralControl, -50, 50);

    // Total PID adjustment
    dutyCycle += (proportionalControl+ integralControl);
  }

  // Constrain and apply duty cycle
  dutyCycle = constrain(dutyCycle, 100, 900);
  atverterE.setDutyCycle(dutyCycle);
}


void boostControl(double highVoltage) {

  // --- Moving Average Filter Setup ---
  static double highVoltageSamples[NUM_SAMPLES];
  static int sampleIndex = 0;
  static double highVoltageSum = 0;
  static bool bufferFilled = false;
  static int numSamplesCollected = 0;

  // Raw voltage measurement
  double rawVH = ((double)atverterE.getActualVH() * 0.92) - 92;

  // Remove oldest sample from sum, add new sample
  highVoltageSum -= highVoltageSamples[sampleIndex];
  highVoltageSamples[sampleIndex] = rawVH;
  highVoltageSum += highVoltageSamples[sampleIndex];

  sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;
  if (numSamplesCollected < NUM_SAMPLES) {
    numSamplesCollected++;
  }

  // Use average once buffer is filled
  double actualHighVoltage;
  if (numSamplesCollected >= NUM_SAMPLES) {
    actualHighVoltage = highVoltageSum / NUM_SAMPLES;
  } else {
    actualHighVoltage = rawVH;  // Fallback for first few iterations
  }

  // --- PID Control ---
  if (highVoltage < 1000) {
    highVoltage = 1000;
  }

  const double kp = 0.035;
  const double ki = 0.001;
  const double kd = 0.0;

  static double integralControl = 0.0;
  int32_t voltageError = highVoltage - actualHighVoltage;

  if (abs(voltageError) > OUTPUT_BOOST_VOLTAGE_STEADY_STATE) {

    // Proportional term
    double proportionalControl = -(kp * voltageError);

    // Integral term
    integralControl += -(ki * voltageError);

    // Clamp integral to prevent wind-up
    integralControl = constrain(integralControl, -50, 50);

    // double derivativeControl = -(kd * ((double)(voltageError - prevVoltageError) / INTERRUPT_TIME));
    // prevVoltageError = voltageError;

    // Total PID adjustment
    dutyCycle += (proportionalControl + integralControl);
  }

  // Constrain and apply duty cycle
  dutyCycle = constrain(dutyCycle, 100, 900);
  atverterE.setDutyCycle(dutyCycle);
}

void currentControl(double lowCurrent) {

  // --- Moving Average Filter Setup ---
  static double currentSamples[NUM_SAMPLES] = {0};
  static int sampleIndex = 0;
  static double currentSum = 0;
  static int numSamplesCollected = 0;

  // Raw current measurement
  double rawI = (((double)(-atverterE.getIL()) * 0.9) + 10);

  // Remove oldest sample from sum, add new sample
  currentSum -= currentSamples[sampleIndex];
  currentSamples[sampleIndex] = rawI;
  currentSum += rawI;

  sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;
  if (numSamplesCollected < NUM_SAMPLES) {
    numSamplesCollected++;
  }

  // Use average once buffer is filled
  double actualLowCurrent;
  if (numSamplesCollected >= NUM_SAMPLES) {
    actualLowCurrent = currentSum / NUM_SAMPLES;
  } else {
    actualLowCurrent = rawI;  // Fallback for first few iterations
  }

  // --- PID Control ---
  const double kp = 0.03;    // Proportional Control
  const double ki = 0.0003;   // Integral Control
  const double kd = 0.001;    // Derivative Control

  static double integralControl = 0.0;
  static double prevCurrentError = 0.0;

  int32_t currentError = actualLowCurrent - lowCurrent;

  // Proportional term
  double proportionalControl = -(kp * ((double)currentError));

  // Integral term
  integralControl += -(ki * ((double)currentError));
  integralControl = constrain(integralControl, -50, 50);  // Anti-windup

  // Derivative term
  double derivativeControl = -(kd * ((double)(currentError - prevCurrentError) / (double)INTERRUPT_TIME));
  prevCurrentError = currentError;

  // Total PID adjustment
  dutyCycle += (proportionalControl + integralControl + derivativeControl);

  // Constrain and apply duty cycle
  dutyCycle = constrain(dutyCycle, 100, 900);
  atverterE.setDutyCycle(dutyCycle);
}


void serialControl(void) {


  String messageSTR;


  while (Serial.available() > 0) {
    // read the incoming byte:
    char serialIn = Serial.read();
    if ((serialIn != '\n') && (messagePosition < 7)) {
      message[messagePosition] = serialIn;
      messagePosition++;
    } else {
      message[messagePosition] = '\0';
      messagePosition = 0;
      messageSTR = String(message);
    }
  }

  if ((messageSTR == "Request")) {
    Serial.print("VL ");
    Serial.println(((double)atverterE.getActualVL() * 0.92) + 104);

    Serial.print("VH ");
    Serial.println(((double)atverterE.getActualVH() * 0.92) - 92);

    Serial.print("PWM ");
    Serial.println(atverterE.getDutyCycle());

    Serial.print("STATE ");
    Serial.println(state);

    Serial.print("SETPOINT ");
    Serial.println(setPoint);

    Serial.print("lvCurrent ");
    Serial.println(((double)(-atverterE.getIL()) * 0.9) + 10);

    Serial.print("hvCurrent ");
    Serial.println(((double)(atverterE.getIH()) * 0.95) + 30);

    messageSTR = "done";
  }

  else if (messageSTR.substring(0, 7) == "tooIdle") {
    state = "idle";
    Serial.println(messageSTR);
    dutyCycle = 512;
  } 
  else if (messageSTR.substring(0, 7) == "toBoost") {
    state = "boost";
    Serial.println(messageSTR);
    setPoint = 15000;
    dutyCycle = 1000;
  } 
  else if (messageSTR.substring(0, 7) == "tooBuck") {
    state = "buck";
    Serial.println(messageSTR);
    setPoint = 5000;  //Default value
    dutyCycle = 200;
  } 
  else if (isAllDigits(messageSTR)) {
    setPoint = messageSTR.toDouble();
    Serial.print(messageSTR);
    Serial.println("mV");
  }

  else if (messageSTR.substring(0, 7) == "current") {
    state = "current";
    Serial.println(messageSTR);
    setPoint = 200;
  } 
  else {
    Serial.flush();
  }
}


boolean isAllDigits(String str) {
  boolean allDigits = true;

  if (str.length() == 0) {
    allDigits = false;
  }

  for (int i = 0; i < str.length(); i++) {
    if (!isDigit(str.charAt(i))) {
      allDigits = false;
    }
  }
  return allDigits;
}
