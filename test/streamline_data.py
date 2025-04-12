import pandas as pd
import numpy as np
from scipy.interpolate import interp1d
orientation_df = pd.read_csv("test/test_data/orientationInfo.csv")
gnss_df = pd.read_csv("test/test_data/gnssInfo.csv")
baro_df = pd.read_csv("test/test_data/baro.csv")
flight_df = pd.read_csv("test/test_data/flightInfo.csv")

eggtimer_df = pd.read_csv("test/test_data/dtl3.csv")
fsm_df = pd.read_csv("test/test_data/flightStates.csv")
filtered_df = pd.read_csv("test/test_data/filteredDataInfo.csv")


baro_df = baro_df.drop(columns=['id'])
baro_df.to_csv("test/test_data/baro.csv", index=False)

