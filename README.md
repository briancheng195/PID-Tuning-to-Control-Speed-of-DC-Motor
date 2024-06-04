# PID-Tuning-to-Control-Speed-of-DC-Motor
Implementation of PID controller to tune the speed of FIT0522 DC motor to a set target speed of 1400rpm

# Specifications of FIT0522 DC Motor
- Motor Rated Voltage: 6V
- Encoder Rated Voltage: 3.3 / 5V
- Reducer Reduction Ratio: 1:75
- No load Speed: 100RPM@0.13A
- Maximum Efficiency Point: Load 1.0kgcm/80RPM/1.7W/0.5A
- Maximum Power point: Load 3.0kgcm/55RPM/2.8W/1.0A
- Stall Torque: 6.5kgcm
- Stall Current: 3.0A
- Hall Resolution: Hall Resolution 11x Precision Reduction Ratio 74.83=823.1PPR Dimension: 54 * Φ24.4mm / 2.13 * Φ0.96inches
- Weight: 96g

# Speed Sensing of Motor 
- The speed sensing of the motor can be conducted as the motor’s rotational speed can be measured with the aid of its incremental encoder
- The incremental encoder provides pulse signals as the motor’s shaft rotates
- For providing speed feedback, one of the encoder pins (Encoder A phase) of the motor is connected to the interrupt pin of the Arduino microcontroller. This establishes the 
  connection with one of the hall effect sensors of the motor and its incremental encoder. There is a disk mounted on the shaft of the motor and the disk would 
  spin as the motor shaft rotates
- There are magnets within the disk and as the disk spins, this would cause changes in the magnetic field and it is detected by the hall effect sensor. Due to the change in the   
  magnetic field, the hall effect sensor creates electric signals in the form of a square wave which can be displayed on the Arduino serial monitor
- The incremental encoder would generate pulses as the disk spins and these pulses are accounted for so as to measure the speed of the motor. This is done by multiplying the      
  number of pulses with the number of pulses per revolution made by the shaft at every specific time interval 

# Implementation of External Driver (L293 Motor Driver)
- The motor driver acts as a medium to control the speed of the motor, based on the commands sent from the Arduino microcontroller on reading and writing different PWM values to 
  the motor
- The motor driver helps to control the direction of rotation of the motor
- The motor driver can also amplify the electric signals from the voltage supplied to the motor

# Electronics Design 
<img width="500" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/ce9ed260-e928-4dfa-9b2c-8b4b848513a7">

# Open Loop Speed Control of FIT0522 DC Motor
- Speed Measurement at Steady State of DC Motor ranges from 2563rpm to 2618rpm when PWM of DC Motor is set to a maximum value of 255

<img width="500" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/2ce44095-66a8-4027-933a-ceec8a86d008">

- Speed Measurement at Steady State of DC Motor ranges from 1418rpm to 1472rpm when a step change is applied as PWM of DC Motor is set to a value of 150

<img width="500" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/ad6b15d3-4b1e-49b9-bcf7-7ca762349b22">

- Minimum PWM value of DC Motor is 52 when measured speed of the DC Motor is zero

<img width="500" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/2213387a-e04d-4192-a133-1ea2ee561de4">

# Implementation of Moving Average Filter
- To reduce the noise of the motor, a moving average filter is applied
- For example, the code takes in the latest 5 measured speed values (1st to 5th measured speed values) and calculates the average. Afterwards, once the 6th measured speed value 
  is taken in, the oldest measured speed reading is removed from the array and the latest measured speed reading is added into the array. The average is calculated again and the 
  process repeats so forth. 

# Control Loop Speed Control of FIT0522 DC Motor
- The purpose of implementing a PID controller is to regulate the speed of the motor so that it is able to drive towards its target speed
- For this case, the target speed of the motor is set to 1400rpm

# P-Controller 
# Kp = 0.1 and Ki = 0.0
- When setting the P-controller, the value of Kp was set to 0.1 and the value of Ki is set to 0 (since tuning only the P-controller). With these values, the output speed of the 
  motor was not meeting the set point and there was an offset difference between them as shown in the figure below

  <img width="800" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/7f0a1f85-336b-4aaa-a1d4-920fa02b2bb7">

# Kp = 0.2 and Ki = 0.0
- Subsequently, the value of Kp was increased and it is set to 0.2 and Ki remains at 0. With the increased Kp value, the offset difference between the speed of the motor and the 
  target speed is smaller. However, this led to the system output to oscillate and it also caused the speed of the motor to fluctuate significantly as shown in the figure below

  <img width="800" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/2964c969-f91a-4f77-9ee9-935bab54f999">

