

# Alice-Ghost-Type: ESP32 Code Typer 🖥️✨

![Alice-Ghost-Type](https://img.shields.io/badge/Alice-Ghost--Type-blueviolet?style=for-the-badge&logo=arduino)  
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=flat-square)](https://github.com/Rohan-Gautam/Alice-Ghost-Type)    
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-orange.svg?style=flat-square)](https://github.com/Rohan-Gautam/Alice-Ghost-Type/issues)  
[![GitHub Issues](https://img.shields.io/github/issues/Rohan-Gautam/Alice-Ghost-Type.svg?style=flat-square)](https://github.com/Rohan-Gautam/Alice-Ghost-Type/issues)  

## Introduction 🚀
Welcome to **Alice-Ghost-Type**, a clever ESP32-WROOM project that types code for you like a ghostly assistant! 👻 Built to bypass frustrating "could not paste" errors in IDEs like Arduino IDE, this tool lets you send code from a web interface on your Mac to a PC via Bluetooth keyboard emulation. No more tab-switching—just paste, send, and watch your code appear! 🎉 
  
## **Note: This project is for *educational purposes only* and must not be used as an exploitation tool.**

## What It Is 📝
Alice-Ghost-Type is an innovative system that:
- 📡 Creates a WiFi hotspot (`ESP32_Hotspot`) at `192.69.69.69`.
- 🌐 Hosts a web interface for pasting code.
- ⌨️ Emulates a Bluetooth keyboard to type code on a paired PC, ideal for Arduino sketches.
- ✅ Achieves near-100% accuracy for special characters like `#`, `(`, `;`, fixing issues like '3' for '#' or dropped ')'.
- 🎓 Helps developers bypass IDE paste restrictions by simulating human typing.

It’s your wireless, fun solution for automating code entry! 😎

## How It Was Made 🛠️
Crafted with passion, here’s the journey:
- **Hardware**: ESP32-WROOM module with dual WiFi and Bluetooth capabilities. ⚡
- **Software**:
  - Arduino IDE with ESP32 board support (v3.0.5).
  - `ESP32-BLE-Keyboard` library (patched for compatibility) for Bluetooth HID.
  - `WiFi` and `WebServer` libraries (ESP32 core) for hotspot and web server.
- **Development Process**:
  - Began with a WiFi client setup, then switched to a standalone hotspot for flexibility. 🌍
  - Resolved `ESP32-BLE-Keyboard` compilation errors by patching `BleKeyboard.cpp` to convert `std::string` to `String`.
  - Addressed packet loss (e.g., '3' for '#', `client();` to `client(;`) with:
    - 50ms delays per key for reliable transmission.
    - Custom key mappings for special characters.
    - 5 retries per key with reconnection attempts.
    - WiFi channel 1 and paused web server to minimize 2.4GHz interference.
- Iterated extensively to ensure rock-solid reliability for Arduino code! 💪

## Idea Behind It 💡
The inspiration? Tackling "could not paste" errors in IDEs that block clipboard pasting, frustrating developers. Alice-Ghost-Type solves this by:
- Simulating human typing via Bluetooth to bypass restrictions.
- Enabling wireless code transfer (e.g., Mac to PC) without USB.
- Streamlining workflows for testing Arduino sketches or other code.
- Adding a fun, wireless interface to make coding a breeze! 🎮

**Disclaimer**: This tool is for *educational use only* to explore ESP32 capabilities and must not be used for malicious purposes.

## How It Works ⚙️
Here’s the magic in action:
1. **WiFi Hotspot** 📶: ESP32 creates `ESP32_Hotspot` (password: `12345678`) at `192.69.69.69`.
2. **Web Interface** 🌐: Mac connects to the hotspot, opens `http://192.69.69.69`, and sees a textarea.
3. **Code Submission** ✍️: Paste code (e.g., `#include <WiFi.h>\nclient();`) and click "Send Code."
4. **Bluetooth Typing** ⌨️: ESP32, paired as “ESP32 Keyboard,” types the code in the PC’s active editor.
5. **Reliability Tricks** 🛡️:
   - 50ms delay per key to prevent packet loss.
   - Custom handling for `#`, `(`, `;`, etc., to ensure accuracy.
   - 5 retries per key with reconnection if BLE drops.
   - WiFi channel 1 and paused web server to minimize interference.

## How to Make Your Own 🔧
### Prerequisites
- **Hardware** 🛠️:
  - ESP32-WROOM module with USB for programming.
- **Software** 💻:
  - Arduino IDE with ESP32 board support (v3.0.5).
  - Patched `ESP32-BLE-Keyboard` library (included in this repo).
  - `WiFi` and `WebServer` libraries (ESP32 core).

### Setup Steps
1. **Clone the Repository** 📥:
   ```bash
   git clone https://github.com/Rohan-Gautam/Alice-Ghost-Type.git
   ```
2. **Install the Patched Library** 📚:
   - Copy `libraries/ESP32_BLE_Keyboard` to `~/Documents/Arduino/libraries/`.
   - Alternatively, patch `BleKeyboard.cpp` in your existing library (see repo for details).
3. **Upload the Code** ```cpp:
   - Open **`esp32_web_keyboard_hotspot.ino`** in Arduino IDE.
   - Customize `ssid` and `password` if desired (*password needs ≥8 characters*).
   - Upload to your ESP32 via USB.
4. **Connect from Mac** 📱:
   - Go to **System Settings > Wi-Fi**, join `ESP32_Hotspot` (password: `12345678`).
   - Browse to `http://192.69.69.69` in your browser.
5. **Pair with PC** 🖥️:
   - Pair **“ESP32 Keyboard”** via Bluetooth on your PC.
   - Open a text editor (e.g., Notepad, Arduino IDE).
6. **Test It** 🧪:
   - Paste code like `#include <WiFi.h>\nclient();` in the web interface.
   - Click **“Send Code”** and verify it types correctly.
   - Check **Serial Monitor** (115200 baud) for debug logs.

## File Structure 📂
- **`esp32_web_keyboard_hotspot.ino`**: Main sketch for hotspot, web server, and Bluetooth typing.
- **`libraries/ESP32_BLE_Keyboard/BleKeyboard.cpp`**: Patched library for BLE keyboard emulation.
- **`LICENSE.txt`**: MIT License for open-source usage.
- **`README.md`**: This awesome guide! 😄
- **`.gitignore`**: Ignores build artifacts and IDE files.

## Working 🎬
Alice-Ghost-Type excels at:
- **Typing Arduino Code** ✅: Perfectly handles `#include <WiFi.h>`, `client();`, and more.
- **Bypassing Paste Restrictions** 🚫: Simulates human typing to dodge IDE clipboard issues.
- **Stable Operation** 🌐: Runs a reliable hotspot and responsive web interface.

### Demo Workflow
1. Mac connects to `ESP32_Hotspot`. 📶
2. Paste `#include <WiFi.h>\nclient();` at `http://192.69.69.69`. ✍️
3. ESP32 types the code into the PC’s Arduino IDE, ready to compile! 🚀

## Result ✅
After rigorous optimization, the project delivers:
- **Accuracy** 🎯: Near-100% typing reliability, fixing misprints ('3' for '#') and dropped characters (e.g., ')').
- **Usability** 🌟: Seamless hotspot (`192.69.69.69`) and intuitive web UI.
- **Robustness** 🛡️: Handles packet loss with 50ms delays, retries, and reconnection.
- **Test Cases**:
  - `#include <WiFi.h>`: Types '#' correctly.
  - `client();`: Includes '(', ')', and ';'.
  - Long sketches (100+ lines): Typed accurately.

## Conclusion 🌈
Alice-Ghost-Type is a game-changer for developers battling IDE paste issues! 🎉 It harnesses the ESP32’s WiFi and Bluetooth to create a wireless, reliable code-typing tool. Built for educational exploration, it’s perfect for Arduino enthusiasts and beyond. Future ideas include NimBLE support, faster typing, or a mobile app interface. **Reminder**: Use this responsibly for learning, not exploitation.

## Connect with Me 🤝
Let’s make Alice-Ghost-Type even better! Contribute ideas, report bugs, or suggest features like:
- ⚡ Faster typing with NimBLE.
- 🌐 Custom key mappings for other languages.
- 📱 A slick mobile app UI.

Reach out:
- **GitHub**: [Rohan-Gautam](https://github.com/Rohan-Gautam) (file issues or PRs).
- **LinkedIn**: [@Rohan Gautam](https://www.linkedin.com/in/rohan-gautam-b33695250/) (Connect with me).
- **Email**: [582004rohangautam@gmail.com](mailto:582004rohangautam@gmail.com) (Mail me anytime).

Join the adventure to build the ultimate code-typing tool! 🚀🔥

