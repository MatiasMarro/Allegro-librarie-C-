#include <iostream>
#include<allegro.h>

using namespace std;

void pintar_nave(float cx, float cy,BITMAP *buffer);
void mover_nave(float &cx, float &cy,float &vx, float &vy);


int main()
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 740 ,500, 0, 0);
    BITMAP *buffer = create_bitmap(740, 500);

    float cx,cy;
    float vx,vy;
    cx=100;cy=100;


    while(!key[KEY_ESC]){
        clear_to_color(buffer, 0x000000);
        pintar_nave(cx,cy,buffer);
        blit(buffer,screen,0,0,0,0,740,500);
        mover_nave(cx,cy,vx,vy);
        rest(20);


    }


    return 0;
}
END_OF_MAIN();



void pintar_nave(float cx, float cy,BITMAP *buffer){
   /* line(buffer, cx-20, cy+20,cx-20,cy+10, 0xeaeae6);
    line(buffer, cx-20, cy+20,cx-20,cy+10, 0xeaeae6);
    line(buffer, cx-20, cy+10,cx-10,cy, 0xeaeae6);
    line(buffer, cx-10, cy,cx-10,cy-10, 0xeaeae6);
    line(buffer, cx-10, cy-10,cx,cy-15, 0xeaeae6);
    line(buffer, cx, cy-15,cx+10,cy-10, 0xeaeae6);
    line(buffer, cx+10, cy-10,cx+10,cy, 0xeaeae6);
    line(buffer, cx+10, cy,cx+20,cy+10, 0xeaeae6);
    line(buffer, cx+20, cy+10,cx+20,cy+20, 0xeaeae6);
    line(buffer, cx-10, cy,cx+10,cy, 0xeaeae6);
                    UNA FORMA DE PINTAR LA NAVE */
    float nav[26] =
    {cx-20,cy+20,    cx-20,cy+10,     cx-10,cy,   cx-10,cy-10,    cx,cy-15,      cx+10,cy-10,
     cx+10,cy,       cx+20,cy+10,       cx+20,cy+20,       cx-10,cy,    cx+10,cy};

     for(int i=0; i<=14;i+=2){
        line(buffer,nav[i],nav[i+1],nav[i+2],nav[i+3],0x999999);
        if(i == 14) line (buffer,nav[i+4],nav[i+5],nav[i+6],nav[i+7],0x999999);
     }

}

void mover_nave(float &cx, float &cy,float &vx, float &vy){
    float ax,ay;
    ax=0;
    ay = 0.05;

    vx += ax;
    vy += ay;

    cx +=vx;
    cy +=vy;


}
