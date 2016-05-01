#include "stdafx.h"
#include "Gif.h"


CGif::CGif()
{
}


CGif::~CGif()
{
}

void CombineGif(CFile* pFile, CString& strgifName, WORD beginSize)
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
		/*gif.Read(&byte, 1);
		if (byte == 0x21)
		{
			pFile->Write(&byte, 1);
			GIFCONTROL gifcontrol;
			gif.Read(&gifcontrol, sizeof(GIFCONTROL));
			gifcontrol.m_wDelayTime = DELAYTIME;
			pFile->Write(&gifcontrol, sizeof(GIFCONTROL));
			while (gif.Read(&byte, 1) > 0)
			{
				pFile->Write(&byte, 1);
			}
			break;
		}*/

	delete[] pbuf;
	gif.Close();
}