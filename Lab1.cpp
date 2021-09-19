#include <windows.h>        // заголовочный файл, содержащий функции API
//#include <shellapi.h>
#include <string>


LRESULT CALLBACK WindowProcess(HWND,// дескриптор окошка
    UINT,// сообщение, посылаемое ОС
    WPARAM,// параметры
    LPARAM);// сообщений, для последующего обращения

LPSTR lpArgv;

int WINAPI WinMain(HINSTANCE hInst, // указатель на текущий экземпляр
    HINSTANCE hPrevInst, /// указатель на предыдущйи запушенный экземпляр
    LPSTR pCommandLine, // нужен для запуска окошка в режиме командной строки
    int nCommandShow)  // режим отображения окна
{
    
    lpArgv = GetCommandLine();
    if (NULL == lpArgv)
    {
        return 0;
    }

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
    // Функция, создающая окошко:O
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
    ShowWindow(hWindow, nCommandShow);// отображаем окошко
    UpdateWindow(hWindow);// обновляем окошко
    while (GetMessage(&message, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        TranslateMessage(&message);// интерпретируем сообщения
        DispatchMessage(&message);// передаём сообщения обратно ОС
    }
    //LocalFree(lpArgv);
    return message.wParam;// возвращаем код выхода из приложения
}

LRESULT CALLBACK WindowProcess(HWND hWindow, // дескриптор окна
    UINT uMessage,  // идентификатор сообщения
    WPARAM wParameter,  // первый параметр сообщения
    LPARAM lParameter)  // второй параметр сообщения
{
    HDC hDeviceContext; // создаём дескриптор ориентации текста на экране
    PAINTSTRUCT paintStruct;// структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
    RECT rectPlace; // стр-ра, определяющая размер клиентской области
    HFONT hFont; //дескриптор шрифта

    static PTCHAR text; //PTCHAR — указатель на тип TCHAR.
    static int size = 0;
    static int fontsize = 20;

    switch (uMessage)
    {
    case WM_CREATE:
        
        //text = (PTCHAR)GlobalAlloc(GPTR, 50000 * sizeof(TCHAR));
        text = lpArgv;
        size = strlen(text);
        break;
    case WM_PAINT: // если нужно нарисовать, то:
        hDeviceContext = BeginPaint(hWindow, &paintStruct);// инициализируем контекст устройства
        GetClientRect(hWindow, &rectPlace);// получаем ширину и высоту области для рисования
        SetTextColor(hDeviceContext, NULL);// устанавливаем цвет контекстного устройства
        //мы меняем шрифт, и размер буквы.
        hFont = CreateFont(fontsize, 0, 0, 0, 0, 0, 0, 0,
            DEFAULT_CHARSET,
            0, 0, 0, VARIABLE_PITCH,
            "Arial Bold");
        SelectObject(hDeviceContext, hFont);
        if (wParameter != VK_RETURN)
            DrawText(hDeviceContext,
                (LPCSTR)text,
                //lpArgv,
                size,
                //-1,
                &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWindow, &paintStruct);
        break;
    case WM_KEYDOWN:
        switch (wParameter)
        {
        case VK_DOWN:
            //уменьшался размер шрифта
            if (fontsize != 10)
                fontsize = fontsize - 10;
            break;
        case VK_UP:
            //увеличивался размер шрифта
            if (fontsize != 200)
                fontsize += 10;
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