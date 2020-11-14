# DualSense / PS5 Controller on Windows [API]
Windows API for the PS5 DualSense controller.  Written in C++ for C++

First Release coming soon!

#### Road map to first preview release:

- ~~Reading the input state from the controller using USB~~. DONE!
- Writing Output state to the controller using USB. *Work In Progress*
- Documenting the API
- Documenting the raw protocol 

First preview should be released before 28.11.2020 (The hard work is done - The USB protocol is, at least partially, reverse engineered)

#### Features of the first preview

- Reading **all** buttons and analog values (triggers and sticks) which are on the controller
- Reading the two finger touch positions
- Reading the accelerometer
- Reading the  gyroscope (Currently only raw / uncalibrated values) 
- Reading the connection state of the headphone jack
- Setting the rumble motors speed
- Setting various effects to the force feedback triggers
- controlling the RGB-Leds, Microphone Led and User Leds 

#### Feature planed for the future

- Calibrating the gyroscope 
- Writing / Reading over bluetooth (If you know something about the wireless protocol - especial writing data to the controller - please let me know by creating an issue!) 