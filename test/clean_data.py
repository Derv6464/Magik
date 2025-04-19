import pandas as pd

imu_data = pd.read_csv("test/test_data/imu.csv")
print(imu_data)
imu_data = imu_data.drop(columns=['Unnamed: 0', 'id', 'Gx', 'Gy', 'Gz'])

print(imu_data)

imu_data.to_csv( "test/test_data/imu.csv", index=False)