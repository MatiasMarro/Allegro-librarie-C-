#include <iostream>
#include<allegro.h>
#include<cmath>

using namespace std;

void pintar_nave(float cx, float cy,BITMAP *buffer);
void mover_nave(float &cx, float &cy,float &vx, float &vy);
void rotar(float &x, float &y, float cx ,float cy , float da);
void aceleracion(float da, float &vx, float &vy);
void pintar_motor(float da, float cx, float cy, BITMAP *buffer);
void medidor_combustible(bool gastar_combustible,float &combustible,BITMAP *buffer);

void pintar_nivel(int num_nivel, BITMAP *buffer );
void Explocion(float cx, float cy, BITMAP *buffer,int num_nivel);
bool Game_Over(float &cx, float &cy,float &vx,float &vy,int &num_nivel,float &combustible, BITMAP *buffer);
bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel);
bool choque_triangulo(float x1, float y1, float x2, float y2, float p1x, float p1y, float p2x, float p2y, string tipo_triangulo);
bool choque_nave(int num_nivel, float cx, float cy);
void avanzar_nivel(float &cx,float &cy, float &vx, float &vy, int &num_nivel, float &combustible,BITMAP *buffer);
void reiniciar_nivel(float &cx,float &cy,float &vx,float &vy, float &combustible);


