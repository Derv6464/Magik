import plotly.express as px
import plotly.graph_objects as go
import pandas as pd

def get_baseline_data():
    df_egg = pd.read_csv('test/board_data/eggtimer/dtl3.csv')
    # feet to meters
    df_egg['Alt'] = df_egg['Alt'].apply(lambda x : x*0.3048)
    df_egg['FAlt'] = df_egg['FAlt'].apply(lambda x : x*0.3048)

    df_vega = pd.read_csv('test/board_data/vega/flightInfo.csv')

    df_vega["eggAlt"] = df_vega['ts'].map(df_egg.set_index('T')['Alt'])
    df_vega["eggFAlt"] = df_vega['ts'].map(df_egg.set_index('T')['FAlt'])


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
    f = open('test/board_data/magik/1.txt', 'r')
    #data ponts were read in 0.5s appart (est)
    lines = f.readlines()
    f.close()
    df = pd.DataFrame(columns=['time', 'alt', 'vel', 'acc', 'state'])
    state = 0
    start_time = int(lines[1].split(' ')[1])
    for line in lines:
        if 'data' in line:
            line = line.split(' ')
            if len(line) != 6:
                continue
            try:
                df.loc[len(df)]= [float((int(line[1])- start_time)/10000),float(line[2]),float(line[3]),float(line[4]),int(line[5][0])]
            except:
                print("Error parsing line: ", line)
                continue

    print(df)
    return df

def make_test_graph():
    df = get_test_data()
    df_vega = get_baseline_data()
    fig = go.Figure()
    title = ""
    x_axis = ""
    fig.add_trace(go.Scatter(x=df['time'], y=df['alt'], mode='lines',name='Magik'))

    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['height'],
            mode='lines',
            name='Cats Vega'))
    fig.add_trace(go.Scatter(x=df_vega['ts'], y=df_vega['eggAlt'], mode='lines', connectgaps=True, name='Eggtimer not filered'))
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


make_test_graph()
