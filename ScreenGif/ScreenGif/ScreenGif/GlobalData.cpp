#include "stdafx.h"
#include "GlobalData.h"
#include "Mouse.h"

#include <gdiplus.h>  
#include <stdio.h>  
using namespace Gdiplus;
#pragma comment(lib,"gdiplus")  

CGlobalData::CGlobalData()
{
	m_rc.left = 0;
	m_rc.top = 0;
	m_rc.right = 0;
	m_rc.bottom = 0;
}


CGlobalData::~CGlobalData()
{
}
CString CGlobalData::GetCurrentFilePath()
{
	CString strFilePath;
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, (LPWCH)exeFullPath, MAX_PATH);
	strFilePath.Format(_T("%s"), exeFullPath);
	return strFilePath;
}
int CGlobalData::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders  
	UINT  size = 0;         // size of the image encoder array in bytes  

	ImageCodecInfo* pImageCodecInfo = NULL;

	//2.获取GDI+支持的图像格式编码器种类数以及ImageCodecInfo数组的存放大小  
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure  

	//3.为ImageCodecInfo数组分配足额空间  
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure  

	//4.获取所有的图像编码器信息  
	GetImageEncoders(num, size, pImageCodecInfo);

	//5.查找符合的图像编码器的Clsid  
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success  
		}
	}

	//6.释放步骤3分配的内存  
	free(pImageCodecInfo);
	return -1;  // Failure  
}

bool CGlobalData::GetGifPic(CString strBmpPathName, CString strGifPathName)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	//1.初始化GDI+，以便后续的GDI+函数可以成功调用  
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID   encoderClsid;
	Status  stat;

	//7.创建Image对象并加载图片  
	Image*   image = new Image(strBmpPathName);

	// Get the CLSID of the gif encoder.  
	GetEncoderClsid(L"image/gif", &encoderClsid);

	//8.调用Image.Save方法进行图片格式转换，并把步骤3)得到的图像编码器Clsid传递给它  
	stat = image->Save(strGifPathName, &encoderClsid, NULL);

	if (stat == Ok)
	//9.释放Image对象  
	delete image;
	//10.清理所有GDI+资源  
	GdiplusShutdown(gdiplusToken);
	if (stat == Ok)
	{
		return true;

	}
	else
	{
		return false;
	}
}