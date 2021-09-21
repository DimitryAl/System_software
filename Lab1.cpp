#include <windows.h>        // заголовочный файл, содержащий функции API
#include <string>


LRESULT CALLBACK WindowProcess(HWND,// дескриптор окошка
    UINT,// сообщение, посылаемое ОС
    WPARAM,// параметры
    LPARAM);// сообщений, для последующего обращения

LPSTR lpArgv;

int WINAPI WinMain(HINSTANCE hInst, 
    HINSTANCE hPrevInst, 
    LPSTR pCommandLine,
    int nCommandShow)  
{    
    lpArgv = GetCommandLine();
    if (NULL == lpArgv)
    {
        return 0;
    }

    TCHAR className[] = "My class";
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
        MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
        return NULL;
    }
    hWindow = CreateWindow(className,
        "Symbol input program",
        WS_OVERLAPPEDWINDOW,
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
    ShowWindow(hWindow, nCommandShow);
    UpdateWindow(hWindow);
    while (GetMessage(&message, NULL, NULL, NULL)) { 
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    //LocalFree(lpArgv);
    return message.wParam;
}

LRESULT CALLBACK WindowProcess(HWND hWindow, // дескриптор окна
    UINT uMessage,  // идентификатор сообщения
    WPARAM wParameter,  // первый параметр сообщения
    LPARAM lParameter)  // второй параметр сообщения
{
    HDC hDeviceContext; 
    PAINTSTRUCT paintStruct;
    RECT rectPlace; 
    HFONT hFont;

    static PTCHAR text; //PTCHAR — указатель на тип TCHAR.
    static int size = 0;
    static int fontsize = 20;

    static int d = 0;
    static int e = 0;
    static int n = 0;

    switch (uMessage)
    {
    case WM_CREATE:
        text = lpArgv;
        size = strlen(text);
        break;
    case WM_PAINT:
        hDeviceContext = BeginPaint(hWindow, &paintStruct);
        GetClientRect(hWindow, &rectPlace);
        SetTextColor(hDeviceContext, NULL);
        hFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0,
            DEFAULT_CHARSET,
            0, 0, 0, VARIABLE_PITCH,
            "Arial Bold");
        SelectObject(hDeviceContext, hFont);
        if (wParameter != VK_RETURN)
            DrawText(hDeviceContext,
                (LPCSTR)text,
                size,
                &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWindow, &paintStruct);
        break;
    case WM_KEYDOWN:
        switch (wParameter)
        {
        case VK_DOWN:
            //уменьшался размер шрифта
            if (fontsize != 10)  fontsize = fontsize - 10;
            break;
        case VK_UP:
            //увеличивался размер шрифта
            if (fontsize != 200) fontsize += 10;
            break;
        default: break;
        }
        InvalidateRect(hWindow, NULL, TRUE);
        break;
    case WM_CHAR:
        switch (wParameter)
        {
        case VK_RETURN:
            size = 0;
            break;
        case VK_BACK:
            if (size != 0)
                size--;
            break;
        case 0x2B:
            if (fontsize != 200) fontsize += 10;
            break;
        case 0x2D:
            if (fontsize != 10) fontsize -= 10;
            break;
        default:
            text[size] = (char)wParameter;
            size++;
            break;
        }
        InvalidateRect(hWindow, NULL, TRUE);
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