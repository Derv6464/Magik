class UART:
    def __init__(self, data):
        self.data = data

    def send(self, data):
        print("Sending data: " + data)
    
class DataPoint:
    def __init__(self, value, values_name):
        self.values_name = values_name
        self.value = value

class DataStamps:
    def __init__(self, time, data_points: list[DataPoint]):
        self.time = time
        self.data_points = data_points

    def get_data(self):
        data = ""
        for point in self.data_points:
            data += point.values_name + str(point.value)
        return data

    def get_time(self):
        return self.time

class Sensor:
    def __init__(self, file_loc, name, columns):
        self.file_loc = file_loc
        self.name = name
        self.data = []
        self.current_index = 0
        self.columns = columns

    def process_data(self):
        #read file 
        with open(self.file_loc) as file:
            for line in file:
                if 'ts' in line:
                    continue
                row = line.strip().split(',')
                data_points = []
                for i, col in enumerate(self.columns[1:],1):
                    print("col: " + col, i)
                    data_points.append(DataPoint(float(row[i]), col))

                self.data.append(DataStamps(float(row[0]), data_points))

    def print_data(self):
        for data in self.data:
            print("Time: " + str(data.get_time()))
            for point in data.data_points:
                print(point.values_name + ": " + str(point.value))
            print("")



    def make_data_packet(self, time):
        data_packet = self.name + str(time) + str(self.data[self.current_index].get_data())
        return data_packet
    
def main():
    uart = UART("UART data")
    baro = Sensor("test/test_data/baro.csv", "baro", ["time", "temp", "pres"])
    timer = -0.755

    sensors = [baro]
    for sensor in sensors:
        sensor.process_data()
        sensor.print_data()

    while timer < 112.5:
        for sensor in sensors:
            if sensor.data[sensor.current_index].get_time() == timer:
                packet = sensor.make_data_packet(timer)
                uart.send(packet)
                sensor.current_index += 1
        timer = round(timer + 0.005, 3)
    
if __name__ == "__main__":
    main()