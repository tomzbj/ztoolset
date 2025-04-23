# zcharlie

简易的查理复用方式数码管驱动, 硬件无关, 支持多个串口实例.

使用方法: 管脚和gpio对应关系在回调函数里指定, 引脚排列做成数组, 再传给charlie实例的构造函数. 在SysTick_Handler里调用charlie.poll()即可. 不过如果是逐led扫描, 1ms的SysTick中断还是嫌慢了点, 最好是用几十kHz的定时器中断来调用. MCU随便什么都可以, 这里提供gd32f303cc和gd32f405rg的两个demo.
