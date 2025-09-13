
#include "framework.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include <iostream>
#include <vector>

WNDCLASS            wndClass;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

HWND                hWnd;

HDC hdcMem;

HBITMAP     hbmMem;
HANDLE      hOld;


enum item_id { stick, sword, rubin };

struct items {
    item_id id;
    std::string name;
    int dmgup;
    int hpup;
};

enum enemies_id { slime, bat };

struct enemies {
    enemies_id id;
    std::string name;
    int hp;
    int dmg;
}enemy[2];

struct {
    std::string name;
} item[3];

struct {
    int id;
    std::string name;
    std::vector<int> door;
    std::vector<items> loot;
    std::vector<enemies> enemy;
    Image* background;
} location[4];

struct {
    std::string name;
}inventory;

struct {
    int pos;
    int hp;
    int dmg;
    std::vector<items> inventory;

} player;

void Init()
{
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    std::vector<int> a{ 1,2,3 };
    a.erase(a.begin() + 1);



    /*struct {
        std::string name;
        int hp;
        int dmg;
    } enemy[2];*/

    enemy[0].dmg = 3;
    enemy[0].hp = 3;

    enemy[1].dmg = 1;
    enemy[1].hp = 3;


    /*item[0].name = "stick1";
    item[1].name = "stick2";
    item[2].name = "stick3";
    item[3].name = "stick4";*/

    location[0].name = "home";
    location[0].door.push_back(1);
    location[0].background = new Bitmap(L"home.jpg");

    location[1].name = "forest";
    location[1].id = 1;
    location[1].door.push_back(0);
    location[1].door.push_back(2);
    location[1].door.push_back(3);
    location[1].loot.push_back({ item_id::stick,"stick " });
    location[1].background = new Bitmap(L"forest1.jpg");

    location[2].name = "cave";
    location[2].door.push_back(1);
    location[2].loot.push_back({ item_id::sword,"sword ",2,0 });
    location[2].enemy.push_back({ enemies_id::bat,"bat",2,1 });
    location[2].background = new Bitmap(L"cave.jpg");

    location[3].name = "swamp";
    location[3].door.push_back(1);
    location[3].loot.push_back({ item_id::rubin,"rubin ",0,10 });
    location[3].enemy.push_back({ enemies_id::slime,"slime",5,3 });
    location[3].background = new Bitmap(L"Swamp.jpg");




    int act;
    int invact;
    std::string locact;
    player.pos = 0;
    player.hp = 20;
    player.dmg = 1;

    int enemy_hp;
    int enemy_dmg = 0;


    /*
    while (true) {

        for (int i = 0; i < location[player.pos].enemy.size(); i++)
        {

            //std::cout << "enemy: " << location[player.pos].enemy[i].name << "\n";

            while (location[player.pos].enemy[i].hp > 0) {
                if (player.hp <= 0) {
                    break;
                }
                std::cout << "enemy: " << location[player.pos].enemy[i].name << "\n";
                std::cout << "your stats: " << "hp: " << player.hp << " dmg: " << player.dmg << "\n";
                std::cout << "enemy stats: " << "hp: " << location[player.pos].enemy[i].hp << " dmg: " << location[player.pos].enemy[i].dmg << "\n";
                std::cout << "your next move:\n" << "1.atack " << "2.defence " << "\n";
                std::cin >> invact;
                if (invact == 1) {
                    location[player.pos].enemy[i].hp -= player.dmg;
                    player.hp -= location[player.pos].enemy[i].dmg;
                }
                if (invact == 2) {
                    player.hp -= (location[player.pos].enemy[i].dmg - 1);
                }
            }
        }

        std::cout << "current location: " << location[player.pos].name << "\n";
        std::cout << "your stats: \n" << "hp: " << player.hp << "\n" << "dmg: " << player.dmg << "\n";
        //std::cout << "your equip: " <<  << "\n";



        if (player.hp <= 0) {
            std::cout << "game over! \n";
            break;
        }

        std::cout << "your action:\n" << "1.inventory " << "2.items around " << "3.go to location " << "\n";

        std::cin >> act;

        if (act == 1) {
            if (player.inventory.size() == 0) {
                std::cout << "empty \n";
            }
            else
                std::cout << "your items: \n";
            for (int c = 0; c < player.inventory.size(); c++) {
                std::cout << player.inventory[c].name << "\n";
            }
        }

        if (act == 2) {
            if (location[player.pos].loot.size() > 0) {
                for (int c = 0; c < location[player.pos].loot.size(); c++) {
                    std::cout << location[player.pos].loot[c].name << "\n";

                    std::cout << "your action:\n" << "1.pick up " << "2.leave ";
                    std::cin >> invact;
                    if (invact == 1) {
                        player.inventory.push_back({ location[player.pos].loot[c] });

                        player.dmg += location[player.pos].loot[c].dmgup;
                        player.hp += location[player.pos].loot[c].hpup;
                        location[player.pos].loot.erase(location[player.pos].loot.begin());
                    }
                }
            }
            else {
                std::cout << "nothing here\n";
            }
        }

        if (act == 3) {
            std::cout << "choose location:\n";

            for (int i = 0; i < location[player.pos].door.size(); i++)
            {
                int dst = location[player.pos].door[i];
                std::cout << location[dst].name << "\n";

            }


            bool is_ok = false;
            while (is_ok == false) {
                std::cout << "go to: ";
                std::cin >> locact;
                for (int i = 0; i < location[player.pos].door.size(); i++) {

                    int dst = location[player.pos].door[i];

                    if (locact == location[dst].name)
                    {
                        player.pos = dst;
                        is_ok = true;

                    }
                }

                if (!is_ok) {
                    std::cout << "wrong name \n";
                }
            }

        }
    }
    */



}


