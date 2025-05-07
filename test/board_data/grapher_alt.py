import plotly.express as px
import plotly.graph_objects as go
import pandas as pd
import numpy as np

def get_baseline_data():
    df_egg = pd.read_csv('test/board_data/eggtimer/dtl3.csv')
    # feet to meters
    df_egg['Alt'] = df_egg['Alt'].apply(lambda x : x*0.3048)
    df_egg['FAlt'] = df_egg['FAlt'].apply(lambda x : x*0.3048)

    df_vega = pd.read_csv('test/board_data/vega/flightInfo.csv')

    df_vega["eggAlt"] = df_vega['ts'].map(df_egg.set_index('T')['Alt'])
    df_vega["eggFAlt"] = df_vega['ts'].map(df_egg.set_index('T')['FAlt'])

    df_vega['upper'] = df_vega[['height', 'eggFAlt']].max(axis=1) * 1.05
    df_vega['lower'] = df_vega[['height', 'eggFAlt']].min(axis=1) * 0.95
    
    print(df_vega)

    return df_vega


def make_baseline_graph(fig, df_vega):
    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['height'],
               mode='lines',
               name='Cats Vega'))
    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['eggAlt'], mode='lines', connectgaps=True, name='Eggtimer not filered'))
    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['eggFAlt'], mode='lines', connectgaps=True, name='Eggtimer filtered'))
   

def make_graph():
    #df_vega, df_egg = get_baseline_data()
    #make_baseline_graph(fig, df_vega, df_egg)
   

    fig = go.Figure()
    title = ""
    x_axis = ""



    x_axis = "Height (m)"
    title = "Altitude comparison"

    fig.update_layout(
        title=dict(text=title, x=0.5),
        xaxis=dict(
            title=dict(
                text="Time (s)"
            )
        ),
        yaxis=dict(
            title=dict(
                text=x_axis
            )
        )
    )

    fig.show()
   
def get_test_data():
    f = open('test/board_data/magik/2.txt', 'r')
    #data ponts were read in 0.5s appart (est)
    lines = f.readlines()
    f.close()
    df = pd.DataFrame(columns=['time', 'alt', 'vel', 'acc', 'state'])
    state = 0
    start_time = int(lines[0].split(' ')[1])
    for line in lines:
        if 'data' in line:
            line = line.split(' ')
            if len(line) != 6:
                continue
            try:
                df.loc[len(df)]= [float((int(line[1])- start_time)/1000),float(line[2]),float(line[3]),float(line[4]),int(line[5][0])]
            except:
                print("Error parsing line: ", line)
                continue

    print(df)
    df['time_diff'] = df['time'].diff()  # Difference between consecutive times

    # Drop the first row if needed (since its diff will be NaN)
    avg_time_diff = df['time_diff'].iloc[1:].mean()
    scaling_factor = 0.03 / avg_time_diff
    print("Scaling factor:", scaling_factor)
    df['time_scaled'] = df['time'] / 10
    df['time_scaled_factor'] = df['time'] * scaling_factor
    print("Average time difference:", avg_time_diff)

    print(df)
    return df

import numpy as np

def make_test_graph():
    df = get_test_data()
    df_vega = get_baseline_data()

    # Interpolate upper/lower bounds to Magik time scale
    interp_upper = np.interp(df['time_scaled'], df_vega['ts'], df_vega['upper'])
    interp_lower = np.interp(df['time_scaled'], df_vega['ts'], df_vega['lower'])

    fig = go.Figure()

    # Add Magik data as a line
    fig.add_trace(go.Scatter(
        x=df['time_scaled'],
        y=df['alt'],
        mode='lines',
        name='Magik',
        line=dict(color='blue')
    ))

    # Add shaded bounds between upper and lower
    fig.add_trace(go.Scatter(
        x=df['time_scaled'],
        y=interp_upper,
        line=dict(width=0),
        showlegend=False,
        hoverinfo='skip'
    ))

    fig.add_trace(go.Scatter(
        x=df['time_scaled'],
        y=interp_lower,
        fill='tonexty',
        fillcolor='rgba(150,150,150,0.2)',
        line=dict(width=0),
        hoverinfo='skip',
        name='±10% Bound'
    ))

    # Add baseline Vega and Eggtimer data
    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['height'], mode='lines', name='Cats Vega'))
    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['eggFAlt'], mode='lines', name='Eggtimer Filtered'))

    fig.update_layout(
        title=dict(text="Altitude comparison with ±10% bounds", x=0.5),
        xaxis=dict(title="Time (s)"),
        yaxis=dict(title="Height (m)")
    )

    fig.show()

make_test_graph()
