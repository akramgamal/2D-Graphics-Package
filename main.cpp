#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
#include <tchar.h>
#include <windows.h>
#define clearr 1
#define Open_File 2
#define Save_File 3
#define Line_DDA 4
#define Line_midpoint 5
#define Line_parametric 6
#define q1 7
#define q2 8
#define q3 9
#define q4 10
#define Circle_Polar 11
#define Circle_iterative 12
#define Circle_Midpoint 13
#define Circle_Modified 14
#define Ellipse_Polar 15
#define Ellipse_Direct 16
#define red 17
#define green 18
#define blue 19
#define black 20
#define yellow 21
#define grey 22
#define purple 23
#define clipping 24
#define clipping2 25
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               500,                 /* The programs width */
               400,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

bool PointClipping(int x, int y, int xleft, int xright, int ytop, int ybottom)
{
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom) return true;
    else return false;
}
int xleft,xright,ytop,ybottom;
bool clip=false;
double round(double y)
{
    return (int)y+.5;
}
void DDA(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {
        double slope=dy/dx;
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        int x=x1;
        double y=y1;
        SetPixel(hdc,x,y,color);
        while(x<=x2)
        {
            x++;
            y+=slope;
            SetPixel(hdc,x,y,color);
        }
    }
    else
    {
        double slope=dx/dy;
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double x=x1;
        int y=y1;
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            y++;
            x+=slope;
            SetPixel(hdc,x,y,color);
        }
    }
}
void DDAForClippingCircle(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {
        double slope=dy/dx;
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        int x=x1;
        double y=y1;
        if(PointClipping(x,y,xleft,xright,ytop,ybottom))
        SetPixel(hdc,x,y,color);
        while(x<=x2)
        {
            x++;
            y+=slope;
            if(PointClipping(x,y,xleft,xright,ytop,ybottom))
                SetPixel(hdc,x,y,color);
        }
    }
    else
    {
        double slope=dx/dy;
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double x=x1;
        int y=y1;
        if(PointClipping(x,y,xleft,xright,ytop,ybottom))
        SetPixel(hdc,x,y,color);
        while(y<=y2)
        {
            y++;
            x+=slope;
            if(PointClipping(x,y,xleft,xright,ytop,ybottom))
                SetPixel(hdc,x,y,color);
        }
    }
}
void DrawLineParamtaric(HDC hdc, int x1,int y1,int x2,int y2, COLORREF color )
{
    int dy=abs(y2-y1);
    int dx=abs(x2-x1);
    float dt=((dx>dy)?dx:dy)+1;
    if(x1>x2)
    {
        swap(x1,x2);
        swap(y1,y2);
    }
    dt=1/dt;
    for(double t=0; t<=1; t+=dt)
    {
        int x=round(x1+(t*(x2-x1)));
        int y=round(y1+(t*(y2-y1)));
        SetPixel(hdc,x,y,color);
    }
}
void DrawLineMidPoint(HDC hdc, int x1,int y1,int x2,int y2, COLORREF color )
{
    // at any slope
    if(x1>x2)
    {
        swap(x1,x2);
        swap(y1,y2);
    }

    int x,y,dy,dx,d,d2,d3,d4,ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8;
    x=x1;
    y=y1;
    dy=y2-y1;
    dx=x2-x1;
    d=dx-(2*dy);
    d2=2*dx-dy;
    d3=(-2*dx)-dy;
    d4=-dx-(2*dy);
    ch1=2*(dx-dy);
    ch2=-2*dy;
    ch3=2*(dx-dy);
    ch4=2*dx;
    ch5=-2*(dx+dy);
    ch6=-2*dx;
    ch7=-2*(dx+dy);
    ch8=-2*dy;
    SetPixel(hdc,x,y,color);
    if(abs(dx)>abs(dy))
    {
        if(dy>0 && dx >0)
        {
            while(x<x2)
            {
                if(d<0)
                {
                    d=d+ch1;
                    y=y+1;
                }
                else
                {
                    d=d+ch2;
                }
                x=x+1;
                SetPixel(hdc,x,y,color);
            }
        }
        else
        {
            while(x<x2)
            {
                if(d4>0)
                {
                    d4=d4+ch7;
                    y=y-1;
                }
                else
                {
                    d4=d4+ch8;
                }
                x=x+1;
                SetPixel(hdc,x,y,color);
            }
        }
    }
    else if (abs(dy)>abs(dx))
    {
        if(dx>0 && dy>0)
        {
            while(y<y2)
            {
                if(d2>0)
                {
                    d2=d2+ch3;
                    x=x+1;
                }
                else
                {

                    d2=d2+ch4;
                }
                y=y+1;
                SetPixel(hdc,x,y,color);
            }
        }
        else
        {
            while(y>y2)
            {
                if(d3<0)
                {
                    x=x+1;
                    d3=d3+ch5;
                }
                else
                {
                    d3=d3+ch6;
                }
                y=y-1;
                SetPixel(hdc,x,y,color);
            }
        }
    }
}
void draw8points(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    if(clip)
    {
        if(PointClipping(xc+x,yc+y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc+x,yc+y,c);
    }
    else SetPixel(hdc,xc+x,yc+y,c);
    if(clip)
    {
        if(PointClipping(xc-x,yc+y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc-x,yc+y,c);
    }
    else SetPixel(hdc,xc-x,yc+y,c);
    if(clip)
    {
        if(PointClipping(xc-x,yc-y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc-x,yc-y,c);
    }
    else SetPixel(hdc,xc-x,yc-y,c);
    if(clip)
    {
        if(PointClipping(xc+x,yc-y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc+x,yc-y,c);
    }
    else SetPixel(hdc,xc+x,yc-y,c);
    if(clip)
    {
        if(PointClipping(xc+y,yc+x,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc+y,yc+x,c);
    }
    else SetPixel(hdc,xc+y,yc+x,c);
    if(clip)
    {
        if(PointClipping(xc-y,yc+x,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc-y,yc+x,c);
    }
    else SetPixel(hdc,xc-y,yc+x,c);
    if(clip)
    {
        if(PointClipping(xc-y,yc-x,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc-y,yc-x,c);
    }
    else SetPixel(hdc,xc-y,yc-x,c);
    if(clip)
    {
        if(PointClipping(xc+y,yc-x,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc+y,yc-x,c);
    }
    else SetPixel(hdc,xc+y,yc-x,c);

}
void Circle_Iterative_Polar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    draw8points(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;
        draw8points(hdc,xc,yc,round(x),round(y),color);
    }
}
void Polar_direct_method(HDC hdc,int xc,int yc,int R,COLORREF color)
{
    int x=R,y=0;
    double theta=0,dtheta=1.0/R;
    draw8points(hdc,xc,yc,x,y,color);
    while(y<x)
    {
        theta+=dtheta;
        x=round(R*cos(theta));
        y=round(R*sin(theta));
        draw8points(hdc,xc,yc,x,y,color);
    }
}
void draw_Midpoint_circle2ndimpro(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0;
    int y = R;
    int d = 1 - R;
    int d1 = 3;
    int d2 = 5-(2*R);
    draw8points(hdc, xc, yc, x, y, c);
    while (x < y)
    {
        if (d <= 0)
        {
            d+=d1;
            d2+=2;
        }
        else if (d > 0)
        {
            d+=d2;
            d2+=4;
            y--;
        }
        d1+=2;
        x++;
        draw8points(hdc, xc, yc, x, y, c);
    }

}

void draw_Midpoint_circle1stimpro(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0;
    int y = R;
    int d = 1 - R;
    draw8points(hdc, xc, yc, x, y, c);
    while (x < y)
    {
        if (d <= 0)
        {
            d+=(2*x) + 3;
        }
        else if (d > 0)
        {
            d+=2*(x-y) + 5;
            y--;
        }
        x++;
        draw8points(hdc, xc, yc, x, y, c);
    }

}
void draw4points(HDC hdc, int xc,int yc,int x,int y,COLORREF c)
{
    if(clip)
    {
        if(PointClipping(xc+x,yc+y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc+x,yc+y,c);
    }
    else SetPixel(hdc,xc+x,yc+y,c);
    if(clip)
    {
        if(PointClipping(xc-x,yc-y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc-x,yc-y,c);
    }
    else SetPixel(hdc,xc-x,yc-y,c);
    if(clip)
    {
        if(PointClipping(xc-x,yc+y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc-x,yc+y,c);
    }
    else SetPixel(hdc,xc-x,yc+y,c);
    if(clip)
    {
        if(PointClipping(xc+x,yc-y,xleft,xright,ytop,ybottom))
            SetPixel(hdc,xc+x,yc-y,c);
    }
    else SetPixel(hdc,xc+x,yc-y,c);
}
void DrawEllipse_Cartesian(HDC hdc,int xc,int yc,double a,double b,COLORREF c)
{
    double a_sqr,b_sqr,x,y;
    x=0;
    y=b;
    a_sqr=a*a;
    b_sqr=b*b;
    while(x<=a+1)
    {
        draw4points(hdc,xc,yc,round(x),round(y),c);
        x+=0.01;
        y=sqrt((a_sqr*b_sqr-b_sqr*(x*x))/a_sqr);
    }
}
void draw_ellipse_Polar(HDC hdc,int xc,int yc,double a,double b,COLORREF c)
{
    double x = 0;
    double y = b;
    for(double theta=0; theta<=1.57; theta+=0.001)
    {
        x = a*cos(theta);
        y = b*sin(theta);
        draw4points(hdc, xc, yc, round(x), round(y), c);
    }
}
void Quarter1(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double y=R;
    double x=0;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        if(clip)
        {
            DDAForClippingCircle(hdc,xc,yc,xc+x,yc-y,c);
            DDAForClippingCircle(hdc,xc,yc,xc+y,yc-x,c);
        }
        else
        {
            DDA(hdc,xc,yc,xc+x,yc-y,c);
            DDA(hdc,xc,yc,xc+y,yc-x,c);
        }
    }
}
void Quarter2(HDC hdc,int xc,int yc,int R,COLORREF c)
{

    double y=R;
    double x=0;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        if(clip)
        {
            DDAForClippingCircle(hdc,xc,yc,xc-x,yc-y,c);
            DDAForClippingCircle(hdc,xc,yc,xc-y,yc-x,c);
        }
        else
        {
            DDA(hdc,xc,yc,xc-x,yc-y,c);
            DDA(hdc,xc,yc,xc-y,yc-x,c);
        }
    }
}
void Quarter3(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double y=R;
    double x=0;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        if(clip)
        {
            DDAForClippingCircle(hdc,xc,yc,xc-x,yc+y,c);
            DDAForClippingCircle(hdc,xc,yc,xc-y,yc+x,c);
        }
        else
        {
            DDA(hdc,xc,yc,xc-x,yc+y,c);
            DDA(hdc,xc,yc,xc-y,yc+x,c);
        }
    }
}
void Quarter4(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double y=R;
    double x=0;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        if(clip)
        {
            DDAForClippingCircle(hdc,xc,yc,xc+x,yc+y,c);
            DDAForClippingCircle(hdc,xc,yc,xc+y,yc+x,c);
        }
        else
        {
            DDA(hdc,xc,yc,xc+x,yc+y,c);
            DDA(hdc,xc,yc,xc+y,yc+x,c);
        }
    }
}
HMENU hmenu;
void AddMenus(HWND hwnd)
{
    hmenu=CreateMenu();
    HMENU hfile=CreateMenu();
    HMENU hline=CreateMenu();
    HMENU hcircle=CreateMenu();
    HMENU hellipse=CreateMenu();
    HMENU filling=CreateMenu();
    HMENU color=CreateMenu();
    HMENU cl=CreateMenu();
    ///file
    AppendMenu(hfile,MF_STRING,Open_File,"Open");
    AppendMenu(hfile,MF_STRING,Save_File,"Save");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hfile,"File");
    ///Line
    AppendMenu(hline,MF_STRING,Line_DDA,"DDA");
    AppendMenu(hline,MF_STRING,Line_midpoint,"Midpoint");
    AppendMenu(hline,MF_STRING,Line_parametric,"Parametric");
    ///Circle
    AppendMenu(hcircle,MF_STRING,Circle_Polar,"Polar");
    AppendMenu(hcircle,MF_STRING,Circle_iterative,"iterative");
    AppendMenu(hcircle,MF_STRING,Circle_Midpoint,"Midpoint");
    AppendMenu(hcircle,MF_STRING,Circle_Modified,"Modified Midpoint");
    AppendMenu(hcircle,MF_POPUP,(UINT_PTR)filling,"filling");
    AppendMenu(filling,MF_STRING,q1,"Quarter 1");
    AppendMenu(filling,MF_STRING,q2,"Quarter 2");
    AppendMenu(filling,MF_STRING,q3,"Quarter 3");
    AppendMenu(filling,MF_STRING,q4,"Quarter 4");
    ///ellipse
    AppendMenu(hellipse,MF_STRING,Ellipse_Polar,"Polar");
    AppendMenu(hellipse,MF_STRING,Ellipse_Direct,"Direct");
    ///color
    AppendMenu(color,MF_STRING,red,"Red");
    AppendMenu(color,MF_STRING,green,"Green");
    AppendMenu(color,MF_STRING,blue,"Blue");
    AppendMenu(color,MF_STRING,black,"Black");
    AppendMenu(color,MF_STRING,yellow,"Yellow");
    AppendMenu(color,MF_STRING,grey,"Grey");
    AppendMenu(color,MF_STRING,purple,"purple");

    AppendMenu(cl,MF_STRING,clipping,"point for circle and ellipse");
    AppendMenu(cl,MF_STRING,clipping2,"line");

    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hline,"Line");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hcircle,"Circle");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hellipse,"Ellpise");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)color,"Color");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)cl,"Clipping");
    AppendMenu(hmenu,MF_STRING,clearr,"Clear");

    SetMenu(hwnd,hmenu);
}
struct data
{
    int x1,x2,y1,y2,da;//1->line,2->circle,3->ellipse
    COLORREF color;
};
vector<data> v;
string open_file(HWND hwnd)
{
    string path;
    OPENFILENAME ofn;
    char file_path[100];
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=file_path;
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile=100;
    ofn.lpstrFilter="Text Files\0*.txt\0";
    ofn.nFilterIndex=1;
    GetOpenFileName(&ofn);
    path=ofn.lpstrFile;
    return path;
}
string save_file(HWND hwnd)
{
    string path;
    OPENFILENAME ofn;
    char file_name[100];
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=file_name;
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile=100;
    ofn.lpstrFilter="Text Files\0*.txt\0";
    ofn.nFilterIndex=1;
    ofn.lpstrDefExt="*.txt";
    GetSaveFileName(&ofn);
    path=ofn.lpstrFile;
    return path;
}
void save(string path)
{
    ofstream in(path);
    for(int i=0; i<v.size(); i++)
    {
        in<<v[i].x1<<"\t"<<v[i].x2<<"\t"<<v[i].y1<<"\t"<<v[i].y2<<"\t"<<v[i].color<<"\t"<<v[i].da<<endl;
    }
}
void SaveShapes(HWND hwnd)
{
    string path=save_file(hwnd);
    save(path);
    v.clear();
}
void load(string path,HWND hwnd,HDC hdc)
{
    //InvalidateRect(hwnd, NULL, TRUE);
    ifstream out(path);
    while(!out.eof())
    {
        int x1,x2,y1,y2,drawalgo;
        COLORREF c;
        out>>x1>>x2>>y1>>y2>>c>>drawalgo;
        if(drawalgo==1)
        {
            DDA(hdc,x1,y1,x2,y2,c);
        }
        else if(drawalgo==2)
        {
            int R=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
            draw_Midpoint_circle1stimpro(hdc,x1,y1,R,c);
        }
        else if(drawalgo==3)
        {
            double a=abs(x2-x1);
            double b=abs(y2-y1);
            draw_ellipse_Polar(hdc,x1,y1,a,b,c);
        }
        else if(drawalgo==4)
        {
            int R=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
            Quarter1(hdc,x1,y1,R,c);
        }
        else if(drawalgo==5)
        {
            int R=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
            Quarter2(hdc,x1,y1,R,c);
        }
        else if(drawalgo==6)
        {
            int R=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
            Quarter3(hdc,x1,y1,R,c);
        }
        else if(drawalgo==7)
        {
            int R=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
            Quarter4(hdc,x1,y1,R,c);
        }
    }
    out.close();
}
void LoadShapes(HWND hwnd,HDC hdc)
{
    string path=open_file(hwnd);
    load(path,hwnd,hdc);
}
union outcode
{
    unsigned all:4;
    struct
    {
        unsigned left:1, top:1, right:1, bottom:1;
    };
};