std::vector<Gdiplus::Image*> images;
int currentImageIndex = 0;

    
//Gdiplus::Image* img0 = new Bitmap(L"home.jpg");
//Gdiplus::Image* img1 = new Bitmap(L"forest1.jpg");
//Gdiplus::Image* img2 = new Bitmap(L"cave.jpg");
//Gdiplus::Image* img3 = new Bitmap(L"Swamp.jpg");

RECT rc;

int x = 100;
int y = 100;
const int move = 50;

//void HandleKeyPress(WPARAM key) {
//    switch (key) {
//    case VK_LEFT:
//        x = -100;
//        break;
//    case VK_RIGHT:
//        x = +100;
//        break;
//    }
//}

VOID OnPaint(HWND hWnd, LPPAINTSTRUCT lpPS)
{
    /*RECT rc;
    HDC hdcMem;
    HBITMAP hbmMem, hbmOld;
    HBRUSH hbrBkGnd;
    GetClientRect(hWnd, &rc);
    hdcMem = CreateCompatibleDC(lpPS->hdc);
    hbmMem = CreateCompatibleBitmap(lpPS->hdc,
        rc.right - rc.left,
        rc.bottom - rc.top);
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);


    hdcMem = CreateCompatibleDC(lpPS->hdc);

    Gdiplus::Graphics graphics(hdcMem);

    RECT rc1;
    GetClientRect(hWnd, &rc1);
    Gdiplus::Rect gdi_rc(0, 0, rc1.right, rc1.bottom);
    
    Gdiplus::Image image(L"Swamp.jpg");
    graphics.DrawImage(&image,0,0);
    */

    
    if (images.empty())
    {
        images.push_back(new Gdiplus::Bitmap(L"home.jpg"));
        images.push_back(new Gdiplus::Bitmap(L"forest1.jpg"));
        images.push_back(new Gdiplus::Bitmap(L"cave.jpg"));
        images.push_back(new Gdiplus::Bitmap(L"Swamp.jpg"));
    }
      
    Gdiplus::Graphics graphics(lpPS->hdc);

    auto dc = GetWindowDC(hWnd);
    //Gdiplus::Graphics graphics(dc);
    //Gdiplus::Image image(L"home.jpg");
    //Gdiplus::Image* img0 = new Bitmap(L"home.jpg");
    //Gdiplus::Image* img1 = new Bitmap(L"forest1.jpg");
    //Gdiplus::Image* img2 = new Bitmap(L"cave.jpg");
    //Gdiplus::Image* img3 = new Bitmap(L"Swamp.jpg");

    //Image* currentImage = images[currentImage];
    //graphics.DrawImage(currentImage, 0, 0);
    //graphics.DrawImage(img1, 0, 0);

    

    if (currentImageIndex < images.size())
    {
        Gdiplus::Image* currentImage = images[currentImageIndex];

        RECT rc;
        GetClientRect(hWnd, &rc);

        graphics.DrawImage(images[currentImageIndex],0, 0,rc.right - rc.left,rc.bottom - rc.top);

        graphics.DrawImage(images[1], x, y, 88, 88);
    }



}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");
    wndClass.hbrBackground = NULL;


    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("Getting Started"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;


    

    switch (message)
    {

    case WM_TIMER:
        
    switch (1)
    {
    case 1:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }

    case WM_CREATE:
    SetTimer(hWnd, 1, 700, NULL);
    break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        OnPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_ERASEBKGND:
        return 1; 
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);

    case WM_LBUTTONDOWN:
        if (!images.empty())
        {
            currentImageIndex = (currentImageIndex + 1) % images.size();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_KEYDOWN:
        switch (wParam) 
        {
        case VK_LEFT:
            x -= move;
            break;
        case VK_RIGHT:
            x += move;
            break;
        case VK_UP:
            y -= move;
            break;
        case VK_DOWN:
            y += move;
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
}