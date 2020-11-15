# DualSense / PS5 Controller on Windows [API]
Windows API for the PS5 DualSense controller.  Written in C++ for C++

First Release coming soon!

#### Road map to first preview release:

- ~~Reading the input state from the controller using USB~~. DONE!
- ~~Reading basic input via bluetooth~~. DONE!
- ~~Writing Output state to the controller using USB~~. DONE!
- Writing basic Output state via bluetooth. *Work in progress*
- Controlling and reading the adaptive triggers. *Work in progress*
- Documenting the API
- Documenting the raw protocol 
- Updating the github repo and publish release

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

- Reading the battery status
- Calibrating the gyroscope 
- Using the haptic feedback
- Fully write and read over bluetooth (If you know something about the wireless protocol - especial writing data to the controller - please let me know by creating an issue!) 

#### Sources

I have partially used the following sources to implement the functionality:

- The GitHub community on this project
- https://gist.github.com/dogtopus/894da226d73afb3bdd195df41b3a26aa
- https://github.com/Ryochan7/DS4Windows/tree/dualsense-integration (Copyright (c) 2019 Travis Nickles - [MIT License](https://github.com/Ryochan7/DS4Windows/blob/jay/LICENSE.txt))

