#include <windows.h>        // ������������ ����, ���������� ������� API
//#include <shellapi.h>
#include <string>


LRESULT CALLBACK WindowProcess(HWND,// ���������� ������
    UINT,// ���������, ���������� ��
    WPARAM,// ���������
    LPARAM);// ���������, ��� ������������ ���������

LPSTR lpArgv;

int WINAPI WinMain(HINSTANCE hInst, // ��������� �� ������� ���������
    HINSTANCE hPrevInst, /// ��������� �� ���������� ���������� ���������
    LPSTR pCommandLine, // ����� ��� ������� ������ � ������ ��������� ������
    int nCommandShow)  // ����� ����������� ����
{
    
    lpArgv = GetCommandLine();
    if (NULL == lpArgv)
    {
        return 0;
    }

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
    // �������, ��������� ������:O
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
        MessageBox(NULL, "�� ���������� ������� ����!", "������", MB_OK);
        return NULL;
    }
    ShowWindow(hWindow, nCommandShow);// ���������� ������
    UpdateWindow(hWindow);// ��������� ������
    while (GetMessage(&message, NULL, NULL, NULL)) { // ��������� ��������� �� �������, ���������� ��-�����, ��
        TranslateMessage(&message);// �������������� ���������
        DispatchMessage(&message);// ������� ��������� ������� ��
    }
    //LocalFree(lpArgv);
    return message.wParam;// ���������� ��� ������ �� ����������
}

LRESULT CALLBACK WindowProcess(HWND hWindow, // ���������� ����
    UINT uMessage,  // ������������� ���������
    WPARAM wParameter,  // ������ �������� ���������
    LPARAM lParameter)  // ������ �������� ���������
{
    HDC hDeviceContext; // ������ ���������� ���������� ������ �� ������
    PAINTSTRUCT paintStruct;// ���������, ���-��� ���������� � ���������� ������� (�������, ���� � ��)
    RECT rectPlace; // ���-��, ������������ ������ ���������� �������
    HFONT hFont; //���������� ������

    static PTCHAR text; //PTCHAR � ��������� �� ��� TCHAR.
    static int size = 0;
    static int fontsize = 20;

    switch (uMessage)
    {
    case WM_CREATE:
        
        //text = (PTCHAR)GlobalAlloc(GPTR, 50000 * sizeof(TCHAR));
        text = lpArgv;
        size = strlen(text);
        break;
    case WM_PAINT: // ���� ����� ����������, ��:
        hDeviceContext = BeginPaint(hWindow, &paintStruct);// �������������� �������� ����������
        GetClientRect(hWindow, &rectPlace);// �������� ������ � ������ ������� ��� ���������
        SetTextColor(hDeviceContext, NULL);// ������������� ���� ������������ ����������
        //�� ������ �����, � ������ �����.
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
            //���������� ������ ������
            if (fontsize != 10)
                fontsize = fontsize - 10;
            break;
        case VK_UP:
            //������������ ������ ������
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