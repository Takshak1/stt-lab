#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

void main() {
    int gd;
    int gm;
    int maxx;
    int maxy;
    int m[500];
    int n[500];
    int con;
    int x;
    int y;
    int p;
    int q;
    int temp;
    int a;
    int i;
    int j;
    int spd;
    int level;
    int special;
    int score;
    int highscore;
    int wallX;
    int wallY;
    char ch;
    clock_t start;
    clock_t stop;
    int t;

    // ------------------ Initial Setup ------------------
    gd = DETECT;
    gm = 0;
    con = 20;
    x = 13;
    y = 14;
    a = 0;
    i = 0;
    spd = 100;
    level = 1;
    special = 0;
    score = 0;
    highscore = 0;

    initgraph(&gd,&gm,"..\\bgi");

    // ------------------ Intro ------------------
    setcolor(WHITE);
    settextstyle(3,0,6);
    outtextxy(200,2,"SNAKE 2 EXTENDED");
    settextstyle(6,0,2);
    outtextxy(20,80,"Use Arrow Keys To Move");
    outtextxy(20,140,"Avoid Hitting Yourself or Walls");
    outtextxy(20,160,"Eat Beats for Points");
    outtextxy(20,180,"Yellow Beats give Bonus");
    outtextxy(20,200,"Press P to Pause");
    outtextxy(20,220,"Press Esc to Exit");
    outtextxy(20,240,"Press Any Key to Start");
    ch = getch();
    if(ch==27) exit(0);
    cleardevice();

    // ------------------ Random Setup ------------------
    maxx = getmaxx();
    maxy = getmaxy();
    srand(time(NULL));
    p = rand()%maxx;
    temp = p%13;
    p = p-temp;
    q = rand()%maxy;
    temp = q%14;
    q = q-temp;

    wallX = maxx/2;
    wallY = maxy/2;

    start = clock();

    // ------------------ Main Loop ------------------
    while(1) {
        // Draw Beat
        setcolor(WHITE);
        if(special==1) setfillstyle(SOLID_FILL,YELLOW);
        else setfillstyle(SOLID_FILL,con+5);
        circle(p,q,5);
        floodfill(p,q,WHITE);

        // Draw Wall
        setcolor(RED);
        setfillstyle(SOLID_FILL,RED);
        rectangle(wallX-50,wallY-50,wallX+50,wallY+50);
        floodfill(wallX,wallY,RED);

        // Input
        if(kbhit()) {
            ch=getch();
            if(ch==0) ch=getch();
            if(ch==72 && a!=2) a=1;
            if(ch==80 && a!=1) a=2;
            if(ch==75 && a!=4) a=3;
            if(ch==77 && a!=3) a=4;
            if(ch==27) break;
            if(ch=='p'||ch=='P'){
                while(1){
                    if(kbhit()){
                        ch=getch();
                        if(ch=='p'||ch=='P') break;
                    }
                }
            }
        }

        // ------------------ Snake Position Update ------------------
        if(i<20){
            m[i]=x;
            n[i]=y;
            i=i+1;
        }
        else{
            for(j=con;j>=0;j--){
                m[1+j]=m[j];
                n[1+j]=n[j];
            }
            m[0]=x;
            n[0]=y;

            // Draw Snake Head
            setcolor(WHITE);
            setfillstyle(SOLID_FILL,con);
            circle(m[0],n[0],8);
            floodfill(m[0],n[0],WHITE);

            // Draw Snake Body
            for(j=1;j<con;j++){
                setfillstyle(SOLID_FILL,con+j%3);
                circle(m[j],n[j],5);
                floodfill(m[j],n[j],WHITE);
            }

            // Erase Snake
            delay(spd);
            setcolor(BLACK);
            setfillstyle(SOLID_FILL,BLACK);
            circle(m[0],n[0],8);
            floodfill(m[0],n[0],BLACK);
            circle(m[j-1],n[j-1],5);
            floodfill(m[j-1],n[j-1],BLACK);
        }

        // ------------------ Time & Score ------------------
        stop = clock();
        t = (stop-start)/CLK_TCK;
        score = con-20;
        if(score>highscore) highscore=score;
        printf("TIME %d sec  SCORE %d  LEVEL %d  HIGHSCORE %d\n",t,score,level,highscore);

        // ------------------ Collision Check ------------------
        for(j=1;j<con;j++){
            if(m[0]==m[j] && n[0]==n[j]){
                for(int k=0;k<5;k++){
                    delay(500);
                    cleardevice();
                    delay(500);
                    for(int l=0;l<=con;l++){
                        setfillstyle(SOLID_FILL,RED);
                        circle(m[l],n[l],5);
                        floodfill(m[l],n[l],WHITE);
                    }
                }
                settextstyle(3,0,4);
                outtextxy(150,150,"GAME OVER");
                getch();
                exit(0);
            }
        }

        // Collision with Wall
        if(m[0]>wallX-50 && m[0]<wallX+50 && n[0]>wallY-50 && n[0]<wallY+50){
            settextstyle(3,0,4);
            outtextxy(150,150,"HIT WALL! GAME OVER");
            getch();
            exit(0);
        }

        // Eating Beat
        if(x==p && y==q){
            if(special==1) con=con+10;
            else con=con+5;
            spd=(spd>=5)?spd-5:5;
            level=1+(con-20)/15;
            p=rand()%maxx;
            temp=p%13;
            p=p-temp;
            q=rand()%maxy;
            temp=q%14;
            q=q-temp;
            special=(rand()%5==0)?1:0;
        }

        // Move Head
        if(a==1){ y=y-14; if(y<0){ temp=maxy%14; y=maxy-temp; } }
        if(a==2){ y=y+14; if(y>maxy) y=0; }
        if(a==3){ x=x-13; if(x<0){ temp=maxx%13; x=maxx-temp; } }
        if(a==4){ x=x+13; if(x>maxx) x=0; }
        if(a==0){ y=y+14; x=x+13; }

        // Win Condition
        if(con>=490){
            for(int k=0;k<5;k++){
                for(int l=0;l<=con;l++){
                    setfillstyle(SOLID_FILL,con);
                    circle(m[l],n[l],5);
                    floodfill(m[l],n[l],WHITE);
                }
                delay(500);
                cleardevice();
                delay(500);
            }
            settextstyle(3,0,4);
            outtextxy(210,320,"YOU WIN");
            getch();
            exit(0);
        }

        // ------------------ Delay for Loop ------------------
        delay(50);
    }
}
