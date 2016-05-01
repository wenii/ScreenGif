#include "stdafx.h"
#include "GlobalData.h"

CGlobalData::CGlobalData()
{
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