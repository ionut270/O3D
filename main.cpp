#include <iostream>
#include <fstream>
#include <graphics.h>
#include <winbgim.h>
#include <string>
#include <math.h>
using namespace std;

int Xecran=1280,Yecran=720,depth=0, centreZ=1500,id=0,id1=0,xMouse,yMouse,pSelectat1=NULL,pSelectat2=NULL;
bool click=false;
int page=1,okPunct=0;

struct punct {
    float xC;
    float yC;
    float zC;
    int id;
    punct* urm;
    punct* prec;
};
struct listaDublaPunct {
    punct* prim;
    punct* ultim;
    unsigned int lungime;
};
void initializeazaPuncte(listaDublaPunct& P) {
    P.lungime=0;
    P.prim=NULL;
    P.ultim=NULL;
}
void adaugaPunct(listaDublaPunct& P, float x, float y, float z, int i) {
    if (P.lungime==0) {
        P.prim=new punct;
        P.prim->xC=x;
        P.prim->yC=y;
        P.prim->zC=z;
        P.prim->id=i;
        P.prim->urm=NULL;
        P.prim->prec=NULL;
        P.ultim=P.prim;
        P.lungime++;
    } else {
        punct* p;
        p=new punct;
        p->xC=x;
        p->yC=y;
        p->zC=z;
        p->id=i;
        p->urm=NULL;
        p->prec=P.ultim;
        P.lungime++;
        P.ultim->urm=p;
        P.ultim=p;
    }

}

struct linie {
    int aP;
    int bP;
    linie* urm;
    linie* prec;
};
struct listaDublaLinie {
    linie* prim;
    linie* ultim;
    unsigned int lungime;
};
void initializeazaLinie(listaDublaLinie& L) {
    L.lungime=0;
    L.prim=NULL;
    L.ultim=NULL;
}
void adaugaLinie(listaDublaLinie& L, int a, int b) {
    if (L.lungime==0) {
        L.prim=new linie;
        L.prim->aP=a;
        L.prim->bP=b;
        L.prim->urm=NULL;
        L.prim->prec=NULL;
        L.ultim=L.prim;
        L.lungime++;
    } else {
        linie* l;
        l=new linie;
        l->aP=a;
        l->bP=b;
        l->urm=NULL;
        l->prec=L.ultim;
        L.lungime++;
        L.ultim->urm=l;
        L.ultim=l;
    }
}

void citire (listaDublaPunct& P, listaDublaLinie& L) {
    float x,y,z;
    int i,a,b;
    ifstream f("cub.iaa");
    int nrPoints,nrLines;
    f >> nrPoints;
    f >> nrLines;
    for(i=1; i<=nrPoints; i++) {
        f >> x;
        f >> y;
        f >> z;
        adaugaPunct(P, x, y, z, i);
    }
    for(int j=1; j<=nrLines; j++) {
        f>>a;
        f>>b;
        adaugaLinie(L,a,b);
    }
    f.close();
}

int transforma(float coordonata,float adancime) {
    float scale,fl=300;
    scale=fl/(fl*(adancime+depth+centreZ));
    coordonata=coordonata*scale*100;
    return  coordonata;
}

int deTransforma(float coordonata,float adancime) {
    float scale,fl=300;
    scale=fl/(fl*(adancime+depth+centreZ));
    coordonata=(coordonata/scale)/100;
    return  coordonata;
}

int buton(char *Text,int x1,int y1,int idButon) {
    int color=15;
    setfillstyle(SOLID_FILL,0);
    bar(x1,y1,x1+80,y1+20);
    setfillstyle(EMPTY_FILL,15);
    outtextxy(x1+10,y1+2,Text);
    int x,y;
    x=mousex();
    y=mousey();
    if (x>=x1 && x<=x1+80 && y>=y1 && y<=y1+20) {
        if(ismouseclick(WM_LBUTTONDOWN)) {
            color=4;
            id=idButon;
            if(idButon!=1)
                id1=idButon;
        }
        if(ismouseclick(WM_LBUTTONUP)) {
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_LBUTTONUP);
        }
        setfillstyle(SOLID_FILL,color);
        bar(x1+5,y1+5,x1+80-5,y1+20-5);
        setfillstyle(EMPTY_FILL,15);
        outtextxy(x1+10,y1+2,Text);
    }
}

