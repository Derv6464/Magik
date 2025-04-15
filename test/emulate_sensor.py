import serial
import time
import struct

class DataPoint:
    def __init__(self, value, values_name):
        self.values_name = values_name
        self.value = value

class DataStamps:
    def __init__(self, time, data_points: list[DataPoint]):
        self.time = time
        self.data_points = data_points

    def get_data(self):
        return [point.value for point in self.data_points]

    def get_time(self):
        return self.time

class Sensor:
    def __init__(self, file_loc, name, columns , struc):
        self.file_loc = file_loc
        self.name = name
        self.data = []
        self.current_index = 0
        self.columns = columns
        self.struc = struc

    def process_data(self):
        with open(self.file_loc) as file:
            for line in file:
                if 'ts' in line:
                    continue
                row = line.strip().split(',')
                data_points = []
                for i, col in enumerate(self.columns[1:],1):
                   
                    if "." in row[i] and "t" not in col:
                        data_points.append(DataPoint(float(row[i]), col))
                    elif "t" in col:
                        data_points.append(DataPoint(int(float(row[i])*100), col))
                    else:
                        data_points.append(DataPoint(int(row[i]), col))

                self.data.append(DataStamps(float(row[0]), data_points))

    def print_data(self):
        for data in self.data:
            print("Time: " + str(data.get_time()))
            for point in data.data_points:
                print(point.values_name + ": " + str(point.value))
            print("")

    def make_data_packet(self):
        print(*self.data[self.current_index].get_data())
        data_packet = struct.pack(self.struc, self.name.encode('ascii'), *self.data[self.current_index].get_data())
        print("Data Packet for " + self.name + " : " + str(data_packet))
        print("Data Packet Length: " + str(len(data_packet)))
        return data_packet
    
def main():
    port = '/dev/tty.usbmodem21301'
    baud = 115200
    print(serial.__file__)

    baro = Sensor("test/test_data/baro.csv", "b", ["time", "t", "p"], "<sii")
    #baro = Sensor("test_data/baro.csv", "b", ["time", "t", "p"], "<sii")
    gnss = Sensor("test/test_data/gnssInfo.csv", "g", ["time", "t", "n"], "<sff")
    accel = Sensor("test/test_data/flightInfo.csv", "a", ["time", "h", "v", "a"], "<sfff")
    timer = -0.755
    start_time = time.time()
    sensors = [baro, gnss, accel]
    for sensor in sensors:
        sensor.process_data()
        #sensor.print_data()
    with open("test/data.txt", "w") as file:
    #with serial.Serial(port, baud, timeout=1) as ser:
        print(time.time() - start_time, timer)
        while timer < 112.5:
            for sensor in sensors:
                print("Sensor: " + sensor.name)
                print("Current Index: " , sensor.data[sensor.current_index].get_time())
                if sensor.data[sensor.current_index].get_time() == timer:
                    packet = sensor.make_data_packet()
                    print(type(packet))
                    file.write(str(timer) + " " + str(packet) + "\n")
                    #ser.write(packet)
                    sensor.current_index += 1
                    #response = ser.read_all().decode().strip()

                    #print("\nPico said: \n", response)
            
            timer = round(timer + 0.005, 3)
    
if __name__ == "__main__":
    print(serial.__file__)
    print(serial.Serial)
    main() 