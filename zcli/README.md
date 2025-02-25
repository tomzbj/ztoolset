# zcli

简易的串口命令行调试工具.

使用方法, 建立cli.cc如下:

    #include <cstdio>
    #include "zcli.h"

    ZCli cli(20); // 最多允许的命令条数

    // 自定义命令处理函数
    // 读取堆起始位置和栈顶位置
    static void cli_rd_sbrk_sp(const char* tokens[])
    {
      register uint8_t* sp asm("sp");
      auto a1 = sp;
      auto a2 = malloc(1);
      printf("%p %p %d\n", a1, a2, (int)a1 - (int)a2);
      free(a2);
    }

    // 也可以用lambda指针
    // 测试串口是否正常收发, 复位, 读取MCU主频(适用于gd32)
    const auto test_f = [](const char**) {puts("TEST OK");};
    const auto reboot_f = [](const char**) {puts("REBOOT."); NVIC_SystemReset();};
    const auto rd_freq_f = []( const char**) {printf("%lu\n", rcu_clock_freq_get(CK_SYS));};

    void CLI_Config(void)
    {
      // 添加命令
      cli.bind("test", test_f);
      cli.bind("reboot", reboot_f);
      cli.bind("sbrk_sp", "rd", cli_rd_sbrk_sp);
      cli.bind("freq", "rd", rd_freq_f);
      // 也可以直接在这里用lambda函数添加简单的命令
    }

之后在串口处理函数里调用cli.parse().

调试时可以在串口助手里发送# test, # reboot, # rd sbrk_sp, # rd freq, 执行对应的功能.