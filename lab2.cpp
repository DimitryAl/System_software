#include <windows.h>

#define CHANGE_TITLE


LRESULT CALLBACK WindowProcess(HWND,// дескриптор окошка
    UINT,// сообщение, посылаемое ОС
    WPARAM,// параметры
    LPARAM);// сообщений, для последующего обращения

void AddControls(HWND);
void AddButton(HWND hWindow);

HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, // указатель на текущий экземпляр
    HINSTANCE hPrevInst, /// указатель на предыдущйи запушенный экземпляр
    LPSTR pCommandLine, // нужен для запуска окошка в режиме командной строки
    int nCommandShow)  // режим отображения окна
{
    TCHAR className[] = "My class";
    HWND hWindow; // создаём дескриптор будущего окошка
    MSG message; // создём экземпляр структуры MSG для обработки сообщений
    WNDCLASSEX windowClass;// создаём экземпляр, для обращения к членам класса WNDCLASSEX
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
        // в случае отсутствия регистрации класса:
        MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
        return NULL;
    }
    // Функция, создающая окошко:
    hWindow = CreateWindow(className,
        "Lab2",
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
        MessageBox(NULL, "Не получилось создать окно!", "Ошибка", MB_OK);
        return NULL;
    }
    ShowWindow(hWindow, nCommandShow);// отображаем окошко
    UpdateWindow(hWindow);// обновляем окошко
    while (GetMessage(&message, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        TranslateMessage(&message);// интерпретируем сообщения
        DispatchMessage(&message);// передаём сообщения обратно ОС
    }
    return message.wParam;// возвращаем код выхода из приложения
}

LRESULT CALLBACK WindowProcess(HWND hWindow, // дескриптор окна
    UINT uMessage,  // идентификатор сообщения
    WPARAM wParameter,
    LPARAM lParameter)  // второй параметр сообщения
{
    HDC hDeviceContext; // создаём дескриптор ориентации текста на экране
    PAINTSTRUCT paintStruct;// структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
    RECT rectPlace; // стр-ра, определяющая размер клиентской области
    HFONT hFont; //дескриптор шрифта

    static PTCHAR text; //PTCHAR — указатель на тип TCHAR.
    static int size = 0;
    static int fontsize = 100;

    switch (uMessage)
    {
    case WM_CREATE:
        AddButton(hWindow);
        AddControls(hWindow);
        break;
    /*case WM_COMMAND:
        switch (wParameter)
        {
        case CHANGE_TITLE:
            wchar_t text[100];
            GetWindowTextW(hEdit, text, 100);
            break;
        }*/
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
        "BUTTON",  // Predefined class; Unicode assumed 
        "Send",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        200,         // x position 
        250,         // y position 
        100,        // Button width
        50,        // Button height
        hWindow,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
}

void AddControls(HWND hWindow) 
{
    //HWND hEdit;

    CreateWindow("static",
        "Enter your text below:",
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
        200,
        100,
        100,
        50,
        hWindow,
        NULL,
        NULL,
        NULL);

    hEdit = CreateWindow( "Edit",
        "Your text",
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        200,
        150,
        100,
        50,
        hWindow,
        NULL,
        NULL,
        NULL);

}