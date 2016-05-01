#include "stdafx.h"
#include "Gif.h"
#include <gdiplus.h>  
#include <stdio.h>  
using namespace Gdiplus;
#pragma comment(lib,"gdiplus")  

CGif::CGif()
{
}


CGif::~CGif()
{
}

void CGif::CombineGif(CFile* pFile, CString& strgifName, WORD beginSize)
{
	pFile->Seek(-1, CFile::current);
	CFile gif;
	gif.Open(strgifName, CFile::modeRead | CFile::typeBinary);
	BYTE byte = 0x00;
	BYTE* pbuf = new BYTE[beginSize];
	BYTE Databuf[10240];

	gif.Read(pbuf, beginSize);
	byte = 0x21;
	pFile->Write(&byte, 1);
	GIFCONTROL gifcontrol;
	gif.Read(&gifcontrol, sizeof(GIFCONTROL));
	gifcontrol.m_wDelayTime = DELAYTIME;
	pFile->Write(&gifcontrol, sizeof(GIFCONTROL));
	WORD wsize = 0;
	while ((wsize = gif.Read(Databuf, sizeof(Databuf))) == sizeof(Databuf))
	{
		pFile->Write(Databuf, wsize);
	}
	pFile->Write(Databuf, wsize);
	delete[] pbuf;
	gif.Close();
}

int CGif::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders  
	UINT  size = 0;         // size of the image encoder array in bytes  

	ImageCodecInfo* pImageCodecInfo = NULL;

	//1.获取GDI+支持的图像格式编码器种类数以及ImageCodecInfo数组的存放大小  
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure  

	//2.为ImageCodecInfo数组分配足额空间  
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure  

	//3.获取所有的图像编码器信息  
	GetImageEncoders(num, size, pImageCodecInfo);

	//4.查找符合的图像编码器的Clsid  
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success  
		}
	}

	//5.释放步骤3分配的内存  
	free(pImageCodecInfo);
	return -1;  // Failure  
}

bool CGif::GetGifPic(CString strBmpPathName, CString strGifPathName)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	//1.初始化GDI+，以便后续的GDI+函数可以成功调用  
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CLSID   encoderClsid;
	Status  stat;
	//2.创建Image对象并加载图片  
	Image*   image = new Image(strBmpPathName);
	//3. Get the CLSID of the gif encoder.  
	GetEncoderClsid(L"image/gif", &encoderClsid);
	//4.调用Image.Save方法进行图片格式转换，并把步骤3)得到的图像编码器Clsid传递给它  
	stat = image->Save(strGifPathName, &encoderClsid, NULL);
	if (stat == Ok)
		//5.释放Image对象  
		delete image;
	//6.清理所有GDI+资源  
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