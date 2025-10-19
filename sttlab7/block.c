#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>

#define MS (157.2*5)  // Password magic number

int main() {
    //------------------ Variables -------------------
    float speed = 50;          // Game speed
    int sb1 = 700, sb2 = 100, sb3 = 1000, sb4 = 500; // Sound blocks
    int f1 = 25, f2 = 25;      // Player position
    int x = 25, y = 25, r = 12; // Circle radius
    int blx = 10, bly = 10, bls = 10, score1 = 0; // Falling block variables
    int x2 = 100, y2 = 340, rndx, rndy;
    int x3 = 200, y3 = 380, rndx3, rndy3;
    int x4 = 300, y4 = 300, rndx4, rndy4;
    int x5 = 200, y5 = 380, rndx5, rndy5;
    int gdriver = DETECT, gmode;
    int c, a, hl = 1, ttr = 0;
    int stack[5] = {383,356,329,302,275}, j = 4, n = 5, k = 0, bcol = 475;
    int stack2[12] = {383,356,329,302,275,248,221,194,167,140,113,86}, n2 = 0, k2 = 0, bcol2 = 575, j2 = 0;
    FILE *fp;
    int h;

    //------------------ Password -------------------
    initgraph(&gdriver, &gmode, "c:\\turboc3\\bgi");
    cleardevice();
    setcolor(YELLOW);
    outtextxy(15, 15, "ENTER PASSWORD...");
    gotoxy(20,2);
    scanf("%d", &h);
    outtextxy(100,30,"Matching...********");

    fp = fopen("license.exe","r");

    if(fp > 0) {
        if(h == MS) {
            //---------------- Loading Animation ----------------
            int a_load = 0, b_load = 0, c_load = 100;
            while(a_load < 600) {
                setcolor(WHITE);
                settextstyle(0,HORIZ_DIR,3);
                outtextxy(a_load,410,"Þ");
                delay(5);
                a_load++;
                if(b_load <= 100) {
                    gotoxy(38,15);
                    printf("%d", b_load);
                    if(a_load == c_load) b_load += 18;
                    else if(a_load == c_load+100) b_load += 18;
                    else if(a_load == c_load+200) b_load += 18;
                    else if(a_load == c_load+300) b_load += 18;
                    else if(a_load == c_load+400) b_load += 18;
                    else if(a_load == c_load+500) b_load += 18;
                }
            }

            //---------------- Logo Screen ----------------
            cleardevice();
            setbkcolor(0);
            settextstyle(7,HORIZ_DIR,4);
            setcolor(10);
            outtextxy(280,10,"786");
            setcolor(WHITE);
            rectangle(4,50,635,90);
            rectangle(90,95,540,135);
            rectangle(130,140,500,185);
            rectangle(430,250,620,400);
            setcolor(13);
            outtextxy(55,50,"University of Sindh Jamshoro");
            setcolor(11);
            outtextxy(120,95,"Computer Science 2K11");
            setcolor(9);
            outtextxy(155,140,"Bismiallah Struggle");
            settextstyle(8,HORIZ_DIR,2);
            setcolor(14);
            outtextxy(40,290,"further Please visit given site");
            rectangle(430,200,490,235);
            line(430,200,490,235);
            line(430,235,490,200);
            setcolor(2);
            outtextxy(40,370,"http://sindhitutorials.com/blog");
            settextstyle(1,HORIZ_DIR,1);
            setcolor(7);
            outtextxy(440,260,"Thoughts:");
            settextstyle(3,HORIZ_DIR,1);
            outtextxy(440,300,"* Life is nothing");
            outtextxy(440,330," without struggle");
            settextstyle(1,HORIZ_DIR,1);
            setcolor(100);
            outtextxy(230,430,"press any key to continue");
            getch();

            //---------------- Menu System ----------------
            initgraph(&gdriver,&gmode,"c:\\turboc3\\bgi");
            cleardevice();

            union REGS i,o;
            i.x.ax=0x0; int86(0x33,&i,&o); // mouse init
            i.x.ax=0x1; int86(0x33,&i,&o); // show pointer

            MENU_LOOP:
            cleardevice();
            settextstyle(1,HORIZ_DIR,7);
            setcolor(14);
            outtextxy(170,8,"Rapid Roll");
            outtextxy(240,350,"Game");

            int button, x_mouse, y_mouse;
            i.x.ax=0x3; int86(0x33,&i,&o);
            button = o.x.bx; x_mouse = o.x.cx; y_mouse = o.x.dx;

            if(x_mouse>250 && x_mouse<400) {
                if(y_mouse>100 && y_mouse<150 && button==1) goto GAME_INIT;
                else if(y_mouse>150 && y_mouse<200 && button==1) {
                    int ssb; settextstyle(1,HORIZ_DIR,2);
                    outtextxy(200,200,"Press 1 for Sound on");
                    outtextxy(200,100,"Press 0 for Sound off");
                    scanf("%d",&ssb);
                    if(ssb==1) goto GAME_INIT;
                    else { sb1=sb2=sb3=sb4=0; goto GAME_INIT; }
                }
                else if(y_mouse>200 && y_mouse<250 && button==1) {
                    FILE *fp_ins; char s[50];
                    fp_ins = fopen("Instructions.c","r");
                    if(fp_ins != 0) {
                        while(fgets(s,49,fp_ins) != 0) printf("%s", s);
                        getch(); fclose(fp_ins);
                    }
                    goto MENU_LOOP;
                }
                else if(y_mouse>250 && y_mouse<300 && button==1) goto LOGO_RESTART;
                else if(y_mouse>300 && y_mouse<350 && button==1) exit(0);
            }
            goto MENU_LOOP;

            //---------------- Game Initialization ----------------
            GAME_INIT:
            rndx = x2; rndy = y2;
            rndx3 = x3; rndy3 = y3;
            rndx4 = x4; rndy4 = y4;
            rndx5 = x5; rndy5 = y5;
            srand(time(NULL));
            int ic = rand()%40*bls;
            int pr = rand()%40*bls;
            blx = ic; bly = pr;
            if(bly<30 || bly>350) bly=40;
            if(blx<30 || blx>383) blx=20;

            //---------------- Main Game Loop ----------------
            while((c=getch())!='\033') { // ESC to exit
                if(c=='\115') a=1; else if(c=='\113') a=2; else a=0; // Arrow keys

                // Move Player
                if(a==1){ f1+=5; if(f1>390) f1=385; }
                else if(a==2){ f1-=5; if(f1<10) f1=20; }

                // Move Falling Blocks
                rndy -= 1; if(rndy<=10) rndy=y2;
                rndy3 -= 1; if(rndy3<=10) rndy3=y3;
                rndy4 -= 1; if(rndy4<=10) rndy4=y4;

                f2 += 1;
                if((f2==rndy-18 && (f1>=75 && f1<=125)) ||
                   (f2==rndy3-18 && (f1>=175 && f1<=225)) ||
                   (f2==rndy4-18 && (f1>=275 && f1<=325))) f2--;

                delay(speed);

                // Display Stack Circles
                for(int kk=0; kk<n-ttr; kk++){
                    setcolor(WHITE); circle(bcol,stack[kk],r);
                    setfillstyle(SOLID_FILL,GREEN); floodfill(bcol,stack[kk],WHITE);
                }

                // Collision and Score
                if(f2==383 || f2==14){
                    setcolor(BLACK); circle(bcol,stack[j],r);
                    setfillstyle(SOLID_FILL,BLACK); floodfill(bcol,stack[j],BLACK);
                    j--; sound(sb4); delay(50); nosound();
                    if(j==-1) goto GAME_OVER;
                }

                if((f2==bly || f2+11==bly || f2-11==bly) && (f1>=blx && f1<=blx+11)){
                    sound(sb3); delay(50); score1+=2; nosound();
                    if(score1%10==0){ j2++; n2=j2; sound(sb1); delay(70); nosound(); speed-=3.75; }
                    if(score1>120) goto GAME_WON;
                    ic = rand()%40*bls; pr = rand()%40*bls; blx=ic; bly=pr;
                    if(bly<30 || bly>350) bly=40; if(blx<30 || blx>383) blx=20;
                }
            }

            //---------------- Game Won ----------------
            GAME_WON:
            cleardevice(); rectangle(40,100,100,135);line(40,100,100,135);line(40,135,100,100);
            settextstyle(7,HORIZ_DIR,5); setcolor(GREEN);
            outtextxy(135,150,"Congratulation!"); outtextxy(150,200,"You won the GAME");
            settextstyle(7,HORIZ_DIR,1); setcolor(YELLOW);
            outtextxy(300,300,"By: SindhiTutorials.com"); getch(); exit(0);

            //---------------- Game Over ----------------
            GAME_OVER:
            cleardevice(); rectangle(40,100,100,135);line(40,100,100,135);line(40,135,100,100);
            settextstyle(7,HORIZ_DIR,5); setcolor(RED); outtextxy(200,200,"GAME OVER");
            settextstyle(7,HORIZ_DIR,1); setcolor(YELLOW); outtextxy(300,300,"By: SindhiTutorials.com"); getch();
            goto MENU_LOOP;
        }
    } else {
        settextstyle(7,HORIZ_DIR,3); setcolor(4); outtextxy(110,225,"SORRY YOUR PASSWORD IS INVALID");
        getch();
    }

    return 0;
}
