from machine import UART, Pin
import time
import sys

class DataPoint:
    def __init__(self,time, data):
        self.time = time
        self.data = eval(data)

def printstate(time):
    if time < 0:
        print("ready", 2)
    elif time < 5.69:
        print("powered", 3)
    elif time < 21.22:
        print("coasting", 4)
    elif time < 110.36:
        print("drouge", 5)
    elif time < 122.12:
        print('main', 6)
    elif time > 122.12:
        print('landed', 7)
        
   
def main():
    led = machine.Pin(25, machine.Pin.OUT)
    uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))
    uart.init(115200, bits=8, parity=None, stop=1)
    
    start_time = time.time()
    with open("data.txt", "r") as file:
        while True:
            start_op = time.time_ns()
            line = file.readline()
            if not line:
                break  # Stop if the file has fewer than 10 lines
            print(line.strip())
            dataIn = line.split(' ', 1)
            data = DataPoint(float(dataIn[0]), dataIn[1])
            print(printstate(data.time))
            print(data.data)
            print(len(data.data))
            uart.write(data.data)
            #print('end op', time.time_ns()-start_op)
            time.sleep(0.03)
           
        
            
def other_main():
    data = b'b\x18\x10\x00\x00w\x85\x01\x00'
    led = machine.Pin(25, machine.Pin.OUT)
    uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))
    uart.init(115200, bits=8, parity=None, stop=1)
    
    while True:
        try:
            uart.write(data)  # Send it out via UART
            print("recvied", len(data), data)
                
        except Exception as e:
            led.high()
            print("Error:", e)
        time.sleep(3)
    
if __name__ == "__main__":
    main()