# Insights with Implementation of Proportional Controller
- Therefore, selecting the Kp value to be 0.1 is suitable as Kp = 0.1 for tuning of the P-controller did not cause the speed of the motor to oscillate and fluctuate significantly.

# PI-Controller 
- However, it is still needed to implement both the Proportional and Integral controllers as with only the P-controller, the measured speed was not close to the target speed and 
  there is an offset difference between them
- Thus, the Integral controller can help reduce the offset between them and further drive the speed of the motor in reaching closer to its target speed

# Kp = 0.1 and Ki = 0.003
- With those values, it could be shown in the Arduino serial plotter below that the tuning of the PI controller helps to reduce the offset difference between the target speed and 
  measured speed, as it drives the measured speed of the motor to rise and get close to its target speed
- The rise time is the time taken for the system (measured speed) to reach from 10% to 90% of its steady state and from the serial plotter (Figure 18), it is recorded to be 
  approximately 5 seconds. 
- An overshoot is also observed which is caused by the PI controller, as the measured speed shoots above its target value. The percentage overshoot can be measured from the  
  formula, % overshoot = (max. speed value – steady state value)/steady-state value x 100%. The maximum speed value is 1800rpm and the steady state value is 1395 rpm. Thus, the  
  percentage overshoot based on the formula would be 29.0% (3sf)
- The settling time is the time taken for the measured speed to reach its target speed by ±5% and it is recorded to be approximately 37 seconds.
- The steady state error can be measured from the formula, steady state error (%) = (target speed – measured speed=1357)/target speed x 100%, which is 0.357% (3sf)

  <img width="800" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/c5812799-0c04-4aac-b4c8-bac910c59512">

- However, it is observed that with the Ki value = 0.003, the measured speed of the motor continues to fluctuate around the target speed. This could be due to the noise present     in the motor and the range of the measured speed was from 1374 rpm to 1428 rpm as shown in the figure below. Thus, there was still opportunity to further improve the system 
  response, so as to reduce the fluctuations of the motor’s speed.

  <img width="800" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/bd4c2496-ba30-4924-bcc5-1d83fb8c0203">

# Kp = 0.1 and Ki = 0.006
- The Arduino serial plotter show that the measured speed of the motor was significantly close to its target speed at 1400rpm and the range of the motor’s measured speed was from 
  1396rpm to 1406rpm which is much smaller, compared to the ranges of the measured speed when Ki was set to 0.003 and 0.01 respectively.
- The measured speed also did not oscillate and it showed minimal fluctuations. The output PWM value also remained constant as possible, hovering from 146 to 148 (as shown in the 
  figure below). Thus, the result showed that the measured speed of the motor was driven as close to the target speed and the system is more stable with this tuning of the PI  
  controller.
  
<img width="800" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/cd2592b9-f749-41c4-adac-4996d31f0d6f">

# PID-Controller
# Kp = 0.1, Ki = 0.006 and Kd = 0.01
- With the values of Kp, Ki and Kd set, the system output showed that the speed of the motor was hovering close to the target speed, as its range is from 1374 rpm to 1440 rpm.   
  The output PWM was also relatively stable
- However, the speed of the motor and its output PWM does fluctuate more as it has a larger range, as compared to the results from the PI controller with Kp = 0.1 and Ki = 0.006. 

<img width="800" alt="image" src="https://github.com/briancheng195/PID-Tuning-to-Control-Speed-of-DC-Motor/assets/122734373/d3de4c91-fd91-45de-9b60-f22340392be7">

# Insights with Implementation of PI and PID-Controllers
- This could be due to the addition of the derivative controller where the derivative term may have amplified the noise in the error signal, which led to greater fluctuations and   instability of the system output, as compared to the PI controller.
- Based on the results from the tuning of the PI controller (Kp = 0.1 and Ki = 0.006), it can be considered that implementing the additional D-controller may not be necessary.
- This is because from the tuning of the PI controller, the measured speed of the motor is significantly close to the target speed and its measured speed does not oscillate or 
  fluctuate significantly. Furthermore, its range difference is 9, suggesting minimal noise from the motor. It also highlights that the measured speed of the motor is steady and    without any significant overshoot. The system output, PWM value of the motor, is also relatively constant over time in setting the speed of the motor (shown in Figure 23).
- Thus, this shows that the PI controller has minimised the steady state error and caused the speed of the motor to be stable. Therefore, the current tuning of the PI controller    provides a stable system and it may not be critical to further refine it with the PID controller. 
