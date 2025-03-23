#include <stdio.h>
#include "si5351.h"

#define REG_CLK_CTRL(x) (16 + (x)) // x=0..7
#define REG_PLLA_PARAMS(x) (26 + (x)) // x=0..7
#define REG_PLLB_PARAMS(x) (34 + (x)) // x=0..7
#define REG_MS_PARAMS(x) (42 + (x) * 8) // x=0..5
#define REG_SSC_PARAMS_BASE (149)
#define REG_SSC_PARAMS(x) (REG_SSC_PARAMS_BASE + (x))   // x=0..12
#define REG_CLK_PHASE_OFS(x) (165 + (x))  // x=0..5

//static int g_mode;

enum {
    REG_DEVICE_STATUS = 0, REG_INTERRUPT_STATUS = 1, REG_INTERRUPT_MASK = 2,
    REG_OUTPUT_ENABLE_CTRL = 3, REG_OEB_PIN_ENABLE_CTRL = 9,
    REG_PLL_INPUT_SOURCE = 15, REG_CLK3_0_DISABLE_STATE = 24,
    REG_CLK7_4_DISABLE_STATE = 25, REG_CLK6_7_OUTPUT_DIVIDER = 92,
    REG_VXCO_PARAMETERS_LOW = 162, REG_VXCO_PARAMETERS_MID = 163,
    REG_VXCO_PARAMETERS_HIGH = 164, REG_PLL_RESET = 177,
    REG_CRYSTAL_LOAD = 183,
    REG_FANOUT_ENABLE = 187,
};

enum {
    REG_STATUS_SYS_INIT = 0x80, REG_STATUS_LOL_B = 0x40,
    REG_STATUS_LOL_A = 0x20, REG_STATUS_LOS = 0x10, REG_CLKIN_DIV_MASK = 0xc0,
    REG_CLKIN_DIV_1 = 0x00, REG_CLKIN_DIV_2 = 0x40, REG_CLKIN_DIV_4 = 0x80,
    REG_CLKIN_DIV_8 = 0xc0, REG_PLLB_SOURCE = 0x08, REG_PLLA_SOURCE = 0x04,
    REG_CLK_POWERDOWN = 0x80, REG_CLK_INTEGER_MODE = 0x40,
    REG_CLK_PLL_SELECT = 0x20, REG_CLK_INVERT = 0x10,
    REG_CLK_INPUT_MASK = 0x0c,
    REG_CLK_INPUT_XTAL = 0x00, REG_CLK_INPUT_CLKIN = 0x04,
    REG_CLK_INPUT_MULTISYNTH_0_4 = 0x08, REG_CLK_INPUT_MULTISYNTH_N = 0x0c,
    REG_CLK_DRIVE_STRENGTH_MASK = (3 << 0),
    REG_CLK_DRIVE_STRENGTH_2MA = (0 << 0),
    REG_CLK_DRIVE_STRENGTH_4MA = (1 << 0),
    REG_CLK_DRIVE_STRENGTH_6MA = (2 << 0),
    REG_CLK_DRIVE_STRENGTH_8MA = (3 << 0), REG_CLK_DISABLE_STATE_MASK = 3,
    REG_CLK_DISABLE_STATE_LOW = 0, REG_CLK_DISABLE_STATE_HIGH = 1,
    REG_CLK_DISABLE_STATE_FLOAT = 2, REG_CLK_DISABLE_STATE_NEVER = 3,
    REG_PARAMETERS_LENGTH = 8, REG_OUTPUT_CLK_DIV_MASK = (7 << 4),
    REG_OUTPUT_CLK6_DIV_MASK = (7 << 0), REG_OUTPUT_CLK_DIV_SHIFT = 4,
    REG_OUTPUT_CLK_DIV6_SHIFT = 0, REG_OUTPUT_CLK_DIV_1 = 0,
    REG_OUTPUT_CLK_DIV_2 = 1, REG_OUTPUT_CLK_DIV_4 = 2,
    REG_OUTPUT_CLK_DIV_8 = 3, REG_OUTPUT_CLK_DIV_16 = 4,
    REG_OUTPUT_CLK_DIV_32 = 5, REG_OUTPUT_CLK_DIV_64 = 6,
    REG_OUTPUT_CLK_DIV_128 = 7, REG_OUTPUT_CLK_DIVBY4 = (3 << 2),
    REG_PLL_RESET_B = 0x80, REG_PLL_RESET_A = 0x20,
    REG_CRYSTAL_LOAD_MASK = 0xc0, REG_CRYSTAL_LOAD_6PF = 0x40,
    REG_CRYSTAL_LOAD_8PF = 0x80, REG_CRYSTAL_LOAD_10PF = 0xc0,
    REG_CLKIN_ENABLE = 0x80, REG_XTAL_ENABLE = 0x40,
    REG_MULTISYNTH_ENABLE = 0x10
};

