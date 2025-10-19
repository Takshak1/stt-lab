| Definition ID | Variable | Block | Line | Statement |
|---------------|----------|-------|------|-----------|
| D1 | gd | B0 | 8 | `int gd;` |
| D2 | gm | B0 | 9 | `int gm;` |
| D3 | maxx | B0 | 10 | `int maxx;` |
| D4 | maxy | B0 | 11 | `int maxy;` |
| D5 | m | B0 | 12 | `int m[500];` |
| D6 | n | B0 | 13 | `int n[500];` |
| D7 | con | B0 | 14 | `int con;` |
| D8 | x | B0 | 15 | `int x;` |
| D9 | y | B0 | 16 | `int y;` |
| D10 | p | B0 | 17 | `int p;` |
| D11 | q | B0 | 18 | `int q;` |
| D12 | temp | B0 | 19 | `int temp;` |
| D13 | a | B0 | 20 | `int a;` |
| D14 | i | B0 | 21 | `int i;` |
| D15 | j | B0 | 22 | `int j;` |
| D16 | spd | B0 | 23 | `int spd;` |
| D17 | level | B0 | 24 | `int level;` |
| D18 | special | B0 | 25 | `int special;` |
| D19 | score | B0 | 26 | `int score;` |
| D20 | highscore | B0 | 27 | `int highscore;` |
| D21 | wallX | B0 | 28 | `int wallX;` |
| D22 | wallY | B0 | 29 | `int wallY;` |
| D23 | ch | B0 | 30 | `char ch;` |
| D24 | t | B0 | 33 | `int t;` |
| D25 | gd | B0 | 36 | `gd = DETECT;` |
| D26 | gm | B0 | 37 | `gm = 0;` |
| D27 | con | B0 | 38 | `con = 20;` |
| D28 | x | B0 | 39 | `x = 13;` |
| D29 | y | B0 | 40 | `y = 14;` |
| D30 | a | B0 | 41 | `a = 0;` |
| D31 | i | B0 | 42 | `i = 0;` |
| D32 | spd | B0 | 43 | `spd = 100;` |
| D33 | level | B0 | 44 | `level = 1;` |
| D34 | special | B0 | 45 | `special = 0;` |
| D35 | score | B0 | 46 | `score = 0;` |
| D36 | highscore | B0 | 47 | `highscore = 0;` |
| D37 | ch | B0 | 63 | `ch = getch();` |
| D38 | maxx | B2 | 68 | `maxx = getmaxx();` |
| D39 | maxy | B2 | 69 | `maxy = getmaxy();` |
| D40 | p | B2 | 71 | `p = rand()%maxx;` |
| D41 | temp | B2 | 72 | `temp = p%13;` |
| D42 | p | B2 | 73 | `p = p-temp;` |
| D43 | q | B2 | 74 | `q = rand()%maxy;` |
| D44 | temp | B2 | 75 | `temp = q%14;` |
| D45 | q | B2 | 76 | `q = q-temp;` |
| D46 | wallX | B2 | 78 | `wallX = maxx/2;` |
| D47 | wallY | B2 | 79 | `wallY = maxy/2;` |
| D48 | start | B2 | 81 | `start = clock();` |
| D49 | ch | B9 | 100 | `ch=getch();` |
| D50 | ch | B19 | 110 | `ch=getch();` |
| D51 | i | B22 | 121 | `i=i+1;` |
| D52 | stop | B27 | 155 | `stop = clock();` |
| D53 | t | B27 | 156 | `t = (stop-start)/CLK_TCK;` |
| D54 | score | B27 | 157 | `score = con-20;` |
| D55 | spd | B41 | 193 | `spd=(spd>=5)?spd-5:5;` |
| D56 | level | B41 | 194 | `level=1+(con-20)/15;` |
| D57 | p | B41 | 195 | `p=rand()%maxx;` |
| D58 | temp | B41 | 196 | `temp=p%13;` |
| D59 | p | B41 | 197 | `p=p-temp;` |
| D60 | q | B41 | 198 | `q=rand()%maxy;` |
| D61 | temp | B41 | 199 | `temp=q%14;` |
| D62 | q | B41 | 200 | `q=q-temp;` |
| D63 | special | B41 | 201 | `special=(rand()%5==0)?1:0;` |