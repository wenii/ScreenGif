#pragma onece
#ifdef SCREENGIF_API
#else
#define SCREENGIF_API _declspec(dllimport)
#endif
#define HOTKEYID 999
#define WM_USEHELP 888
#define UM_BEGINPOINT	WM_USER + 1
#define UM_ENDPOINT		WM_USER + 2
#define UM_MOUSEMOVE	WM_USER + 3
#define UK_REGION		WM_USER + 4
#define WM_USER_NOTIFYICON WM_USER+5
#define UM_PROGRESS     WM_USER + 6
#define UM_THREADEND    WM_USER + 7

//设置鼠标钩子
SCREENGIF_API void SetHook(HWND hWnd, HHOOK& hmouse);
//设置键盘钩子
SCREENGIF_API void SetKeyHook(HWND hWnd, HHOOK& hkey);