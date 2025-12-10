import time
import serial

# --- 設定 ---
# 每個位元組傳送後的延遲時間（秒）
# 0.0001 = 0.1 毫秒，這是一個不錯的起點
BYTE_DELAY = 0.0005 

# --- 主程式 ---
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
    timeout=1
)

with open("/home/zhixuan/rpi3_proj/Lab2/kernel8.img", "rb") as f:
    data = f.read()

# 1. 傳送資料長度（這部分可以一次性傳送）
size = len(data)
print(f"Sending size: {size} bytes...")
ser.write(size.to_bytes(4, 'little'))

# 可選：等待 RPi 確認，增加可靠性
# time.sleep(0.1) 

# 2. 逐一傳送 kernel image 的內容
print("Sending kernel data byte by byte...")
for byte in data:
    ser.write(bytes([byte])) # 將單個 byte 轉換成 bytes 物件並傳送
    time.sleep(BYTE_DELAY)   # 在每個位元組後稍作等待

print("Send complete.")
ser.close()
