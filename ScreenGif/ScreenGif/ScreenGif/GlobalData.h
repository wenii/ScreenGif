#pragma once
class CGlobalData
{
public:
	CGlobalData();
	~CGlobalData();
public:
	RECT m_rc;	//ÆÁÄ»½ØÍ¼×ø±ê
	RECT m_AllScreen; //È«ÆÁ³ß´ç
public:
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	static bool GetGifPic(CString strBmpPathName, CString strGifPathName);
	static CString GetCurrentFilePath();
};

