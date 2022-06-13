import serial
import serial.tools.list_ports

from struct import pack, unpack
 
class Packet:
    def __init__(self, message_type: int, message: str) -> None:
        self.start = 0x69
        self.message_type = message_type
        self.message_lengh = len(message) % 256
        self.crc = (sum([ord(c) for c in message]) + len(message)) % 256
        self.message = message
 
    def get_packet(self) -> bytearray:
        header = pack(
            f"BBBB",
            self.start,
            self.message_type,
            self.message_lengh,
            self.crc,
        )
        return header + self.message.encode("ascii")

stm_serial_handle = None
not_connected = True

while not_connected:
    ports = serial.tools.list_ports.comports()
    stm_port = None
    for port, desc, hwid in sorted(ports):
        if(desc[0:3] == 'STM'):
            stm_port = port
            break
    
    try:
        stm_serial_handle = serial.Serial(stm_port, 115200)
        print(f"STM32 znaleziony na porcie {stm_port}")
        not_connected = False

    except:
        print("Brak urządzenia STM32")

import time

for i in range(0, 10, 1):
    tmp_packet = Packet(0, "open")
    stm_serial_handle.write(tmp_packet.get_packet())
    time.sleep(1)

    tmp_packet = Packet(0, "close")
    stm_serial_handle.write(tmp_packet.get_packet())
    time.sleep(1)
