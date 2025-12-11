# Wireless Snap-On Modular Robot
A wheeled robot with a module slot on the front, allowing module attachments wirelessly through pogo pins. Driven by two motors in the rear and controlled by a joystick mounted on top.

## Why?
I've previously made a robot that is sort of modular, but you need to connect the wires together and use the janky pins to attach them to the base. I wanted to create a new robot with an improved system completely wireless.

## PCB: 
Includes:
- Power management
- Motor driver
- Wireless module interface (pogo pins)
- Microcontroller hub
- Headlights module
  
<img width="849" height="888" alt="Screenshot 2025-12-09 192952" src="https://github.com/user-attachments/assets/c28d4941-5d86-4cc8-929d-230980298c60" />

### Schematic
<img width="1173" height="823" alt="Screenshot 2025-12-05 001956" src="https://github.com/user-attachments/assets/e90a1877-60a6-437c-afeb-dbb1d6c95492" />

## CAD: 
Smooth, round base with a flat front for module additions. Detachable lid for battery and PCB.

<img width="973" height="673" alt="Screenshot 2025-12-09 194858" src="https://github.com/user-attachments/assets/d134b6b6-ffb3-46d9-ab19-65d7b360b4dd" />
<img width="917" height="569" alt="Screenshot 2025-12-09 195018" src="https://github.com/user-attachments/assets/b91aab6c-b5ff-484d-aec8-237d12555ef6" />

<img width="1358" height="858" alt="Screenshot 2025-12-05 215607" src="https://github.com/user-attachments/assets/97b60a4c-1d1e-4048-a93e-0e881f40a305" />

Simple shell for the headlights module to be contained in.

<img width="653" height="496" alt="Screenshot 2025-12-05 215749" src="https://github.com/user-attachments/assets/ff439cfc-0db1-4798-b756-94b3c3b15f9d" />

Simple lightweight wheels with slots for motor shaft.

<img width="626" height="491" alt="Screenshot 2025-12-05 215736" src="https://github.com/user-attachments/assets/606498c3-789a-4460-bdab-78e2d1d56668" />

<br>


## BOM
| Item                      | Type                |   Quantity | Price (Total)   | Vendor     |
|:--------------------------|:--------------------|-----------:|:----------------|:-----------|
| CL21A106KAYNNNE           | 10uF                |         10 | $0.10           | JLCPCB     |
| CL31A226KAHNNNE           | 22uF                |          5 | $0.20           | JLCPCB     |
| CL21B105KBFNNNE           | 1uF                 |          5 | $0.04           | JLCPCB     |
| CL10A225KO8NNNC           | 2.2uF               |         10 | $0.05           | JLCPCB     |
| CL31A107MQHNNNE           | 100uF               |          5 | $0.27           | JLCPCB     |
| CL05B104KO5NNNC           | 100nF               |         20 | $0.02           | JLCPCB     |
| CL21B103KBANNNC           | 10nF                |          5 | $0.03           | JLCPCB     |
| B2B-XH-A(LF)(SN)          | JSTConnector        |         20 | $0.70           | LCSC       |
| XL-302UWC                 | XL-302UWC           |         20 | $0.46           | LCSC       |
| KT-0603R                  | KT-0603R            |          5 | $0.03           | JLCPCB     |
| AO3401A                   | AO3401A             |          5 | $0.28           | JLCPCB     |
| 0603WAF5101T5E            | 5.1kΩ               |         10 | $0.01           | JLCPCB     |
| 0603WAF1201T5E            | 1.2kΩ               |          5 | $0.01           | JLCPCB     |
| 0603WAF3303T5E            | 330kΩ               |          5 | $0.01           | JLCPCB     |
| 1206W4F1003T5E            | 100kΩ               |          5 | $0.02           | JLCPCB     |
| 0805W8F1002T5E            | 10kΩ                |         20 | $0.04           | JLCPCB     |
| 0805W8F1000T5E            | 100Ω                |         10 | $0.02           | JLCPCB     |
| 0805W8F2200T5E            | 220Ω                |         15 | $0.03           | JLCPCB     |
| TS-1088-AR02016           | TS-1088-AR02016     |         15 | $0.64           | JLCPCB     |
| ESP32-S3-WROOM-1-N8R2     | 2.4GHz              |          0 | $4.69           | LCSC       |
| TP4056                    | TP4056              |          0 | $0.42           | LCSC       |
| TPS61202DRCT              | TPS61202DRCT        |          0 | $1.76           | LCSC       |
| APH0420T4R7M              | 4.7uH               |          0 | $0.41           | LCSC       |
| AP2112K-3.3TRG1           | AP2112K-3.3TRG1     |          0 | $0.46           | LCSC       |
| DRV8833PWPR               | DRV8833PWPR         |          0 | $0.67           | LCSC       |
| TYPE-C 16PIN 2MD(073)     | TYPE-C16PIN2MD(073) |         10 | $0.59           | LCSC       |
| 1206L150/16NR             | 1206L150/16NR       |         10 | $0.46           | LCSC       |
| Xinyangze YZ134112062B-02 | Pogo pins           |          5 | $1.43           | LCSC       |
| Joystick module           | nan                 |          1 | 1.2             | Aliexpress |
| Gear motor                | N20                 |         10 | 7               | Aliexpress |
| PCB + PCBA + other fees   | nan                 |        nan | $14.86          | JLCPCB     |
| Shipping and handling     | nan                 |        nan | $11.62          | LCSC       |
| Shipping                  | nan                 |        nan | $1.99           | Aliexpress |

Total: $50.50

[Sheet With Links](https://docs.google.com/spreadsheets/d/1DUcNddXo8VYgS7BtmotNU0K7JUAEApslePg6cg7R3zo/edit?usp=sharing)
