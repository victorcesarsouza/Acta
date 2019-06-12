# Gunther Project (AMR)

This project aimed at the development of an autonomous trajectory planner robot, using [laser sensor](http://www.slamtec.com/en/lidar/a1) and [encoders](https://learn.parallax.com/tutorials/robot/arlo/arlo-robot-assembly-guide/section-1-motor-mount-and-wheel-kit-assembly/step-7) for mapping and location and a base of a [differential robot](https://www.parallax.com/product/28966).

## Electrical Mounting

### Arlo

For the electric assembly, the robot was used at [Arlo Power Distribution Board](https://learn.parallax.com/tutorials/robot/arlo/arlo-robot-assembly-guide/section-4-arlo-power-distribution-board) and two 12V / 7A batteries.

### Sabertooth

For the initial tests of the electrical and mechanical part of the robot was used the teleoperated mode of sabertooth by radio control (page 13 and 14  on [Sabertotth2x60 manual](https://www.dimensionengineering.com/datasheets/Sabertooth2x60.pdf), only Switch 6 Down).



For how to configure the Sabertooth, see the DIP switch wizard for. Be sure to select simplified serial mode for use with this library. This sample uses a baud rate of 9600 (page 16 on [Sabertotth2x60 manual](https://www.dimensionengineering.com/datasheets/Sabertooth2x60.pdf)).

### Arduino


Arduino TX->1  ->  Sabertooth S1

Arduino GND    ->  Sabertooth 0V

Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)

Arduino PIN 3  ->  Encoder white cable LA 

Arduino PIN 4  ->  Encoder white cable LB

Arduino PIN 5  ->  Encoder white cable RA

Arduino PIN 6  ->  Encoder white cable RB

[Ecoder white Cables](https://learn.parallax.com/tutorials/robot/arlo/arlo-robot-assembly-guide/section-1-motor-mount-and-wheel-kit-assembly/step-6)

[Encoder Code Arduino](https://github.com/victorcesarsouza/git_Acta/blob/master/ControleArduino/Arduino_SaberTooth_Encoder/Arduino_SaberTooth_Encoder.ino)


## Integration