int UI() {
    setfillstyle(SOLID_FILL,4);
    bar(0,0,1280,40);
    setfillstyle(EMPTY_FILL,15);
    buton("MutPunct",15,10,0);
    buton("SaveOBJ",15+90,10,1);
    buton("StergPCT",15+90*2,10,2);
    buton("AddPunct",15+90*3,10,3);
    buton("AddLine",15+90*4,10,4);

    buton("Rot x",15+90*5,10,8);
    buton("Rot y",15+90*6,10,9);
    buton("Rot z",15+90*7,10,10);

    buton("       +       ",15+90*8,10,11);
    buton("       -       ",15+90*9,10,12);
}


void draw(listaDublaPunct& P, listaDublaLinie& L) {
    UI();
    punct* p;
    linie* l;
    l=L.prim;
    int Cx1,Cx2,Cy1,Cy2;
    p=P.prim;
    while(p!=NULL) {
        Cx1=p->xC;
        Cx1=transforma(Cx1,p->zC);
        Cy1=p->yC;
        Cy1=transforma(Cy1,p->zC);
        setfillstyle(EMPTY_FILL,15);
        fillellipse(Cx1+Xecran/2,Cy1+Yecran/2,5,5);
        p=p->urm;
    }
    while (l!=NULL) {
        p=P.prim;
        while (p!=NULL) {
            if (p->id==l->aP) {
                Cx1=p->xC;
                Cx1=transforma(Cx1,p->zC);
                Cy1=p->yC;
                Cy1=transforma(Cy1,p->zC);
                p=P.ultim;
            }
            p=p->urm;
        }
        p=P.prim;
        while (p!=NULL) {
            if (p->id==l->bP) {
                Cx2=p->xC;
                Cx2=transforma(Cx2,p->zC);
                Cy2=p->yC;
                Cy2=transforma(Cy2,p->zC);
                p=P.ultim;
            }
            p=p->urm;
        }
        line(Cx1+Xecran/2,Cy1+Yecran/2,Cx2+Xecran/2,Cy2+Yecran/2);
        l=l->urm;
    }
}
float rotateX(float angle,listaDublaPunct& P) {
    float coss = cos(angle);
    float sinn =sin(angle);
    punct* p;
    p=P.prim;
    while (p!=NULL) {
        float y = p->yC * coss - p->zC * sinn;
        float z = p->zC * coss + p->yC * sinn;
        p->yC = y;
        p->zC = z;
        p=p->urm;
    }
}
float rotateY(float angle,listaDublaPunct& P) {
    float coss = cos(angle);
    float sinn =sin(angle);
    punct* p;
    p=P.prim;
    while (p!=NULL) {
        float x = p->xC * coss - p->zC * sinn;
        float z = p->zC * coss + p->xC * sinn;
        p->xC = x;
        p->zC = z;
        p=p->urm;
    }
}
float rotateZ(float angle,listaDublaPunct& P) {
    float coss = cos(angle);
    float sinn =sin(angle);
    punct* p;
    p=P.prim;
    while (p!=NULL) {
        float x = p->xC * coss - p->yC * sinn;
        float y = p->yC * coss + p->xC * sinn;
        p->xC = x;
        p->yC = y;
        p=p->urm;
    }

}

void save(listaDublaPunct& P, listaDublaLinie& L) {
    int maxp,maxl=0;
    string numefisier;
    cout<<"Dati numele fisierului ";
    cin>>numefisier;
    if(P.ultim!=NULL)
        maxp=P.ultim->id;
    else
        maxp=0;
    punct* p;
    p=P.prim;
    linie* l;
    l=L.prim;
    while(l!=NULL) {
        l=l->urm;
        maxl++;
    }
    l=L.prim;
    ofstream f(numefisier.c_str());
    f<<maxp<<" "<<maxl<<endl;
    while(p!=NULL) {
        f<<p->xC<<" ";
        f<<p->yC<<" ";
        f<<p->zC<<endl;
        p=p->urm;
    }
    while(l!=NULL) {
        f<<l->aP<<" ";
        f<<l->bP<<endl;
        l=l->urm;
    }
    f.close();
}

