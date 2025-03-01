# zpin

统一的GPIO读写操作库

## 简介

各种XXX32一般都有自家的库, GPIO操作方式各不相同, 程序在不同MCU之间移植时就会比较麻烦. 如果给GPIO操作增加一个抽象层就简单多了.  
目前支持STM32和GD32, 需要增加别家MCU支持时编辑zpin_port.h即可.  

## 使用方法

只需要include一个zpin.h即可.

读GPIO: ZPin::read(PA0)  
GPIO输出高电平: ZPin::set(PA0)  
GPIO输出低电平: ZPin::clear(PA0)  
GPIO输出指定状态: ZPin::write(PA0, state)  
GPIO翻转: ZPin::toggle(PA0)
