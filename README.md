<h1 align="center">
  <a href="https://www.youtube.com/@eccentric_engineer">
	<img
		width="200"
		alt="Avinashee Tech"
		src="img/Avinashee Tech Logo New.png">
  </a>  
</h1>

<h3 align="center">
	Minimal SSD1306 Graphics Driver for STM32
</h3>




  
## 📝 Overview

This repository provides a platform-independent driver for the SSD1306 OLED display controller available in the SSD1306_OLED_common/
folder, tailored for STM32 using STM32CubeIDE.
The implementation focuses on delivering a clean, modular, and reusable codebase that can serve as a foundation for simple graphics rendering and advanced GUI integrations.

The driver supports basic pixel-level drawing, and buffer management at the moment. It is designed with portability in mind, 
making it easier to adapt the same code structure for other platforms (such as ESP32 or different MCUs) without major changes.

This project represents the first stage of the SSD1306 + STM32 integration: building a standalone driver.
Future work includes adding LVGL support and expanding higher-level graphics functionality.

Reference taken from Adafruit SSD1306 library - https://github.com/adafruit/Adafruit_SSD1306  
Platform used for firmware development is STM32CubeIDE v1.18.1  
Learn more about this series 👇👇  
  
Part 1 👇  
[![STM32_SSD1306_PART1_Youtube Video](img/oledpt1thumbnail.png)](https://youtu.be/BN-J8Ks-_c4)  

Part 2.1 👇  
[![STM32_SSD1306_PART2_1_Youtube Video](img/oledpt2.1thumbnail.png)](https://youtu.be/xIpSNDCL8Ik)  

Part 2.3 👇  
[![STM32_SSD1306_PART2_3_Youtube Video](img/oledpt2.3thumbnail.png)](https://youtu.be/BlbK9RoPh0g)  

  
## ✔️ Requirements

### 📦 Hardware
- STM32F411 Black Pill (main controller  board)
- USB C Cable
- SSD1306 Oled Display Module
- Jumper Cables 

### 📂 Software
- STM32CubeIDE (https://www.st.com/en/development-tools/stm32cubeide.html)  

## 🛠️ Installation and usage

```sh
git clone https://github.com/AvinasheeTech/stm32-ssd1306-cubeide-driver.git
Open STM32CubeIDE.
Go to option 'Open Projects from File System' and select project directory.
Go to Core/Src/main.c and press Ctrl+B shortcut to build it or use build icon from toolbar.
Next connect ST-LinkV2 with your board. Setup Debug Configurations. 
Select Run or Debug icon and make sure that elf file is selected in respective configurations.
Once upload is complete, connect SSD1306 oled module to the spi pins highlighted in .ioc file.
Enjoy...🍹
```
To learn more about how to upload code to STM32 using STLink-V2, click link below 👇👇  

[![STM32 Youtube Video](img/stlinkv2thumbnail.png)](https://youtu.be/XuZgJvGf_Nw)


## ⭐️ Show Your Support

If you find this helpful or interesting, please consider giving us a star on GitHub. Your support helps promote the project and lets others know that it's worth checking out. 

Thank you for your support! 🌟

[![Star this project](https://img.shields.io/github/stars/AvinasheeTech/stm32-ssd1306-cubeide-driver?style=social)](https://github.com/AvinasheeTech/stm32-ssd1306-cubeide-driver/stargazers)