void mutPunct(listaDublaPunct& P, listaDublaLinie& L) {
    float Cx1,Cy1,x,y,x1m,y1m,x2m,y2m,ZE;
    punct* p;
    p=P.prim;
    x=mousex();
    y=mousey();
    while(p!=NULL) {
        x=mousex();
        y=mousey();
        Cx1=p->xC;
        Cx1=transforma(Cx1,p->zC);
        Cx1=Cx1+Xecran/2;
        Cy1=p->yC;
        Cy1=transforma(Cy1,p->zC);
        Cy1=Cy1+Yecran/2;
        x=mousex();
        y=mousey();
        if(Cx1>=x-5 && Cx1<=x+5 && Cy1>=y-5 && Cy1<=y+5) {
            if (ismouseclick(WM_RBUTTONDOWN)) {
                xMouse=mousex();
                yMouse=mousey();
                ZE=p->zC;
                while (ismouseclick(WM_RBUTTONDOWN)) {

                    setactivepage(page);
                    setvisualpage(1-page);
                    cleardevice();
                    draw(P,L);
                    x2m=mousex();
                    y2m=mousey();

                    p->zC=ZE+x2m-xMouse;
                    setcolor(4);
                    setcolor(15);
                    page=1-page;

                    if(ismouseclick(WM_RBUTTONUP)) {
                        clearmouseclick(WM_RBUTTONUP);
                        clearmouseclick(WM_RBUTTONDOWN);
                    }
                }
            } else
                while (ismouseclick(WM_LBUTTONDOWN)) {
                    setactivepage(page);
                    setvisualpage(1-page);
                    cleardevice();
                    draw(P,L);
                    x=mousex();
                    y=mousey();
                    if(click==false) {
                        x=mousex();
                        y=mousey();
                        xMouse=x;
                        yMouse=y;
                        click=true;
                    }
                    x=mousex();
                    y=mousey();
                    x=x-Xecran/2;
                    x=deTransforma(x,p->zC);
                    y=y-Yecran/2;
                    y=deTransforma(y,p->zC);
                    p->xC=x;
                    p->yC=y;
                    if(ismouseclick(WM_LBUTTONUP)) {
                        click=false;
                        clearmouseclick(WM_LBUTTONUP);
                        clearmouseclick(WM_LBUTTONDOWN);
                    }
                    page=1-page;
                }
            x=mousex();
            y=mousey();
            setfillstyle(SOLID_FILL,3);
            fillellipse(Cx1,Cy1,5,5);
            setfillstyle(EMPTY_FILL,15);
        }
        p=p->urm;
    }

}

void addPunctD(listaDublaPunct& P, listaDublaLinie& L) {
    int x,y,Cx=NULL,Cy=NULL,Yv=NULL;
    punct* p;
    p=P.ultim;
    x=mousex();
    y=mousey();
    if (ismouseclick(WM_LBUTTONDOWN)) {
        x=mousex();
        y=mousey();
        cout<<x<<" "<<y<<endl;
        delay(500);
        Yv=y;
        x=x-Xecran/2;
        x=deTransforma(x,p->zC);
        y=y-Yecran/2;
        y=deTransforma(y,p->zC);
        if (okPunct==0 && Yv>40) {
            adaugaPunct(P, x, y, 0, P.ultim->id+1);
            okPunct=1;
        }
        if (ismouseclick(WM_LBUTTONUP)) {
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_LBUTTONUP);
            okPunct=0;
        }
    }
}

