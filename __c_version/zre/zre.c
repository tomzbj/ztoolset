#include <stdio.h>
#include "zre.h"

static zre_t g;

void zre_init(zre_t* zre)
{
    g = *zre;
}

void zre_poll(void)
{
    for(int i = 0; i < ZRE_NUM; i++) {
        unsigned char state = ((g.old_state[i] << 2) | g.read_f(i)) & 0xf;
        switch(state) {
            case 0x1:
            case 0x7:
            case 0xe:
            case 0x8:
                g.action_f(i, ZRE_CW);
                break;
            case 0x2:
            case 0xb:
            case 0xd:
            case 0x4:
                g.action_f(i, ZRE_CCW);
                break;
        }
        g.old_state[i] = state;
    }
}
