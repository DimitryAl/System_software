#include <windows.h>
#include <string>

// �������������� ������
#define IDB_Button1 1

LRESULT CALLBACK WindowProcess(HWND,// ���������� ������
    UINT,// ���������, ���������� ��
    WPARAM,// ���������
    LPARAM);// ���������, ��� ������������ ���������

void AddControls(HWND hWindow);
void AddButton(HWND hWindow);

HWND hEdit;

int WINAPI WinMain(HINSTANCE hInst, // ��������� �� ������� ���������
    HINSTANCE hPrevInst, /// ��������� �� ���������� ���������� ���������
    LPSTR pCommandLine, // ����� ��� ������� ������ � ������ ��������� ������
    int nCommandShow)  // ����� ����������� ����
{
    TCHAR className[] = L"My class";
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
        MessageBox(NULL, L"�� ���������� ���������������� �����!", L"������", MB_OK);
        return NULL;
    }
    // �������, ��������� ������:
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
        MessageBox(NULL, L"�� ���������� ������� ����!", L"������", MB_OK);
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
    case WM_COMMAND:
        if (wParameter == IDB_Button1)
        {
            TCHAR text[256];
            GetWindowTextW(hEdit, text, 100);
            MessageBox(hWindow, L"btn1", L"Message WM_COMMAND", MB_OK);
            //start first program
            STARTUPINFO cif;
            ZeroMemory(&cif, sizeof(STARTUPINFO));
            PROCESS_INFORMATION pi;
          
            BOOL myProc = CreateProcess(L"E:\\Dimas\\Study\\MAI\\7 term\\System software\\labs\\1\\Test4\\x64\\Debug\\Test4.exe",
                text,
                NULL, 
                NULL, 
                FALSE, 
                NULL,
                NULL,
                NULL, 
                &cif, 
                &pi);
            if (myProc == TRUE)
            {
                Sleep(5000);				// ���������
                TerminateProcess(pi.hProcess, NO_ERROR);	// ������ �������
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
}

void AddControls(HWND hWindow) 
{
    //HWND hEdit;

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
        WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | /*ES_MULTILINE |*/ ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        200,
        150,
        200,
        50,
        hWindow,
        NULL,
        NULL,
        NULL);

}