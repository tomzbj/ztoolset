# zfifo

单header kfifo.

使用方法:

初始化: 提供缓冲区尺寸即可.

    ZFIFO fifo(1024);

peek: 查看fifo的首字节.  
traverse: 对fifo每个元素调用printf, 需要传入printf的函数指针(方便使用自行实现的printf.)  
put: 向fifo插入数据, 可以一次插入多个字节.  
get: 从fifo取出数据, 可以一次取出多个字节.  
len: 取得fifo中的数据数量(单位字节).  

