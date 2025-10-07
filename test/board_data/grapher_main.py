import plotly.express as px
import plotly.graph_objects as go
import pandas as pd
import numpy as np

def get_base_data():
    df_egg = pd.read_csv('test/board_data/eggtimer/dtl3.csv')
    # feet to meters
    df_egg['Alt'] = df_egg['Alt'].apply(lambda x : x*0.3048)
    df_egg['FAlt'] = df_egg['FAlt'].apply(lambda x : x*0.3048)
    df_egg['FVeloc'] = df_egg['FVeloc'].apply(lambda x : x*0.3048)   

    df_vega = pd.read_csv('test/board_data/vega/flightInfo.csv')
    df_temp = pd.read_csv('test/board_data/vega/filteredDataInfo.csv')
    df_vega['FHeight'] = df_temp['filteredAltitudeAGL']
    df_vega['FAccel'] = df_temp['filteredAcceleration']

    df_vega["eggAlt"] = df_vega['ts'].map(df_egg.set_index('T')['Alt'])
    df_vega["eggFAlt"] = df_vega['ts'].map(df_egg.set_index('T')['FAlt'])
    df_vega["eggVel"] = df_vega['ts'].map(df_egg.set_index('T')['FVeloc'])
    df_vega['LDA'] = df_vega['ts'].map(df_egg.set_index('T')['LDA'])
    df_vega['Apogee'] = df_vega['ts'].map(df_egg.set_index('T')['Apogee'])
    df_vega['N-O'] = df_vega['ts'].map(df_egg.set_index('T')['N-O'])
    df_vega['Drogue'] = df_vega['ts'].map(df_egg.set_index('T')['Drogue'])
    df_vega['Main'] = df_vega['ts'].map(df_egg.set_index('T')['Main'])
    
    for col in ['eggAlt', 'eggFAlt', 'eggVel', 'LDA', 'Apogee', 'N-O', 'Drogue', 'Main']:
        df_vega[col] = df_vega[col].interpolate(method='linear')


    df_vega['upper_alt_vega'] = df_vega['height'] + 100
    df_vega['lower_alt_vega'] = df_vega['height'] - 100
    df_vega['upper_alt_egg'] = df_vega['eggFAlt'] + 100
    df_vega['lower_alt_egg'] = df_vega['eggFAlt'] - 100


    df_vega['upper_alt_vega_percent'] = df_vega['height'] * 1.1
    df_vega['lower_alt_vega_percent'] = df_vega['height'] * 0.9
    df_vega['upper_alt_egg_percent'] = df_vega['eggFAlt'] * 1.1
    df_vega['lower_alt_egg_percent'] = df_vega['eggFAlt'] * 0.9

    print(df_vega)

    return df_vega

def get_test_data(filename):
    file = 'test/board_data/magik/' + filename + '.txt'
    f = open(file, 'r')
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
                df.loc[len(df)]= [(float((int(line[1])- start_time)/1000)),(float(line[2])),float(line[3]),float(line[4]),int(line[5][0])]
            except:
                print("Error parsing line: ", line)
                continue

    #df['time_diff'] = df['time'].diff()  # Difference between consecutive times

    #avg_time_diff = df['time_diff'].iloc[1:].mean()
    #scaling_factor = 0.03 / avg_time_diff
    #print("Scaling factor:", scaling_factor)
    df['time_scaled'] = (df['time'] / 10) - 0.75

    print(df)
    return df


def make_alt_graph(fig, base, test):
    fig.add_trace(go.Scatter(x=test['time_scaled'], y=test['alt'], mode='lines',name='Magik'))

    fig.add_trace(go.Scatter(x=base['ts'], y=base['height'],
               mode='lines',
               name='Cats Vega'))
    fig.add_trace(go.Scatter(x=base['ts'], y=base['eggFAlt'], mode='lines', connectgaps=True, name='Eggtimer'))

    fig.update_layout(
        title=dict(text="Altitude comparison with bounds", x=0.5),
        xaxis=dict(title="Time (s)"),
        yaxis=dict(title="Height (m)")
    )

    return fig

def add_bounds_meter(fig, base):

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['upper_alt_vega'],
        line=dict(width=0),
        showlegend=False,
        hoverinfo='skip'
    ))

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['lower_alt_vega'],
        fill='tonexty',
        fillcolor='rgba(200,150,150,0.2)',
        line=dict(width=0),
        hoverinfo='skip',
        name='±100m Bound - Vega'
    ))

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['upper_alt_egg'],
        line=dict(width=0),
        showlegend=False,
        hoverinfo='skip'
    ))

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['lower_alt_egg'],
        fill='tonexty',
        fillcolor='rgba(150,200,150,0.2)',
        line=dict(width=0),
        hoverinfo='skip',
        name='±100m Bound - Eggtimer'
    ))

    return fig

