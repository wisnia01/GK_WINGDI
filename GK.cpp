/******************************************************************
 Grafika komputerowa, �rodowisko MS Windows - program  przyk�adowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;


POINT car[13] = { {40,60},{90,60}, {100,60},{150,60},{160,55}, {155,30}, {100,15}, {90,15},{80,20}, {70,30}, {5,30}, {0,50}, {40,60} };


int toggle = -1;
bool scaled = false;
int nr = 0;
Point curvePoints[20];

bool isClosed = false;

void skalujPunkty()
{
	for (int i = 0; i < 13; i++)
	{
		car[i].x *= 8;
		car[i].y *= 8;
	}	
}
void initLetter(POINT* k1, POINT* k2)
{
	k1[0].x = 1;
	k1[1].x = 1;
	k1[2].x = 2;
	k1[3].x = 2;

	k1[0].y = 0;
	k1[1].y = 5;
	k1[2].y = 5;
	k1[3].y = 0;

	k2[0].x = 2;
	k2[1].x = 2;
	k2[2].x = 4;
	k2[3].x = 5;
	k2[4].x = 3;
	k2[5].x = 3;
	k2[6].x = 5;
	k2[7].x = 4;

	k2[0].y = 2;
	k2[1].y = 3;
	k2[2].y = 5;
	k2[3].y = 5;
	k2[4].y = 3;
	k2[5].y = 2;
	k2[6].y = 0;
	k2[7].y = 0;

	int offset = 133;
	for (int i = 0; i < 4; i++)
	{
		k1[i].x *= 30;
		k1[i].y *= 30;
		k1[i].x += offset; //przesuniecie
		k1[i].y += offset;
	}

	for (int i = 0; i < 8; i++)
	{
		k2[i].x *= 30;
		k2[i].y *= 30;
		k2[i].x += offset; //przesuniecie
		k2[i].y += offset;
	}

}


//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa g��wnego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domy�lny konstruktor wype�nia struktur� odpowiednimi warto�ciami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; warto�� uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase g��wnego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizuj�cej przetwarzanie meldunk�w 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klas� okna g��wnego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno g��wne
	okno b�dzie mia�o zmienne rozmiary, listw� z tytu�em, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na ca�y ekran, po utworzeniu
	b�dzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wyb�r rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// G��WNA P�TLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozosta�ych komunikat�w zwraca warto�� TRUE */
	{
		TranslateMessage(&meldunek); // wst�pna obr�bka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu w�a�ciwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunk�w kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
/* PONI�SZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEG�LNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysy�any w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wyb�r opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszcze�?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imi� i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
		{
			
			x = LOWORD(lParam);
			y = HIWORD(lParam);

			for (int i = 0; i < 13; i++)
			{
				if (x < car[i].x + 15 && x > car[i].x - 15)
					if (y < car[i].y + 15 && y > car[i].y - 15)
					{
						toggle = i;
						break;
					}
			}
			return 0;
		}
	case WM_LBUTTONUP:
	{
		toggle = -1;
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		curvePoints[nr].X = x;
		curvePoints[nr].Y = y;
		if(nr<20)
			nr++;

		return 0;
	}
	case WM_RBUTTONUP:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		curvePoints[nr].X = x;
		curvePoints[nr].Y = y;
		if (nr < 20)
			nr++;

		return 0;
	}
	case WM_RBUTTONDBLCLK:
	{
		isClosed = true;
		return 0;
	}

	case WM_MOUSEMOVE:
		{

			x = LOWORD(lParam);
			y = HIWORD(lParam);
			if (toggle>=0)
			{

				if (toggle == 0)
				{
					car[toggle].x = x;
					car[toggle].y = y;
					car[12].x = x;
					car[12].y = y;
				}
				else
				{
					car[toggle].x = x;
					car[toggle].y = y;
				}

			}
			InvalidateRgn(okno, NULL, true);
			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC kontekst;
			if (!scaled)
			{
				skalujPunkty();
				scaled = true;
			}


			kontekst = BeginPaint(okno, &paint);
		
			// MIEJSCE NA KOD GDI

			//HPEN pioro = CreatePen(PS_SOLID, 10, RGB(255,0,0));
			HBRUSH pedzel1 = CreateSolidBrush(RGB(255,0,0));
			HBRUSH pedzel2 = CreateSolidBrush(RGB(0,0,255));
			HBRUSH pedzel3 = CreateSolidBrush(RGB(0,255,0));
			HBRUSH pedzel4 = CreateSolidBrush(RGB(255,255,0));
			HBRUSH pedzel5 = CreateSolidBrush(RGB(255,0,255));
			HBRUSH pedzel6 = CreateSolidBrush(RGB(0,255,255));


			POINT k1[4];
			POINT k2[8];

			initLetter(k1,k2);

		

			SelectObject(kontekst, pedzel3);
			/*Pie(kontekst, 0, 0, 400, 400, 0, 0, 0, 1000);
			SelectObject(kontekst, pedzel4);
			Pie(kontekst, 0, 0, 400, 400, 0, 1000, 1000,1000);
			SelectObject(kontekst, pedzel5);
			Pie(kontekst, 0, 0, 400, 400, 1000, 1000, 1000,0);
			SelectObject(kontekst, pedzel6);
			Pie(kontekst, 0, 0, 400, 400, 1000, 0, 0,0);
			SelectObject(kontekst, pedzel1);
			Polygon(kontekst, k1, 4);
			SelectObject(kontekst, pedzel2);
			Polygon(kontekst, k2, 8);*/

			//MoveToEx(kontekst, 100, 300, NULL);
			//LineTo(kontekst, 800, 300);
			PolyBezier(kontekst, car, 13);
			for (int i = 0; i < 12; i++)
				Ellipse(kontekst, car[i].x, car[i].y, car[i].x + 10, car[i].y + 10);

			DeleteObject(pedzel3);
			//DeleteObject(pioro);




			// utworzenie obiektu umo�liwiaj�cego rysowanie przy u�yciu GDI+
			// (od tego momentu nie mo�na u�ywa� funkcji GDI
			Graphics grafika(kontekst);
			
			// MIEJSCE NA KOD GDI+
			Pen pioro1(Color::Green, 3);
			
			//curvePoints[0] = Point{ 100,100 };
			//curvePoints[1] = Point{ 150,150 };

			if(!isClosed)
				grafika.DrawCurve(&pioro1, curvePoints, nr);
			else
				grafika.DrawClosedCurve(&pioro1, curvePoints, nr);

			


			// utworzenie czcionki i wypisanie tekstu na ekranie
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(100.0f, 400.0f);
			SolidBrush  solidBrush(Color(255, 0, 0, 255));

			grafika.DrawString(L".", -1, &font, pointF, &solidBrush);

			EndPaint(okno, &paint);

			return 0;
		}
  	
	case WM_DESTROY: //obowi�zkowa obs�uga meldunku o zamkni�ciu okna
		PostQuitMessage (0) ;
		return 0;
    
	default: //standardowa obs�uga pozosta�ych meldunk�w
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
