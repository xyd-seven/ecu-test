

//explain：创建三菱mc协议所需要的tcp/ip连接及相关操作, 本进程为客户端
//para serverIP: 服务端ip地址
//para serverPort: 服务端端口号
//ret: 成功返回0，失败返回-1
int mc_open(char* serverIP, unsigned int serverPort);

//explain：关闭三菱mc协议所需要的tcp/ip连接及相关操作
//ret: 成功返回0，失败返回-1
int mc_close(void);

//explain：读取M区软元件状态
//para addr: 软元件地址, 开始软元件M1000地址为1000
//para timeout: 读取超时时间，0 无限等待; 1~65535等待时间(单位250ms); 一般使用10(2.5s)
//ret: 成功返回软元件状态,0或者1; 失败返回-1
int mc_read(unsigned int addr, unsigned int timeout);

//explain：写M区软元件状态
//para addr: 软元件地址, 开始软元件M1000地址为1000; 结束软元件M1050地址为1050; 测试结果软元件为M1055~M1058
//para timeout: 写入超时时间，0 无限等待; 1~65535等待时间(单位250ms); 一般使用10(2.5s)
//ret: 写入成功返回0; 失败返回-1
int mc_write(unsigned int addr, unsigned char state, unsigned int timeout);



