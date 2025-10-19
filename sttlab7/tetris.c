#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void main() {
     int gd;
    int gm;
    gd = DETECT;
    int arr[26][13];
    int i;
    int j;
    int k;
    int score;
    score = 0;
    int spd;
    spd = 100;
    int x;
    int y;
    int rotation;
    int shapeType;
    int lx[4];
    int ly[4];
    int nextShape;
    int nextRot;
    int prevX[4];
    int prevY[4];
    int tempX;
    int tempY;
    int tempLX;
    int tempLY;
    int lineCount;
    lineCount = 0;
    int level;
    level = 1;
    int levelScore;
    levelScore = 0;
    int tempScore;
    int tempLine;
    int holdShape;
    holdShape = -1;
    int holdRot;
    holdRot = 0;
    int canHold;
    canHold = 1;
    int ghostLX[4];
    int ghostLY[4];
    int linesClearedAtOnce;
    int multiplier;
    char play;
    play = 'y';
    char ch;

    initgraph(&gd, &gm, "d:\\tc\\bgi");

    while(play == 'y') {

        for(i = 0; i < 26; i++) {
            for(j = 0; j < 13; j++) {
                arr[i][j] = 0;
            }
        }

        for(i = 0; i < 26; i++) {
            arr[i][0] = 1;
        }

        for(i = 0; i < 26; i++) {
            arr[i][12] = 1;
        }

        for(j = 0; j < 13; j++) {
            arr[25][j] = 1;
        }

        score = 0;
        spd = 100;
        lineCount = 0;
        level = 1;
        levelScore = 0;
        canHold = 1;

        cleardevice();

        settextstyle(2,0,6);

        outtextxy(200,100,"TETRIS GAME");
        outtextxy(100,200,"Select Level:");
        outtextxy(100,250,"1. Easy");
        outtextxy(100,300,"2. Medium");
        outtextxy(100,350,"3. Hard");

        ch = getch();

        if(ch == '1') {
            spd = 200;
        }

        if(ch == '2') {
            spd = 100;
        }

        if(ch == '3') {
            spd = 50;
        }

        shapeType = rand() % 7;
        rotation = 0;
        nextShape = rand() % 7;
        nextRot = 0;

        while(1) {
            x = 5;
            y = 0;

            for(i = 0; i < 4; i++) {
                prevX[i] = lx[i];
            }

            for(i = 0; i < 4; i++) {
                prevY[i] = ly[i];
            }

            // Block coordinates based on shape
            if(shapeType == 0) {
                if(rotation % 2 == 0) {
                    lx[0] = x;
                    lx[1] = x;
                    lx[2] = x;
                    lx[3] = x;
                    ly[0] = y;
                    ly[1] = y + 1;
                    ly[2] = y + 2;
                    ly[3] = y + 3;
                } else {
                    lx[0] = x - 1;
                    lx[1] = x;
                    lx[2] = x + 1;
                    lx[3] = x + 2;
                    ly[0] = y;
                    ly[1] = y;
                    ly[2] = y;
                    ly[3] = y;
                }
            }

            if(shapeType == 1) {
                lx[0] = x;
            }

            lx[1] = x + 1;
            lx[2] = x;
            lx[3] = x + 1;
            ly[0] = y;
            ly[1] = y;
            ly[2] = y + 1;
            ly[3] = y + 1;
            int landed = 0;
            while(!landed){
                for(i=0;i<4;i++){ setcolor(WHITE); setfillstyle(SOLID_FILL,6); rectangle(180+lx[i]*15,90+ly[i]*15,194+lx[i]*15,104+ly[i]*15); floodfill(181+lx[i]*15,91+ly[i]*15,WHITE); }
                delay(spd);
                for(i=0;i<4;i++){ setcolor(BLACK); setfillstyle(SOLID_FILL,BLACK); rectangle(180+lx[i]*15,90+ly[i]*15,194+lx[i]*15,104+ly[i]*15); floodfill(181+lx[i]*15,91+ly[i]*15,BLACK); }
                for(i=0;i<4;i++){ if(arr[ly[i]+1][lx[i]]==1) landed=1; }
                if(!landed) for(i=0;i<4;i++) ly[i]++;
                if(kbhit()){
                    char c=getch();
                    if(c==0) c=getch();
                    if(c==75){ int canMove=1; for(i=0;i<4;i++) if(arr[ly[i]][lx[i]-1]==1) canMove=0; if(canMove) for(i=0;i<4;i++) lx[i]--; }
                    if(c==77){ int canMove=1; for(i=0;i<4;i++) if(arr[ly[i]][lx[i]+1]==1) canMove=0; if(canMove) for(i=0;i<4;i++) lx[i]++; }
                    if(c==32){ rotation=(rotation+1)%4; break; }
                    if(c==27) exit(0);
                    if(c=='c' && canHold==1){ int temp=shapeType; shapeType=holdShape; holdShape=temp; rotation=holdRot; holdRot=rotation; canHold=0; }
                }
            }

            for(i=0;i<4;i++) arr[ly[i]][lx[i]]=1;

            int linesClearedAtOnce=0;
            for(i=24;i>=0;i--){
                int full=1;
                for(j=1;j<12;j++) if(arr[i][j]==0) full=0;
                if(full){ linesClearedAtOnce++; score+=100; levelScore+=100; lineCount++; for(k=i;k>0;k--) for(j=1;j<12;j++) arr[k][j]=arr[k-1][j]; for(j=1;j<12;j++) arr[0][j]=0; i++; }
            }

            int multiplier=1;
            if(linesClearedAtOnce==2) multiplier=2;
            if(linesClearedAtOnce==3) multiplier=3;
            if(linesClearedAtOnce>=4) multiplier=5;
            score = score*multiplier;

            if(levelScore>=500){ spd-=10; level++; levelScore=0; }

            char sc[10], lv[10], ln[10];
            sprintf(sc,"Score:%d",score);
            sprintf(lv,"Level:%d",level);
            sprintf(ln,"Lines:%d",lineCount);
            setcolor(WHITE);
            settextstyle(3,0,2);
            outtextxy(50,50,sc);
            outtextxy(50,70,lv);
            outtextxy(50,90,ln);

            int over=0;
            for(i=1;i<12;i++) if(arr[0][i]==1) over=1;
            if(over){ settextstyle(3,0,4); outtextxy(200,200,"GAME OVER"); outtextxy(200,250,"Press Y to play again"); ch=getch(); if(ch!='y') exit(0); break; }

            shapeType=nextShape;
            rotation=nextRot;
            nextShape=rand()%7;
            nextRot=0;
            canHold=1;
        }
    }

    closegraph();
}
