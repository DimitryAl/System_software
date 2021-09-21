#include <windows.h>
#include <string>

// Идентификаторы кнопок
#define IDB_Button1 1
#define IDB_Button2 2

LRESULT CALLBACK WindowProcess(HWND,// дескриптор окошка
    UINT,// сообщение, посылаемое ОС
    WPARAM,// параметры
    LPARAM);// сообщений, для последующего обращения

void AddControls(HWND hWindow);
void AddButton(HWND hWindow);

HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, // указатель на текущий экземпляр
    HINSTANCE hPrevInst, /// указатель на предыдущйи запушенный экземпляр
    LPSTR pCommandLine, // нужен для запуска окошка в режиме командной строки
    int nCommandShow)  // режим отображения окна
{
    TCHAR className[] = L"My class2";
    HWND hWindow; 
    MSG message; 
    WNDCLASSEX windowClass;
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcess;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = className;
    windowClass.cbWndExtra = NULL;
    windowClass.cbClsExtra = NULL;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.hInstance = hInst;
    if (!RegisterClassEx(&windowClass)) {
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL;
    }
    hWindow = CreateWindow(className,
        L"Lab2",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        NULL,
        CW_USEDEFAULT,
        NULL,
        (HWND)NULL,
        NULL,
        HINSTANCE(hInst),
        NULL);
    if (!hWindow) {
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }
    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);
    while (GetMessage(&message, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        TranslateMessage(&message);// интерпретируем сообщения
        DispatchMessage(&message);// передаём сообщения обратно ОС
    }
    return message.wParam;// возвращаем код выхода из приложения
}

LRESULT CALLBACK WindowProcess(HWND hWindow, 
    UINT uMessage,  
    WPARAM wParameter,
    LPARAM lParameter)  
{
    HDC hDeviceContext; // создаём дескриптор ориентации текста на экране
    PAINTSTRUCT paintStruct;// структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
    RECT rectPlace; // стр-ра, определяющая размер клиентской области
    HFONT hFont; //дескриптор шрифта

    static PTCHAR text; //PTCHAR — указатель на тип TCHAR.
    //static int size = 0;
    static int fontsize = 100;

    switch (uMessage)
    {
    case WM_CREATE:
        AddButton(hWindow);
        AddControls(hWindow);
        break;
    case WM_COMMAND:

        TCHAR text[256];
        GetWindowTextW(hEdit, text, 100);

        if (wParameter == IDB_Button1)
        {
            MessageBox(hWindow, L"Trying to start program", L"Program starting...", MB_OK);

            //start first program
            STARTUPINFO cif;
            ZeroMemory(&cif, sizeof(STARTUPINFO));
            PROCESS_INFORMATION pi;
          
            BOOL myProc = CreateProcess(L"E:\\Dimas\\Study\\MAI\\7 term\\System software\\labs\\1\\Test4\\Debug\\Test4.exe",
                text,
                NULL, 
                NULL, 
                FALSE, 
                NULL,
                NULL,
                NULL, 
                &cif, 
                &pi);
            if (myProc != TRUE)
            {
                MessageBox(hWindow, L"Failed to start program", L"Program starting...", MB_OK);
            }          
        }
        if (wParameter == IDB_Button2)
        {
            HWND hwndServer = NULL;
            hwndServer = FindWindowEx(0, 0, L"My class", 0);
            DWORD b = GetLastError();
            if (!hwndServer) {
                MessageBox(NULL, L"Can't find!", L"Test4.exe", MB_OK);
            }
            else { 
                int len = 256;  //размер text
                for (int i = 0; i < len + 1; i++) {
                    char symbol = (char)text[i];
                    if (symbol == '\0') break;
                    LRESULT test = SendMessage(hwndServer, WM_CHAR, symbol, 0);
                }
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(NULL);
        GlobalFree((HGLOBAL)text);
        break;
    default:
        return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
    }
    return NULL;
}

void AddButton(HWND hWindow)
{
    HWND hwndButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Start program",      // Button text 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Styles 
        200,         // x position 
        250,         // y position 
        200,        // Button width
        50,        // Button height
        hWindow,     // Parent window
        (HMENU) IDB_Button1,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    HWND hwndButton2 = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Send message",      // Button text 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // Styles 
        500,         // x position 
        250,         // y position 
        200,        // Button width
        50,        // Button height
        hWindow,     // Parent window
        (HMENU)IDB_Button2,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

}

void AddControls(HWND hWindow) 
{
    CreateWindow(L"static",
        L"Enter data for first program:",
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        200,
        100,
        200,
        50,
        hWindow,
        NULL,
        NULL,
        NULL);

    hEdit = CreateWindow( L"Edit",
        L"Your text",
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        200,
        150,
        200,
        50,
        hWindow,
        NULL,
        NULL,
        NULL);

}