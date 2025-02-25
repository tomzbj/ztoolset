#ifndef _ZRE_H
#define _ZRE_H

#define ZRE_NUM 1

typedef enum {
    ZRE_CW, ZRE_CCW
} zre_action_t;


typedef struct {
    unsigned char (*read_f)(int rot_id);
    void (*action_f)(int rot_id, zre_action_t action);
    unsigned char old_state[ZRE_NUM];
} zre_t;

void zre_init(zre_t* zre);
void zre_poll(void);

#endif
