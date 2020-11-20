# DualSense / PS5 Controller on Windows [API]
Windows API for the PS5 DualSense controller.  Written in C++ for C++

First Release coming soon!

#### Road map to first preview release:

- ~~Reading the input state from the controller using USB~~. DONE!
- ~~Reading input via bluetooth~~. DONE!
- ~~Writing Output state to the controller using USB~~. DONE!
- ~~Controlling and reading the adaptive triggers.~~ DONE!
- Writing basic Output state via bluetooth. *Work in progress*
- Addition input / output parameters. *Work in progress*
- Documenting the API *Work in progress*
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
- Retrieving force feedback triggers feedback
- controlling the RGB-Leds, Microphone Led and User Leds 

Fully working over USB.  No output via Bluetooth (Maybe coming soon)

#### Feature planed for the future

- Calibrating the gyroscope 
- Using the haptic feedback
- More trigger effects?
- Fully write over bluetooth (If you know something about the wireless protocol - especial writing data to the controller - please let me know by creating an issue!) 

#### Sources

I have partially used the following sources to implement the functionality:

- The GitHub community on this project and https://github.com/Ryochan7/DS4Windows/issues/1545
- https://gist.github.com/dogtopus/894da226d73afb3bdd195df41b3a26aa
- https://gist.github.com/Ryochan7/ef8fabae34c0d8b30e2ab057f3e6e039
- https://gist.github.com/Ryochan7/91a9759deb5dff3096fc5afd50ba19e2
- https://github.com/Ryochan7/DS4Windows/tree/dualsense-integration (Copyright (c) 2019 Travis Nickles - [MIT License](https://github.com/Ryochan7/DS4Windows/blob/jay/LICENSE.txt))
- https://www.reddit.com/r/gamedev/comments/jumvi5/dualsense_haptics_leds_and_more_hid_output_report/