void addLineD(listaDublaPunct& P, listaDublaLinie& L) {
    float Cx1,Cy1,x,y;
    punct* p;
    linie* l;
    p=P.prim;
    l=L.prim;

    while (p!=NULL) {
        x=mousex();
        y=mousey();
        Cx1=p->xC;
        Cx1=transforma(Cx1,p->zC);
        Cx1=Cx1+Xecran/2;
        Cy1=p->yC;
        Cy1=transforma(Cy1,p->zC);
        Cy1=Cy1+Yecran/2;
        x=mousex();
        y=mousey();
        if(Cx1>=x-5 && Cx1<=x+5 && Cy1>=y-5 && Cy1<=y+5) {
            setfillstyle(SOLID_FILL,5);
            fillellipse(Cx1,Cy1,5,5);
            setfillstyle(EMPTY_FILL,15);
            if (ismouseclick(WM_LBUTTONDOWN)) {
                if (pSelectat1==NULL && click==false) {
                    click=true;
                    pSelectat1=p->id;
                } else if (pSelectat2==NULL && click==false) {
                    pSelectat2=p->id;
                    click=true;
                }
                if (pSelectat1!=NULL && pSelectat2!=NULL) {
                    adaugaLinie(L,pSelectat1,pSelectat2);
                    pSelectat1=NULL;
                    pSelectat2=NULL;
                }
                if (ismouseclick(WM_LBUTTONUP)) {
                    click=false;
                    clearmouseclick(WM_LBUTTONUP);
                    clearmouseclick(WM_LBUTTONDOWN);
                }
            }
        }
        p=p->urm;
    }
}

void stergePunct(listaDublaPunct& P, listaDublaLinie& L) {
    float x,y,idDeSters;
    float Cx1,Cy1,x1m,y1m,x2m,y2m;
    punct* precedentP;
    punct* sucesorP;
    punct* p;
    linie* test;
    linie* l;
    l=L.prim;
    linie* precedentL;
    linie* sucesorL;
    p=P.prim;
    while(p!=NULL) {
        x=mousex();
        y=mousey();
        Cx1=p->xC;
        Cx1=transforma(Cx1,p->zC);
        Cx1=Cx1+Xecran/2;
        Cy1=p->yC;
        Cy1=transforma(Cy1,p->zC);
        Cy1=Cy1+Yecran/2;
        x=mousex();
        y=mousey();
        if(Cx1>=x-5 && Cx1<=x+5 && Cy1>=y-5 && Cy1<=y+5) {
            idDeSters=p->id;
            setfillstyle(SOLID_FILL,4);
            fillellipse(Cx1,Cy1,8,8);
            setfillstyle(EMPTY_FILL,15);
            if (ismouseclick(WM_LBUTTONDOWN)) {
                if(p->urm!=NULL && p->prec!=NULL) {
                    precedentP=p->prec;
                    sucesorP=p->urm;
                    delete p;
                    precedentP->urm=sucesorP;
                    sucesorP->prec=precedentP;
                } else if(p->urm==NULL && p->prec!=NULL) {
                    precedentP=p->prec;
                    delete p;
                    precedentP->urm=NULL;
                    P.ultim=precedentP;
                } else if(p->urm!=NULL && p->prec==NULL) {
                    sucesorP=p->urm;
                    delete p;
                    sucesorP->prec=NULL;
                    P.prim=sucesorP;
                } else if(p->urm==NULL && p->prec==NULL) {
                    delete p;
                    P.prim=P.ultim=NULL;
                }

                if(P.prim!=NULL) {
                    p=P.prim;
                    while(p!=NULL) {
                        if(idDeSters<p->id)
                            p->id=p->id-1;
                        p=p->urm;
                    }
                }
                while (l!=NULL) {
                    if (l->aP==idDeSters || l->bP==idDeSters) {
                        if(l->prec!=NULL && l->urm!=NULL) {
                            sucesorL=l->urm;
                            precedentL=l->prec;
                            precedentL->urm=sucesorL;
                            sucesorL->prec=precedentL;
                            delete l;
                            l=precedentL;
                        }
                        if(l->prec==NULL && l->urm!=NULL) {
                            sucesorL=l->urm;
                            sucesorL->prec=NULL;
                            L.prim=sucesorL;
                            delete l;
                            l=L.prim;
                        }
                        if(l->prec!=NULL && l->urm==NULL) {
                            precedentL=l->prec;
                            precedentL->urm=NULL;
                            L.ultim=precedentL;
                            delete l;
                            l=L.ultim;
                        }
                        if(l->prec==NULL && l->urm==NULL) {
                            delete l;
                            L.prim=NULL;
                            L.ultim=NULL;
                        }
                    }
                    l=l->urm;
                }
                if(L.prim!=NULL) {
                    l=L.prim;
                    while (l!=NULL) {
                        if(l->aP>idDeSters)
                            l->aP=l->aP-1;
                        if(l->bP>idDeSters)
                            l->bP=l->bP-1;
                        l=l->urm;
                    }
                }
                if(ismouseclick(WM_LBUTTONUP)) {
                    clearmouseclick(WM_LBUTTONDOWN);
                    clearmouseclick(WM_LBUTTONUP);
                }
            }
        }
        if(p!=NULL)
            p=p->urm;
    }
}