void SI5351::init(void) const
{
    _i2c.write_reg(REG_CRYSTAL_LOAD, REG_CRYSTAL_LOAD_6PF);
    _i2c.write_reg(REG_OUTPUT_ENABLE_CTRL, 0x00);    // enable ms0~ms2
    _i2c.write_reg(REG_PLL_RESET, 0xa0);
}

void SI5351::read_all_regs(void) const
{
    for(int i = 0; i < 256; i++) {
        printf("%02x: %02x    ", i, _i2c.read_reg(i));
        if(i % 8 == 7)
            printf("\n");
    }
}

static uint8_t calc_div_ratio(uint32_t freq)
{
    if(freq < 6800000UL)          //<6.8MHz
        return 126;
    else if(freq < 8800000UL)     //6.8~8.8MHz
        return 100;
    else if(freq < 12300000UL)     //8.8~12.3MHz
        return 72;
    else if(freq < 17300000UL)     //12.3~17.3MHz
        return 52;
    else if(freq < 24800000UL)     //17.3~24.8MHz
        return 36;
    else if(freq < 32100000UL)     //24.8~32.1MHz
        return 28;
    else if(freq < 44300000UL)     //32.1~44.3MHz
        return 20;
    else if(freq < 56250000UL)     //44.3~56.25MHz
        return 16;
    else if(freq < 75000000UL)     //56.25~75MHz
        return 12;
    else if(freq < 112500000UL)     //75~112.5MHz
        return 8;
    else if(freq < 150000000UL)     //112.5MHz~150MHz
        return 6;
    else
        return 4;    //>150MHz
}

void SI5351::set_pll(int pll_id, uint32_t freq) const
{
    const uint32_t c = 0x80000UL;
    uint8_t a = freq / _fxtal;
    uint32_t b = (uint64_t)(freq % _fxtal) * c / _fxtal;
    uint32_t p1, p2;
    uint8_t pll_reg;

    p2 = 128 * b / c;
    p1 = 128 * a + p2 - 512;
    p2 = 128 * b - c * p2;

    uint8_t vals[8] = {
        //
        (uint8_t)((c >> 8) & 0xff),
        (uint8_t)(c & 0xff),
        (uint8_t)(p1 >> 16),
        (uint8_t)(p1 >> 8),
        (uint8_t)(p1),    //
        (uint8_t)(((c >> 12) & 0xf0) | ((p2 >> 16) & 0x0f)),
        (uint8_t)(p2 >> 8),
        (uint8_t)(p2)
    }
    ;
    if(pll_id == PLLA)
        pll_reg = REG_PLLA_PARAMS(0);
    else
        pll_reg = REG_PLLB_PARAMS(0);
    _i2c.write_reg(pll_reg, vals, sizeof(vals));
}

void SI5351::set_ms(int ms_id, int pll_id, int divr) const
{
    uint32_t p1 = 128UL * divr - 512;
    uint8_t vals[8] = {0, 1, (uint8_t)(p1 >> 16),
        (uint8_t)(p1 >> 8),
        (uint8_t)p1,
        0, 0, 0};
    uint8_t msreg = REG_MS_PARAMS(ms_id);
    uint8_t msctrl = REG_CLK_CTRL(ms_id);

    _i2c.write_reg(msreg, vals, sizeof(vals));
    if(pll_id == PLLA)
        _i2c.write_reg(msctrl, 0x0f);
    else
        _i2c.write_reg(msctrl, 0x2f);
}

void SI5351::set_phase(uint8_t divr) const
{
    _i2c.write_reg(REG_CLK_PHASE_OFS(0), 0);
    _i2c.write_reg(REG_CLK_PHASE_OFS(1), divr);
    _i2c.write_reg(REG_PLL_RESET, 0xa0);
}

void SI5351::set_freq(uint32_t freq) const
{
//    g_mode = MODE_QUAD;
    uint8_t divr = calc_div_ratio(freq);
    uint32_t f_pll = freq * divr;    // config pll
    set_pll(PLLA, f_pll);
    set_ms(MS0, PLLA, divr);    // config ms
    set_ms(MS1, PLLA, divr);
    set_phase(divr);    // config phase
}

void SI5351::set_freq(uint32_t freq_1, uint32_t freq_2) const
{
//    g_mode = MODE_DUAL;
    uint8_t divr_1 = calc_div_ratio(freq_1);
    uint8_t divr_2 = calc_div_ratio(freq_2);
    uint32_t f_plla = freq_1 * divr_1;    // config pll
    uint32_t f_pllb = freq_2 * divr_2;
    set_pll(PLLA, f_plla);
    set_pll(PLLB, f_pllb);
    set_ms(MS0, PLLA, divr_1);    // config ms
    set_ms(MS1, PLLB, divr_2);
}
