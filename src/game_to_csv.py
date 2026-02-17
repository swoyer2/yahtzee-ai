import pandas as pd
from pathlib import Path

versions = ["v1", "v2", "v3", "v4", "v5"]

for version in versions:
    dir_path = Path(f"../{version}")
    df = pd.DataFrame(columns=['final_score',
                               'aces',
                               'twos',
                               'threes',
                               'fours',
                               'fives',
                               'sixes',
                               'three_of_a_kind',
                               'four_of_a_kind',
                               'full_house',
                               'sm_straight',
                               'lg_straight',
                               'chance',
                               'yahtzee',
                               ])
    for file_path in dir_path.glob("*.txt"):
        if str(file_path) == f"../{version}/info.txt":
            continue
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
            cards = content.split("0 -> A")
            last_card = cards[-1].split(' ')

            final_score = int(last_card[-2].partition("\n")[0])
            aces = int(last_card[1].partition("\n")[0])
            twos = int(last_card[4].partition("\n")[0])
            threes = int(last_card[7].partition("\n")[0])
            fours = int(last_card[10].partition("\n")[0])
            fives = int(last_card[13].partition("\n")[0])
            sixes = int(last_card[16].partition("\n")[0])
            three_of_a_kind = int(last_card[22].partition("\n")[0])
            four_of_a_kind = int(last_card[28].partition("\n")[0])
            full_house = int(last_card[32].partition("\n")[0])
            sm_straight = int(last_card[36].partition("\n")[0])
            lg_straight = int(last_card[40].partition("\n")[0])
            chance = int(last_card[43].partition("\n")[0])
            yahtzee = int(last_card[46].partition("\n")[0])
            
            df.loc[len(df)] = [final_score, aces, twos, threes, fours, fives, sixes,
                               three_of_a_kind, four_of_a_kind, full_house, sm_straight,
                               lg_straight, chance, yahtzee]
            
        df.to_csv(f'../{version}/data.csv')
