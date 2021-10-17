#include <iostream>
#include<allegro.h>
#include<cmath>

using namespace std;

void pintar_nave(float cx, float cy,BITMAP *buffer);
void mover_nave(float &cx, float &cy,float &vx, float &vy);
void rotar(float &x, float &y, float cx ,float cy , float da);
void aceleracion(float da, float &vx, float &vy);
void pintar_motor(float da, float cx, float cy, BITMAP *buffer);


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
        mover_nave(cx,cy,vx,vy);

        if(key[KEY_UP]){
            aceleracion(0,vx,vy); // Cuando el angulo = 0, vamos a tener una aceleracion hacia arriba
            pintar_motor(0,cx,cy,buffer);
        }
        if(key[KEY_RIGHT]){
            aceleracion(-90,vx,vy);
            pintar_motor(-90,cx,cy,buffer);
        }
        if(key[KEY_LEFT]){
            aceleracion(90,vx,vy);
            pintar_motor(90,cx,cy,buffer);
        }

        pintar_nave(cx,cy,buffer);
        blit(buffer,screen,0,0,0,0,740,500);


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
    ax = 0;
    ay = 0.1;

    vx += ax;
    vy += ay;

    cx +=vx;
    cy +=vy;


}

void rotar(float &x, float &y, float cx ,float cy , float da){
    float dx = x - cx;
    float dy = y - cy;
    float r = sqrt(dx*dx + dy*dy);
    float a = atan2(dy,dx);
    float da_rad = da / 180 * M_PI;

    a -= da_rad;

    x = cx + r*cos(a);
    y = cy + r*sin(a);
}

void aceleracion(float da, float &vx, float &vy){
    float ax = 0;
    float ay = -0.15;
    rotar(ax,ay,0,0,da); // Si da = 0 el vector aceleracion no se modifica
    vx += ax;
    vy += ay;
}

void pintar_motor(float da, float cx, float cy, BITMAP *buffer){
    float c1,c2;
    c1 = cx; c2 =cy;
    if(da != 0) c2 += 9;
    float fuego[14] = {c1-5,c2+5   ,c1-10,c2+20   ,c1-5,c2+20   ,c1,c2+35   ,c1+5,c2+20   ,c1+10,c2+20   ,c1+5,c2+5 };

    for(int i =0;i<=12 ; i+=2){
        rotar(fuego[i],fuego[i+1],cx,cy,da);
    }
    for(int i =0;i<=10 ; i+=2){
        line(buffer, fuego[i],fuego[i+1],fuego[i+2],fuego[i+3],0x999999);
    }

}
