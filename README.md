# ping-pong-mik32
## Brief Description
An implementation of arcade Ping-Pong for ELBEAR ACE-UNO board based on the MIK32 Amur MCU.

**Provided Features:**
- Menu
- PvP
- PvE
- Audio Support
- Settings Section

Built with the `mik32-hal` and `mik32-shared` submodules.
## Hardware
**Components required:**
- ELBEAR ACE-UNO Board
- SSD1306 OLED Display (SPI, 128x64)
- 4x Buttons
- Passive Buzzer
- Pull-up Resistors
	- 1x 220 Ω (Passive Buzzer)
	- 4x 10 kΩ (Buttons)
- Jumper Wires
## Installation
### Download Dev Container Extension
1. Install VSCode
2. Go to `Extensions`
3. Find `Dev Containers` Extension
4. Install via Description
### Clone the repository
```
git clone git@github.com:Nekich06/ping-pong-mik32.git
cd ping-pong-mik32
```
### Build Docker Image Using Dev Container
1. Press `Ctrl` + `Shift` + `P`
2. Choose `Dev Containers: Reopen in Container`
3. Wait for image build ending
Now you're in Dev Container.
### Build firmware

>**Note:** `mik32-hal` function `HAL_SPI_Exchange` should have `__attribute__((section(".ram_text")))`, which is set **manually** before building firmware

```
make
```
### Flash MIK32 Amur
```
make flash
```
## Wiring Diagram / Plan
<img width="1968" height="1845" alt="ping-pong-wire-plan" src="https://github.com/user-attachments/assets/bbb96b51-9347-4a0d-b039-42144e9a3b3a" />

### SSD1306 OLED Display (SPI, 128x64)

| **Display Pins** | **ACE-UNO** |
| ---------------- | ----------- |
| **VCC**          | **3.3V**    |
| **GND**          | **GND**     |
| **D0 (SCLK)**    | **D6**      |
| **D1 (MOSI)**    | **D5**      |
| **RES**          | **D0**      |
| **DC**           | **D2**      |
| **CS**           | **D4**      |
### Buttons
| **Purpose**       | **ACE-UNO** |
| ----------------- | ----------- |
| **PLAYER_1_DOWN** | **D12**     |
| **PLAYER_1_UP**   | **D11**     |
| **PLAYER_2_DOWN** | **D13**     |
| **PLAYER_2_UP**   | **D18**     |
### Passive Buzzer
| **Polarity** | **ACE-UNO** |
| ------------ | ----------- |
| **+**        | **D10**     |
| **-**        | **GND**     |
## References / Useful Docs
* [opisanie-elbear-ace_uno-rev.1.1-11.08.1-a3.pdf](https://github.com/user-attachments/files/28034439/opisanie-elbear-ace_uno-rev.1.1-11.08.1-a3.pdf)
* [MIK32_datasheet_v2.2.2.pdf](https://github.com/user-attachments/files/28034442/MIK32_datasheet_v2.2.2.3.pdf)
* [SSD1306.pdf](https://github.com/user-attachments/files/28034450/DOC001404223.pdf)
* [adafruit/Adafruit_SSD1306: Arduino library for SSD1306 monochrome 128x64 and 128x32 OLEDs](https://github.com/adafruit/Adafruit_SSD1306?ysclid=mpda5swber322517148)
* [adafruit/Adafruit-GFX-Library: Adafruit GFX graphics core Arduino library, this is the 'core' class that all our other graphics libraries derive from](https://github.com/adafruit/Adafruit-GFX-Library?ysclid=mpda5gyox4440863316)
 
