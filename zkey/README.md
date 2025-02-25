# zkey

简单的按键处理库, 参考了murphyzhao的FlexibleButton.

## 简介

murphyzhao的FlexibleButton是一个强大的按键处理库, 链接: https://github.com/murphyzhao/FlexibleButton. 

参考FlexibleButton实现了一个极简版, 主要不同之处:

1. 总共只需要提供两个回调函数, 一个用于读键, 一个用于执行按键操作.
2. 只定义了按下, 短按弹起, 长按弹起这三个事件. 多数简单应用其实有这三个就足够了.
3. 长按阈值为全局设置. (FlexibleButton的每个键可以设置不同的阈值.)

## 使用方法

    两个回调函数如下, 这里用PA0和PB11两个GPIO作为按键:

    static int key_read(int key_id);
    static void key_action(int key_id, int action);

    static ZKey::cbs_t cbs = {key_read, key_action};

    int key_count[2], key_state[2];
    // 长按阈值为300ms
    ZKey zk(cbs, 2, 300, key_count, key_state);

    enum { KEY_TOP = 0, KEY_BOTTOM = 1 };

    static int key_read(int key_id)
    {
        //  const zpin_t keys[2] = {PA0, PB11};
        if(key_id == 0)
            return ZPin::read(PA0);
        else if(key_id == 1)
            return !ZPin::read(PB11);
        return 0;
    }

    constexpr static int hash(int key_id, int action)
    {
        return key_id * 100 + action;
    }

    static void key_action(int key_id, int action)
    {
        extern class ZEVENT ze;
        switch(hash(key_id, action)) {
            case hash(KEY_TOP, ZKey::PRESS):
                ze.post(EVENTS::KEY_TOP_PRESSED);
                break;
            case hash(KEY_TOP, ZKey::LONGPRESS):
                ze.post(EVENTS::KEY_TOP_LONGPRESSED);
                break;
            case hash(KEY_BOTTOM, ZKey::PRESS):
                ze.post(EVENTS::KEY_BOTTOM_PRESSED);
                break;
            case hash(KEY_BOTTOM, ZKey::LONGPRESS):
                ze.post(EVENTS::KEY_BOTTOM_LONGPRESSED);
                break;
    }
    }

初始化: 把用到的两个按键对应的gpio初始化为上拉输入即可.

轮询: 每1ms执行一次zk.poll()即可. 如果按键响应处理耗时较长, 则不要放在SysTick中断中执行, 可以用其他调度器来调度zk.poll().    
    