outcode getoutcode(double x, double y, int xleft, int xright, int ytop, int ybottom)
{
    outcode out;
    out.all = 0;
    if (x < xleft) out.left = 1;
    else if(x > xright) out.right =1;
    else if (y < ytop) out.top = 1;
    else if (y > ybottom) out.bottom = 1;
    return out;
}
void line_clipping(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int xright, int ytop, int ybottom,COLORREF c)
{
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    outcode out1 = getoutcode(x1, x2, xleft, xright, ytop, ybottom);
    outcode out2 = getoutcode(x2, y2, xleft, xright, ytop, ybottom);
    while ((out1.all || out2.all) && !(out1.all & out2.all))
    {
        double xi, yi;
        if(out1.all)
        {
            if (out1.left)
            {
                xi = xleft;
                yi = y1 + (xi-x1) * (y2-y1) / (x2-x1);
            }
            else if(out1.top)
            {
                yi = ytop;
                xi = x1 + (yi-y1) * (x2-x1) / (y2-y1);
            }
            else if (out1.right)
            {
                xi = xright;
                yi = y1 + (xi-x1) * (y2-y1) / (x2-x1);
            }
            else if(out1.bottom)
            {
                yi = ybottom;
                xi = x1 + (yi-y1) * (x2-x1) / (y2-y1);
            }
            x1 = xi;
            y1 = yi;
            out1 = getoutcode(x1, y1, xleft, xright, ytop, ybottom);
        }
        else
        {
            if (out2.left)
            {
                xi = xleft;
                yi = y1 + (xi-x1) * (y2-y1) / (x2-x1);
            }
            else if(out2.top)
            {
                yi = ytop;
                xi = x1 + (yi-y1) * (x2-x1) / (y2-y1);
            }
            else if (out2.right)
            {
                xi = xright;
                yi = y1 + (xi-x1) * (y2-y1) / (x2-x1);
            }
            else if(out2.bottom)
            {
                yi = ybottom;
                xi = x1 + (yi-y1) * (x2-x1) / (y2-y1);
            }
            x2 = xi;
            y2 = yi;
            out2 = getoutcode(x2, y2, xleft, xright, ytop, ybottom);
        }
    }
    if (!out1.all && !out2.all)
    {
        DDA(hdc,round(x1),round(y1),round(x2),round(y2),c);
        v.push_back(data{x1,x2,y1,y2,1,c});
    }
}
void Draw(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    DDA(hdc,x1,y1,x2,y1,c);

    DDA(hdc,x2,y1,x2,y2,c);

    DDA(hdc,x1,y2,x2,y2,c);

    DDA(hdc,x1,y1,x1,y2,c);
}
int x1,y11,x2,y2,R;
COLORREF cc;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case  WM_COMMAND:
        switch(wParam)
        {
        case Open_File:
            LoadShapes(hwnd,hdc);
            break;
        case Save_File:
            SaveShapes(hwnd);
            break;
        case red:
            cc=RGB(255,0,0);
            break;
        case green:
            cc=RGB(0,255,0);
            break;
        case blue:
            cc=RGB(0,0,255);
            break;
        case black:
            cc=RGB(0,0,0);
            break;
        case yellow:
            cc=RGB(255,255,0);
            break;
        case grey:
            cc=RGB(150,150,150);
            break;
        case purple:
            cc=RGB(150,0,205);
            break;
        case Line_DDA:
            if(clip)
                line_clipping(hdc,x1,y11,x2,y2,xleft,xright,ytop,ybottom,cc);
            else
            {
                DDA(hdc,x1,y11,x2,y2,cc);
                v.push_back(data{x1,x2,y11,y2,1,cc});
            }
            break;
        case Line_midpoint:
            if(clip)
                line_clipping(hdc,x1,y11,x2,y2,xleft,xright,ytop,ybottom,cc);
            else
            {
                DrawLineMidPoint(hdc,x1,y11,x2,y2,cc);
                v.push_back(data{x1,x2,y11,y2,1,cc});
            }
            break;
        case Line_parametric:
            if(clip)
                line_clipping(hdc,x1,y11,x2,y2,xleft,xright,ytop,ybottom,cc);
            else
            {
                DrawLineParamtaric(hdc,x1,y11,x2,y2,cc);
                v.push_back(data{x1,x2,y11,y2,1,cc});
            }
            break;
        case Circle_Midpoint:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            draw_Midpoint_circle1stimpro(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,2,cc});
            break;
        case Circle_Polar:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            Polar_direct_method(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,2,cc});
            break;
        case Circle_iterative:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            Circle_Iterative_Polar(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,2,cc});
            break;
        case Circle_Modified:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            draw_Midpoint_circle2ndimpro(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,2,cc});
            break;
        case clearr:
            InvalidateRect(hwnd, NULL, TRUE);
            clip=false;
            break;
        case q1:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            Quarter1(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,4,cc});
            break;
        case q2:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            Quarter2(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,5,cc});
            break;
        case q3:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            Quarter3(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,6,cc});
            break;
        case q4:
            R=sqrt(pow(y2-y11,2)+pow(x2-x1,2));
            Quarter4(hdc,x1,y11,R,cc);
            v.push_back(data{x1,x2,y11,y2,7,cc});
            break;
        case Ellipse_Polar:
        {
            double a=abs(x2-x1);
            double b=abs(y2-y11);
            draw_ellipse_Polar(hdc,x1,y11,a,b,cc);
            v.push_back(data{x1,x2,y11,y2,3,cc});
            break;
        }
        case Ellipse_Direct:
        {
            double a=abs(x2-x1);
            double b=abs(y2-y11);
            DrawEllipse_Cartesian(hdc,x1,y11,a,b,cc);
            v.push_back(data{x1,x2,y11,y2,3,cc});
            break;
        }
        case clipping:
            clip=true;
            xleft=x1,xright=x2,ytop=y11,ybottom=y2;
            Draw(hdc,x1,y11,x2,y2,cc);
            break;
        case clipping2:
            clip=true;
            xleft=x1,xright=x2,ytop=y11,ybottom=y2;
            Draw(hdc,x1,y11,x2,y2,cc);
            break;
        }
        break;
    case WM_LBUTTONDOWN:
        x1=LOWORD(lParam);
        y11=HIWORD(lParam);
        break;
    case WM_RBUTTONDOWN:
        x2=LOWORD(lParam);
        y2=HIWORD(lParam);
        cout<<"X1= "<<x1<<" X2= "<<x2<<" Y3= "<<y11<<" Y2= "<<y2<<endl;
        break;
    case WM_CREATE:
        AddMenus(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