def add_bounds_percent(fig, base):

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['upper_alt_vega_percent'],
        line=dict(width=0),
        showlegend=False,
        hoverinfo='skip'
    ))

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['lower_alt_vega_percent'],
        fill='tonexty',
        fillcolor='rgba(200,150,150,0.2)',
        line=dict(width=0),
        hoverinfo='skip',
        name='±10 Bound - Vega'
    ))

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['upper_alt_egg_percent'],
        line=dict(width=0),
        showlegend=False,
        hoverinfo='skip'
    ))

    fig.add_trace(go.Scatter(
        x=base['ts'],
        y=base['lower_alt_egg_percent'],
        fill='tonexty',
        fillcolor='rgba(150,200,150,0.2)',
        line=dict(width=0),
        hoverinfo='skip',
        name='±10% Bound - Eggtimer'
    ))

    return fig

def add_vertical_lines(fig, df_vega, test = None):
    if test is not None:
        test['state_change'] = test['state'] != test['state'].shift()
        test['state_group'] = test['state_change'].cumsum()

    
        group_sizes = test.groupby('state_group')['state'].transform('count')

      
        test['is_stable'] = group_sizes >= 3


        test['filtered_state'] = test.apply(
            lambda row: row['state'] if row['is_stable'] else np.nan,
            axis=1
        )
        test['filtered_state'] = test['filtered_state'].ffill()

        test['filtered_change'] = test['filtered_state'].ne(test['filtered_state'].shift())

        state_changes = test[test['filtered_change']]

        print("State changes:")
        print(state_changes)
        for idx, row in state_changes.iterrows():
            fig.add_vline(
                x=row['time_scaled'],
                line=dict(color="blue", dash="dash"),
                annotation_text=f"Magik: {int(row['state'])}",
                annotation_position="top right",
                annotation=dict(font=dict(size=10, color="blue"), textangle=-90)
            )

    state_df = pd.read_csv("test/board_data/vega/flightStates.csv") 
    for idx, row in state_df.iterrows():
        fig.add_vline(
            x=row['ts'],
            line=dict(color="red", dash="dot"),
                annotation_text=f"Vega: {int(row['state'])}",
                annotation_position="top right",
                annotation=dict(font=dict(size=10, color="red"), textangle=-90)
            )

    fig.add_vline(
        x = df_vega['ts'][df_vega['LDA'].idxmax()],
        line_width = 2,
        line_dash = 'dot',
        line_color = 'green',
        annotation=dict(font=dict(size=10, color="green"), textangle=-90,  y=0.8,  
        yanchor='middle',
        align='center'),
        annotation_text = "Eggtimer LDA", 
    )
    fig.add_vline(
        x = df_vega['ts'][df_vega['Apogee'].idxmax()],
        line_width = 2,
        line_dash = 'dot',
        line_color = 'green',
        annotation_position="top left",
        annotation=dict(font=dict(size=10, color="green"), textangle=-90),
        annotation_text = "Eggtimer Apogee",  
    )   
    fig.add_vline(
        x = df_vega['ts'][df_vega['N-O'].idxmax()],
        line_width = 2,
        line_dash = 'dash',
        line_color = 'green',
        annotation=dict(font=dict(size=10, color="green"), textangle=-90,  y=0.6,  
        yanchor='middle',
        align='center'),
        annotation_text = "Eggtimer N-O",
    )
    fig.add_vline(
        x = df_vega['ts'][df_vega['Drogue'].idxmax()],
        line_width = 2,
        line_dash = 'dot',
        line_color = 'green',
        annotation=dict(font=dict(size=10, color="green"), textangle=-90,  y=0.45,  
        yanchor='middle',
        align='center'),
        annotation_text = "Eggtimer Drogue & ",
    )

    return fig

def make_vel_graph(fig, base, test):
    fig.add_trace(go.Scatter(x=test['time_scaled'], y=test['vel'], mode='lines',name='Magik'))

    fig.add_trace(go.Scatter(x=base['ts'], y=base['velocity'],
               mode='lines',
               name='Cats Vega'))
    fig.add_trace(go.Scatter(x=base['ts'], y=base['eggVel'], mode='lines', connectgaps=True, name='Eggtimer'))
    x_axis = "Speed (m/s)"
    title = "Velocity comparison"

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

    return fig

def make_accel_graph(fig, df_vega, df_egg):
    fig.add_trace(go.Scatter(x=df_egg['time_scaled'], y=df_egg['acc'], mode='lines',name='Magik'))

    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['acceleration'],
               mode='lines',
               name='Cats Vega'))

    fig.update_layout(
        title=dict(text="Acceleration comparison", x=0.5),
        xaxis=dict(title="Time (s)"),
        yaxis=dict(title="Acceleration (m/s²)")
    )

    return fig

