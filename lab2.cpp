#include <windows.h>

#define CHANGE_TITLE


LRESULT CALLBACK WindowProcess(HWND,// ���������� ������
    UINT,// ���������, ���������� ��
    WPARAM,// ���������
    LPARAM);// ���������, ��� ������������ ���������

void AddControls(HWND);
void AddButton(HWND hWindow);

HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, // ��������� �� ������� ���������
    HINSTANCE hPrevInst, /// ��������� �� ���������� ���������� ���������
    LPSTR pCommandLine, // ����� ��� ������� ������ � ������ ��������� ������
    int nCommandShow)  // ����� ����������� ����
{
    TCHAR className[] = "My class";
    HWND hWindow; // ������ ���������� �������� ������
    MSG message; // ����� ��������� ��������� MSG ��� ��������� ���������
    WNDCLASSEX windowClass;// ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
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
        // � ������ ���������� ����������� ������:
        MessageBox(NULL, "�� ���������� ���������������� �����!", "������", MB_OK);
        return NULL;
    }
    // �������, ��������� ������:
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
        MessageBox(NULL, "�� ���������� ������� ����!", "������", MB_OK);
        return NULL;
    }
    ShowWindow(hWindow, nCommandShow);// ���������� ������
    UpdateWindow(hWindow);// ��������� ������
    while (GetMessage(&message, NULL, NULL, NULL)) { // ��������� ��������� �� �������, ���������� ��-�����, ��
        TranslateMessage(&message);// �������������� ���������
        DispatchMessage(&message);// ������� ��������� ������� ��
    }
    return message.wParam;// ���������� ��� ������ �� ����������
}

LRESULT CALLBACK WindowProcess(HWND hWindow, // ���������� ����
    UINT uMessage,  // ������������� ���������
    WPARAM wParameter,
    LPARAM lParameter)  // ������ �������� ���������
{
    HDC hDeviceContext; // ������ ���������� ���������� ������ �� ������
    PAINTSTRUCT paintStruct;// ���������, ���-��� ���������� � ���������� ������� (�������, ���� � ��)
    RECT rectPlace; // ���-��, ������������ ������ ���������� �������
    HFONT hFont; //���������� ������

    static PTCHAR text; //PTCHAR � ��������� �� ��� TCHAR.
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