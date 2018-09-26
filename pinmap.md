# Sump pit node - Photon

## Pin mappings

"-" means input

"+" means output

```
D0 - SDA
D1 - SCL
D2 - NC / lidar tx 1
D3 - NC / lidar tx 2
D4 - trig 1
D5 - trig 2
D6 - Interrupt, Dist echo 1 / lidar rx 1
D7 - Interrupt, Dist echo 2 / lidar rx 2

A0 - voltage 1
A1 - voltage 2
A3 - rpm 1
A4 - rpm 2
A5 - 
A6 -
A7 -

MCP23017 - i2c (SDA,SCL)
0 - button 1
1 - button 2
2 - button 3
3 - leak 1
4 - leak 2
6 -
7 -
8 + led 1
9 + led 2
10+ led 3
11+ pump relay 1
11+ pump relay 2
12+ siren
13+ buzzer?

Required:
2x leak: 1 digital
2x voltage: 1x analog
2x rpm: 1x digital
2x distance: ultrasonic:trig/echo (interrupt); lidar:rx(local pin)/tx (interrupt)
3x button: digital

1x out siren
2x out pump 
3x out led
1x i2c display
```