def make_cots_vel(fig, base):
    fig.add_trace(go.Scatter(x=base['ts'], y=base['velocity'],
               mode='lines',
               name='Vega'))
    fig.add_trace(go.Scatter(x=base['ts'], y=base['eggVel'], mode='lines', connectgaps=True, name='Eggtimer'))


    fig.update_layout(
        title=dict(text="COTS Velocity comparison", x=0.5),
        xaxis=dict(title="Time (s)"),
        yaxis=dict(title="Height (m)")
    )

    return fig



def make_cots_alt(fig, base):
    fig.add_trace(go.Scatter(x=base['ts'], y=base['height'],
               mode='lines',
               name='Vega'))
    fig.add_trace(go.Scatter(x=base['ts'], y=base['eggFAlt'], mode='lines', connectgaps=True, name='Eggtimer Filtered'))
    fig.add_trace(go.Scatter(x=base['ts'], y=base['FHeight'], mode='lines', connectgaps=True, name='Vega Filtered'))
    fig.add_trace(go.Scatter(x=base['ts'], y=base['eggAlt'], mode='lines', connectgaps=True, name='Eggtimer'))

    fig.update_layout(
        title=dict(text="COTS Altitude comparison", x=0.5),
        xaxis=dict(title="Time (s)"),
        yaxis=dict(title="Height (m)")
    )

    return fig

def make_alt_graph_with_animation(base, test):

    test = test.sort_values('time_scaled')
    base = base.sort_values('ts')

    base = pd.merge_asof(base, test, left_on='ts', right_on='time_scaled', direction='nearest')



    base.index = base['ts']
   

    min_time = max(test['time_scaled'].min(), base['ts'].min())
    max_time = min(test['time_scaled'].max(), base['ts'].max())

    

    frames = []
    for t in range(int(min_time*100), int(max_time*100), 1):  
        alt_magik = np.interp(t, test['time_scaled'], test['alt'])
        alt_vega = np.interp(t, base['ts'], base['height'])
        alt_egg = np.interp(t, base['ts'], base['eggFAlt'])

        frames.append(go.Frame(
            data=[
                go.Scatter(x=[base['ts'][t/100]], y=[base['alt'][t/100]], mode='markers+text',
                           marker=dict(color='blue', size=8),
                           text=[f"Time: {t:.2f}s<br>Magik: {test['alt'][t/100]:.1f}m"],
                           textposition="top right",
                           showlegend=False),
                go.Scatter(x=[base['ts'][t/100]], y=[base['eggFAlt'][t/100]], mode='markers+text',
                           marker=dict(color='green', size=8),
                           text=[f"Eggtimer: {base['eggFAlt'][t/100]:.1f}m"],
                           textposition="middle right",
                           showlegend=False),
                go.Scatter(x=[base['ts'][t/100]], y=[base['height'][t/100]], mode='markers+text',
                           marker=dict(color='red', size=8),
                           text=[f"Vega: {base['height'][t/100]:.1f}m"],
                           textposition="bottom right",
                           showlegend=False),
                go.Scatter(x=[t/100, t/100], y=[0, base['height'].max()],
                           mode='lines',
                           line=dict(color='black', dash='dash'),
                           showlegend=False)
            ],
            name=f'{t:.2f}'
        ))

    layout = go.Layout(
        title=dict(text="Altitude Comparison with Animation", x=0.5),
        xaxis=dict(title="Time (s)", range=[min_time, max_time]),
        yaxis=dict(title="Height (m)"),
        updatemenus=[dict(
            type="buttons",
            showactive=False,
            buttons=[dict(label="Play", method="animate", args=[None, {
                "frame": {"duration": 50, "redraw": True},
                "fromcurrent": True,
                "transition": {"duration": 0}
            }])]
        )],
        sliders=[dict(
            steps=[dict(method="animate",
                        args=[[f'{t:.2f}'], dict(mode="immediate", frame=dict(duration=0), transition=dict(duration=0))],
                        label=f"{t:.1f}s")
                   for t in range(int(min_time*100), int(max_time*100), 5)],
            active=0,
            x=0.1,
            xanchor="left",
            y=0,
            yanchor="top"
        )]
    )

    fig = go.Figure( layout=layout, frames=frames)
    return fig


if __name__ == "__main__":
    fig = go.Figure()
    df_vega = get_base_data()
    df_egg = get_test_data('6')
    #fig = make_accel_graph(fig, df_vega, df_egg)
    fig = make_vel_graph(fig, df_vega, df_egg)
   
    #fig = make_cots_alt(fig, df_vega)
    #fig = make_cots_vel(fig, df_vega)
    
    #fig = make_alt_graph_with_animation(df_vega, df_egg)
    fig = add_vertical_lines(fig, df_vega, df_egg)
    #fig = make_alt_graph(fig, df_vega, df_egg)
    #fig = add_bounds_meter(fig, df_vega)
    #fig = add_bounds_percent(fig, df_vega)
    fig.show()