#define length 5
volatile int index = 0; 
float data_points[length];
float average_speed;
long error_Integral = 0;
long error_Derivative = 0;
const int analogOutPin = 3; // Analog output pin that the motor is attached to
int PWMout = 0;             // value output to the PWM (analog out)
const byte interruptPin = 2;
volatile long count=0;
volatile unsigned int Speed;
int timer1_counter;  
unsigned long current_time;
unsigned long previous_time;
long previous_error;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  timer1_counter = 59286;   // preload timer 65536-16MHz/256/10Hz (time interval of 100ms)
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts    
}

void loop() {
  //PWMout = 255;
  //Open-loop DC motor control 
  //analogWrite(analogOutPin, PWMout);
  //Serial.print("Speed: ");
  //Serial.print(Speed);
  //Serial.print(", ");

  // Moving Average Filter
  long total_speed = 0;
  data_points[index] = Speed;
  index = (index + 1) % length;     // The index is incremented and once the index exceeds the array length, it resets the index to 0 and updates index = 0 with the latest value stored in the array 
  for (int i = 0; i < length; i++){
    total_speed += data_points[i];    // Tallies up the total speed from the latest 5 measured speed values
  }
  average_speed = total_speed/length; // Calculates the average speed (moving average filter)
  //Serial.print("Average Speed: ");
  //Serial.println(average_speed);
  
  // Closed-loop DC motor speed control
  int Speed_Reference = 1400;
  int error;
  float Kp = 0.1;
  float Ki = 0.006;
  float Kd = 0.0;

  current_time = millis();
  long time_difference = current_time - previous_time;            // Elapsed time between the controllers

  error = Speed_Reference - average_speed;                        // Calculating the error between the target speed and the measured speed
  error_Integral += error;                                        // Total sum of errors accumulated (error_Integral * Ki so as to drive the accumulative error to 0)
  error_Derivative = (error - previous_error)/time_difference;    // Handles the rate of change of error

  PWMout = (Kp * error) + (Ki * error_Integral) + (Kd * error_Derivative);  // PWM output
  PWMout = constrain(PWMout, 0, 255);       
 
  previous_error = error;                                         // Updates the error, storing the error for the next iteration
  previous_time = current_time;                                   // Updates the time, storing the time for the next iteration

  // Update the analog out value (PWM)
  analogWrite(analogOutPin, PWMout);                              //  Writing the output PWM to the motor

  // print the results to the serial monitor:
  //Serial.print("\t speed = ");   Serial.print(Speed);
  Serial.print("\t average speed = ");   Serial.print(average_speed);
  Serial.print("\t Speed_Reference = ");   Serial.print(Speed_Reference);
  Serial.print("\t PWMout = ");  Serial.println(PWMout);

  // wait 2 milliseconds before the next loop
  delay(2);
}

void blink() {
  count=count+1;
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
  Speed=((count*10)/11.0)*60; //`rpm at High revolution side *10 because of 10Hz, /11 magnets
  count=0;
}
