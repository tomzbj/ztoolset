# zuart

简易的硬件无关UART库, 支持多个串口实例, 用接收字符中断和空闲中断实现接收, 发送则可以选用阻塞方式或者中断方式.

在stm32上使用串口时当然是首选DMA方式. 但有时需要用到的串口很多, DMA通道不够用; 有时DMA通道被更重要的SPI等外设占用, 这时怎么办? 以及, 有时需要把串口程序从这个MCU移植到那个MCU, 改写起来非常麻烦. 有个硬件无关的串口库就方便多了.

依赖zfifo库[../zfifo/README.md](../zfifo/README.md)

## 配置流程

建立u0对象: 
初始化用到的UART或USART, 并实现USART_Write函数. 或者也可以用DMA方式发送.

    void USART0_Write(uint8_t* data, int count)
    {
      for(int i = 0; i < count; i++)
        USART0_WriteByte(data[i]);
    }

建立u0对象, .pre_tx_f和.post_tx_f两个回调函数分别会在发送开始前和结束后执行, 可以用于485切换方向等操作, 如果不需要则设为nullptr即可.

    const ZUart3::cbs_t cbs = {.write_f = USART0_Write, .pre_tx_f = nullptr, .post_tx_f = nullptr};
    ZUart3 u0(cbs, 16384);  // 用于zuart的fifo大小, 可自定义

最简单的情况: 只需要启用RXNE中断, 在RXNE中断里读取1字节并推送给zuart, 适用于通讯量不大, 硬件不提供IDLE中断, 或者没有多余DMA通道的情况:

    ...
    uint8_t c = USART0_Readbyte();
    u0.push(&c, 1);
    ...

当通讯量较大时可以关闭RXNE中断, 打开IDLE中断, 配置DMA读取并打开DMA的半满和全满中断:

    ...
    static void usart_clear_idle(void)    // 对于stm32f10x和f4xx, 需要采用读SR再读DR的方式来清除flag, gd32原样照抄了这个bug
    {
      volatile uint32_t tmp = 0;
      tmp = tmp;
      tmp = USART_STAT0(USART0);
      tmp = USART_DATA(USART0);
    }

    extern "C" void DMA1_Channel5_IRQHandler(void)
    {
      if(dma_interrupt_flag_get(DMA1, DMA_CH5, DMA_INT_FLAG_HTF)) {
        dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INT_FLAG_HTF);
        u0.push(rxbuf, RXBUF_SIZE / 2);   // 推送DMA缓冲区的前面半截
      }
      if(dma_interrupt_flag_get(DMA1, DMA_CH5, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INT_FLAG_FTF);
        u0.push(&rxbuf[RXBUF_SIZE / 2], RXBUF_SIZE / 2);    // 推送DMA缓冲区的后面半截
      }
    }

    extern "C" void USART0_IRQHandler(void)
    {
      if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET) {
        usart_clear_idle();
        int size = RXBUF_SIZE - dma_transfer_number_get(DMA1, DMA_CH5); // 根据DMA剩余数据量计算需要推送的部分
        if(size < RXBUF_SIZE / 2) {
          u0.push(rxbuf, size);
        }
        else {
          u0.push(&rxbuf[RXBUF_SIZE / 2], size - RXBUF_SIZE / 2);
        }
        dma_channel_disable(DMA1, DMA_CH5);
        dma_flag_clear(DMA1, DMA_CH5, DMA_FLAG_HTF);
        dma_flag_clear(DMA1, DMA_CH5, DMA_FLAG_FTF);
        dma_transfer_number_config(DMA1, DMA_CH5, RXBUF_SIZE);
        dma_channel_enable(DMA1, DMA_CH5);
      }
      usart_interrupt_flag_enum errs[] = {USART_INT_FLAG_ERR_NERR,
        USART_INT_FLAG_ERR_ORERR, USART_INT_FLAG_ERR_FERR};
      for(auto& e : errs)
        if(usart_interrupt_flag_get(USART0, e) != RESET)
          usart_clear_idle();
    }
    ...

在SysTick_Handler里执行u0.tick().

zuart本身的idle超时时间可以独立设置, 执行u0.set_timeout(timeout)即可.

之后在主循环或者单独任务里轮询u0.idle(), 如果为true则可以继续进行后续处理.

    ...
    while(1) {
      if(u0.idle()) {
        char buf[1024] = {0};
        char c = u0.peek();   // 偷看1字节
        int len = u0.read(buf, sizeof(buf));
        if(c == '#') cli.parse(buf, len); // 根据首字节决定之后的处理方式
      }
    }
    ...

如果需要使用printf, 对于gcc, 重写_write函数即可.

    ...
    int _write(int file, char* ptr, int len)
    {
      u0.write(ptr, len);
      return len;
    }
    ...