int main()
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 740 ,500, 0, 0);
    BITMAP *buffer = create_bitmap(740, 500);

    float cx,cy;
    float vx,vy;
    cx=710;cy=100; vy=-2;
    float combustible = 100; // 100 significa tanque lleno
    int num_nivel = 3;


    while(!key[KEY_ESC] && !Game_Over(cx,cy,vx,vy,num_nivel,combustible,buffer)){
        bool gastar_combustible = false;
        clear_to_color(buffer, 0x000000);
        pintar_nivel(num_nivel,buffer);
        mover_nave(cx,cy,vx,vy);

        if(key[KEY_UP] && combustible > 0){
            aceleracion(0,vx,vy); // Cuando el angulo = 0, vamos a tener una aceleracion hacia arriba
            pintar_motor(0,cx,cy,buffer);
            gastar_combustible = true;
        }
        if(key[KEY_RIGHT] && combustible > 0){
            aceleracion(-90,vx,vy);
            pintar_motor(-90,cx,cy,buffer);
            gastar_combustible = true;
        }
        if(key[KEY_LEFT] && combustible > 0 ){
            aceleracion(90,vx,vy);
            pintar_motor(90,cx,cy,buffer);
            gastar_combustible = true;
        }



        medidor_combustible(gastar_combustible,combustible,buffer);
        pintar_nave(cx,cy,buffer);
        blit(buffer,screen,0,0,0,0,740,500);
        avanzar_nivel(cx,cy,vx,vy,num_nivel,combustible,buffer);


        rest(15);


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
    ay = 0.098;

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

void medidor_combustible(bool gastar_combustible,float &combustible,BITMAP *buffer){
    textout_centre_ex(buffer,font,"combostible",100,30, 0x999999, 0x00000);
    rectfill(buffer,50,50,50+combustible,55,0x999999);
    if(gastar_combustible == true){
        combustible -=0.1;
    }
}

void pintar_nivel(int num_nivel, BITMAP *buffer ){
    if(num_nivel == 1){
        rectfill(buffer, 10,450,100,500,0xEFEF00);
    }
    if(num_nivel == 2){
        triangle(buffer, 110, 100, 300, 500, 110 , 500 ,0x999999);
        triangle(buffer, 500, 500, 600, 300, 600 , 500 ,0x999999);
        triangle(buffer, 600, 300, 800, 600, 600 , 500 ,0x999999);
        triangle(buffer, 200,   0, 400, 350, 400 ,   0 ,0x999999);
        rectfill(buffer, 10, 450, 100, 500,      0xEFEF00);
    }
    if(num_nivel == 3){
        triangle(buffer, 110, 300, 300, 500, 110 , 500 ,0x999999);
        triangle(buffer, 305, 300, 510, 500, 310 , 500 ,0x999999);
        triangle(buffer, 400,   0, 600, 400, 600 ,   0 ,0x999999);
        triangle(buffer, 100,   0, 250, 200, 250 ,   0 ,0x999999);
        rectfill(buffer, 10, 450, 100, 500,      0xEFEF00);

    }


}

void Explocion(float cx, float cy, BITMAP *buffer,int num_nivel){
    float x[12]={cx-10, cx+10, cx   , cx   , cx+15, cx-15, cx+5, cx-10, cx+10, cx-5,cx-10,cx+10};
    float y[12]={cy   , cy   , cy-15,cy+15 ,cy-15 ,cy+15 , cy+5, cy-10, cy-10,cy+10,cy   ,cy};

    float dx[6]={7,     7,      0,      -7,     -7,     0};
    float dy[6]={0,     -7,     -7,     -7,     0,      7};



    clear(screen);
    do{
        clear(buffer);
        pintar_nivel(num_nivel,buffer);
        int j=0;
        for(int i=0;i<=10;i+=2){
            line(buffer , x[i]  , y[i], x[i+1], y[i+1], 0xFFFFFF);
            rotar(x[i+1],y[i+1] , x[i], y[i]  ,  15);

            x[i]+=dx[j];
            y[i]+=dy[j];
            x[i+1]+=dx[j];
            y[i+1]+=dy[j];
            j++;
        }

        textout_centre_ex(buffer,font,"PRESS (A) FOR TRY AGAIN",370,240,0xFBFF00,0x000000);
        textout_centre_ex(buffer,font,"PRESS (ESC) TO EXIT",370,250,0xFBFF00,0x000000);
        blit(buffer,screen,0,0,0,0,740,500);
        rest(25);

    }while(!key[KEY_ESC] && !key[KEY_A]);
}


bool Game_Over(float &cx, float &cy,float &vx,float &vy,int &num_nivel,float &combustible, BITMAP *buffer){
    if(cx+20 >= 740 || cx-20 <= 0 || cy-20 <=0 || cy+15 <=0 || cy+20 >= 500){

        Explocion(cx,cy,buffer,num_nivel);
        reiniciar_nivel(cx,cy,vx,vy,combustible);
        //return true;
    }
   else if(choque_nave(num_nivel,cx,cy) == true){
        Explocion(cx,cy,buffer,num_nivel);
        reiniciar_nivel(cx,cy,vx,vy,combustible);
        //return true;
    }

    else if(cy+20 >=450 && cx-20 >=10 && cx+20 <=100 && vy > 1.5){
        Explocion(cx,cy,buffer,num_nivel);
        reiniciar_nivel(cx,cy,vx,vy,combustible);
    }
    else if(cx-20 <= 100 && 100<=cx+20 && cy+20 >=450 ){
            Explocion(cx,cy,buffer,num_nivel);
        reiniciar_nivel(cx,cy,vx,vy,combustible);
    }
    return false;
}


bool aterrizar(float cx, float cy, float vx, float vy, BITMAP *buffer, int num_nivel){
    if(cy+20 >=450 && cx-20 >=10 && cx+20 <=100 ){
        if(vy<=1.5) {
                return  true;
        }else {

        }
    }
    return false;
}


bool choque_triangulo(float x1, float y1, float x2, float y2, float p1x, float p1y, float p2x, float p2y, string tipo_triangulo ){
    float m= (y2-y1)/(x2-x1); // Si m>0 triangulo derecho, si m<0 trianglo espejado
    float b = y1-m*x1; // Ecuacion de la recta
if(tipo_triangulo == "abajo"){
    if(m >0 ){
        if(x1 <= p1x && p1x <= x2){
            if(p2y >= m*p1x + b ) return true;
            }
            else{
                if(p1x <=x1 && x1 <= p2x)
                    if( y1 <=p2y) return true;
            }
    }
    else if(m <0 ){
        if(x1 <= p2x && p2x <= x2){
            if(p2y >= m*p2x + b ) return true;
            }
            else{
                if(p1x <=x2 && x2 <= p2x)
                    if( y2 <= p2y) return true;
            }
    }
    }

    if(tipo_triangulo == "arriba"){
        if(m>0){
        if(x1 <= p2x && p2x <= x2){
                if(p1y <= m*p2x+b) return true;
            }
            else{
                if(p1x <= x2 && x2 <= p2x)
                if(p1y <=y2) return true;
            }
            return false;
    }
    else{
        if(x1 <=p1x && p1x <= x2){
            if(p1y <= m*p1x+b) return true;
        }
        else{
            if(p1x <=x1 && x1<= p2x)
            if(p1y <=y1) return true;
        }
        return false;
        }
    }
    return false;
}




bool choque_nave(int num_nivel, float cx, float cy){

    //pata izquierda de la nave
    float r1x =cx-20 , r1y= cy ;
    float r2x=cx-10  , r2y= cy+20 ;
    //pata derecha de la nave
    float p1x =cx+10 , p1y= cy ;
    float p2x=cx+20  , p2y= cy+20 ;
    //cuerpo de la nave
    float q1x =cx-10 , q1y= cy-15 ;
    float q2x=cx+10  , q2y= cy ;

    if(num_nivel == 2){
        if(choque_triangulo(110,100,300,500,r1x,r1y,r2x,r2y,"abajo") == true || choque_triangulo(110,100,300,500,p1x,p1y,p2x,p2y,"abajo")== true || choque_triangulo(110,100,300,500,q1x,q1y,q2x,q2y,"abajo")== true) return true;
        else if(choque_triangulo(500,500,600,300,r1x,r1y,r2x,r2y,"abajo") == true || choque_triangulo(500,500,600,300,p1x,p1y,p2x,p2y,"abajo")== true || choque_triangulo(500,500,600,300,q1x,q1y,q2x,q2y,"abajo")== true) return true;
        else if(choque_triangulo(600,300,800,500,r1x,r1y,r2x,r2y,"abajo") == true || choque_triangulo(600,300,800,500,p1x,p1y,p2x,p2y,"abajo")== true || choque_triangulo(600,300,800,300,q1x,q1y,q2x,q2y,"abajo")== true) return true;
        else if(choque_triangulo(200,0  ,400,350,r1x,r1y,r2x,r2y,"arriba") == true || choque_triangulo(200,0  ,400,350,p1x,p1y,p2x,p2y,"arriba")== true || choque_triangulo(200,0  ,400,350,q1x,q1y,q2x,q2y,"arriba")== true) return true;

    }
    else if(num_nivel == 3){
        if(choque_triangulo(110,100,300,500,r1x,r1y,r2x,r2y,"abajo")  || choque_triangulo(110,100,300,500,p1x,p1y,p2x,p2y,"abajo") || choque_triangulo(110,100,300,500,q1x,q1y,q2x,q2y,"abajo")) return true;
        else if(choque_triangulo(305, 500, 510, 500,r1x,r1y,r2x,r2y,"abajo") == true || choque_triangulo(305, 500, 510, 500,p1x,p1y,p2x,p2y,"abajo")== true || choque_triangulo(305, 500, 510, 500,q1x,q1y,q2x,q2y,"abajo")== true) return true;
        else if(choque_triangulo(400,0,600,400,r1x,r1y,r2x,r2y,"arriba") == true || choque_triangulo(400,0,600,400,p1x,p1y,p2x,p2y,"arriba")== true || choque_triangulo(400,0,600,400,q1x,q1y,q2x,q2y,"arriba")== true) return true;
        else if(choque_triangulo(100,0  ,250,200,r1x,r1y,r2x,r2y,"arriba") == true || choque_triangulo(100,0  ,250,200,p1x,p1y,p2x,p2y,"arriba")== true || choque_triangulo(100,0  ,250,200,q1x,q1y,q2x,q2y,"arriba")== true) return true;

    }
    return false;

}




void avanzar_nivel(float &cx,float &cy, float &vx, float &vy, int &num_nivel, float &combustible,BITMAP *buffer){
    if(aterrizar(cx,cy,vx,vy,buffer,num_nivel)==true){
        cx = 710;
        cy =100; vy =-1 ;vx=0;
        if(num_nivel !=3) num_nivel++;

        while(!key[KEY_A]){
            textout_centre_ex(screen,font,"PRESS (A) FOR THE NEXT LEVEL",370,250,0xFBFF00,0x000000);
            rest(20);
        }
        combustible = 100;
    }
}





void reiniciar_nivel(float &cx,float &cy,float &vx,float &vy,float &combustible){
    cx= 710;
    cy=100;
    vx=0;vy=-1;
    combustible=100;
}













