#ifndef REBOOT_H
#define REBOOT_H
#define PM_PASSWORD 0x5a000000 //寫入 PM 暫存器時必須加上的密碼常數（避免誤觸）
#define PM_RSTC 0x3F10001c //Reset Control Register：控制重設模式
#define PM_WDOG 0x3F100024 //Watchdog Timer Register：設定倒數時間

void set(long addr, unsigned int value);
void reset(int tick);
void cancel_reset();

#endif