int main () {
    initwindow(Xecran,Yecran);
    listaDublaPunct P;
    listaDublaLinie L;
    initializeazaPuncte(P);
    initializeazaLinie(L);
    citire(P,L);
    while (true) {
        setactivepage(page);
        setvisualpage(1-page);
        cleardevice();
        draw(P,L);
        if(id==0) {
            mutPunct(P,L);
        } else if(id==1) {
            save(P,L);
            id=id1;
        } else if (id==2) {
            stergePunct(P,L);
        } else if(id==3) {
            addPunctD(P,L);
        } else if (id==4) {
            addLineD(P,L);
        } else
            /**Rotatie 3D*/
            if(id>7 && id<11) {
                while (id==8 && ismouseclick(WM_LBUTTONDOWN)) {
                    setactivepage(page);
                    setvisualpage(1-page);
                    cleardevice();
                    page=1-page;
                    draw(P,L);
                    rotateX(0.005,P);
                    if (ismouseclick(WM_LBUTTONUP)) {
                        clearmouseclick(WM_LBUTTONDOWN);
                        clearmouseclick(WM_LBUTTONUP);
                    }
                }
                while (id==9 && ismouseclick(WM_LBUTTONDOWN)) {
                    setactivepage(page);
                    setvisualpage(1-page);
                    cleardevice();
                    page=1-page;
                    draw(P,L);
                    rotateY(0.005,P);
                    if (ismouseclick(WM_LBUTTONUP)) {
                        clearmouseclick(WM_LBUTTONDOWN);
                        clearmouseclick(WM_LBUTTONUP);
                    }
                }
                while (id==10 && ismouseclick(WM_LBUTTONDOWN)) {
                    setactivepage(page);
                    setvisualpage(1-page);
                    cleardevice();
                    page=1-page;
                    draw(P,L);
                    rotateZ(0.005,P);
                    if (ismouseclick(WM_LBUTTONUP)) {
                        clearmouseclick(WM_LBUTTONDOWN);
                        clearmouseclick(WM_LBUTTONUP);
                    }
                }
            }
        /**Rotatie 3D*/


        /**Mutare corp 3D*/
        if (id>10&&id<13) {
            while (id ==11 && ismouseclick(WM_LBUTTONDOWN)) {
                setactivepage(page);
                setvisualpage(1-page);
                cleardevice();
                page=1-page;
                draw(P,L);
                depth=depth-5;
                if (ismouseclick(WM_LBUTTONUP)) {
                    clearmouseclick(WM_LBUTTONDOWN);
                    clearmouseclick(WM_LBUTTONUP);
                }
            }
            while (id ==12 && ismouseclick(WM_LBUTTONDOWN)) {
                setactivepage(page);
                setvisualpage(1-page);
                cleardevice();
                page=1-page;
                draw(P,L);
                depth=depth+5;
                if (ismouseclick(WM_LBUTTONUP)) {
                    clearmouseclick(WM_LBUTTONDOWN);
                    clearmouseclick(WM_LBUTTONUP);
                }
            }
        }
        if(ismouseclick(WM_LBUTTONUP)) {
            clearmouseclick(WM_LBUTTONUP);
            clearmouseclick(WM_LBUTTONDOWN);
        }
        page=1-page;
    }
    getch();
}
