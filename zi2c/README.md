# zi2c

简易的软件i2c主机库, 及若干常见器件驱动, 大部分为单header实现. 

## 使用方法

依赖ZPin库(链接: [../zpin/README.md](../zpin/README.md) ), 传入i2c外设用到的SDA和SCL管脚、外设地址和delay_us函数指针, 及默认的延迟时间即可.

    ZI2C i2c(PB1, PB2, CST226SE_I2C_ADDR, _delay_us, 10);

之后把i2c对象传递给i2c外设驱动.

    CST226SE cst226(i2c);

i2c外设驱动程序中调用i2c.read_reg和i2c.write_reg函数即可.

## 已支持器件 (更新中)

### 实时时钟

DS1307(待更新)

### 传感器

加速度传感器/陀螺仪: LIS3DHTR MMA7660 MPU9250 (待更新)  
磁场传感器: QMC5883 (待更新)  
温度传感器: LM75 (待更新)  
CO2和TVOC传感器: CCS811 (待更新)   
湿度传感器: SHTC3 (待更新), WHT20/AHT20  
气压传感器: LPS22HH  

### 显示

SSD1306(OLED屏)  
CST226SE(电容触摸屏驱动)  
NS2009(电阻触摸屏驱动, 待更新)  

### 时钟发生

Si5351  

### ADC

MCP3421 

### DAC

MCP4726 (待更新)

### FM收音机

RDA5807M (待更新)

### 待添加

ADXL345 ALS31313 TM1650 OPT3001 24Cxx PCF8574 QN8027

## 附: 常用i2c器件地址

|型号|类型|地址|备注|
|----|----|----|----|
|CAT24C02|EEPROM|0x50~0x57|可由A0~A2脚设置|
|AT24C02|EEPROM|0x50~0x57|同上|
|LM75|SENSOR/TEMP|0x48~0x4f|同上|
|DS1307|RTC|0x68||
|LIS3DHTR|MEMS|0x18~0x19|由SA0脚设置|
|MMA7660|MEMS|0x4c|原厂可定制|
|MPU9250|MEMS|0x68~0x69|由AD0脚设置|
|SSD1306|DISP|0x3c~0x3d|由SA0脚设置(在SPI模式下是DC脚)|
|NS2009|ADC|0x48~0x4b|由A0~A1脚设置|
|RDA5807M|FM|0x10||
|MCP3421|ADC|0x68~0x6f|看型号后缀末位, 默认0x68(原厂可定制)|
|Si5351|PLL|0x60~0x61|由A0脚设置|
|SHTC3|SENSOR|0x70||
|CCS811|SENSOR|0x5a||
|QMC5883|SENSOR|0x0d|原厂可定制|
|ALS31313|SENSOR|0x60~0x6e|由AD0和AD1脚电压决定, 也可由原厂定制|
|MCP4726|DAC|0x60~0x67|看型号后缀末位, 默认0x60(原厂可定制)|
