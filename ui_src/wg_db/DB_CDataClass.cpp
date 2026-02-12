// DB_CDataClass.cpp: implementation of the DB_CDataClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_CDataClass.h"
#include "..\wg_base\TextFileOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// DB_CData2Stream
BOOL DB_CData2Stream::STR(CString& rcsVal, unsigned char* pStream, int& nStart, const int nEnd)
{
	std::string arrStrTmp = CW2A(rcsVal);
	int len = arrStrTmp.length();

	T_VD_DATA vdStr;
	vdStr.Set(sizeof(char), len, (void*)arrStrTmp.c_str());

	void* pAddr = (void*)(arrStrTmp.c_str());
	if (!VDATA(vdStr, pStream, nStart, nEnd)) return FALSE;
	int nBytes = vdStr.nSize*vdStr.nCount;
	if (nBytes > 0)
	{
		if (nStart+nBytes > nEnd) return FALSE;
		memcpy(pStream+nStart, pAddr, nBytes); nStart += nBytes;
	}

	return TRUE;
}

BOOL DB_CData2Stream::FXD(void* pAddr, int nBytes, unsigned char* pStream, int& nStart, const int nEnd)
{
	if (nStart+nBytes > nEnd) return FALSE;
	memcpy(pStream+nStart, pAddr, nBytes); nStart += nBytes;
	return TRUE;
}

BOOL DB_CData2Stream::VDATA(T_VD_DATA& rvdVal, unsigned char* pStream, int& nStart, const int nEnd)
{
	int nBytes = sizeof(T_VD_DATA);
	if (nStart+nBytes > nEnd) return FALSE;
	memcpy(pStream+nStart, &rvdVal, nBytes); nStart += nBytes;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// DB_CStream2Data
BOOL DB_CStream2Data::STR(CString& rcsVal, unsigned char* pStream, int& nStart, const int nEnd)
{
	CStringA rcsValA;
	T_VD_DATA vdStr;
	if (!VDATA(vdStr, pStream, nStart, nEnd)) return FALSE;
	int nBytes = vdStr.nSize*vdStr.nCount;
	if (nBytes > 0)
	{
		if (nStart+nBytes > nEnd) return FALSE;

		std::string strTmp;
		strTmp.resize(nBytes);
		void* pAddr = (void*)strTmp.c_str();
		memcpy(pAddr, pStream + nStart, nBytes); nStart += nBytes;

		rcsVal = CA2W(strTmp.c_str());
	}
	return TRUE;
}

BOOL DB_CStream2Data::FXD(void* pAddr, int nBytes, unsigned char* pStream, int& nStart, const int nEnd)
{
	if (nStart+nBytes > nEnd) return FALSE;
	memcpy(pAddr, pStream+nStart, nBytes); nStart += nBytes;
	return TRUE;
}

BOOL DB_CStream2Data::VDATA(T_VD_DATA& rvdVal, unsigned char* pStream, int& nStart, const int nEnd)
{
	int nBytes = sizeof(T_VD_DATA);
	if (nStart+nBytes > nEnd) return FALSE;
	memcpy(&rvdVal, pStream+nStart, nBytes); nStart += nBytes;
	if (!rvdVal.IsValid()) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// DB_TDataRW
BOOL DB_TDataRW::Stream2DataFixed(const void* pStream, void* pKey, int nKeySize, void* pData, int nDataSize, int nMaxSize)
{
	unsigned char* pAddr = (unsigned char*)pStream;
	memcpy(pKey, pAddr, nKeySize); pAddr += nKeySize;
	memcpy(pData, pAddr, nDataSize);
	ASSERT(nKeySize + nDataSize == nMaxSize);
	return TRUE;
}

BOOL DB_TDataRW::Data2StreamFixed(void* pStream, const void* pKey, int nKeySize, const void* pData, int nDataSize, int nMaxSize)
{
	unsigned char* pAddr = (unsigned char*)pStream;
	memcpy(pAddr, pKey, nKeySize); pAddr += nKeySize;
	memcpy(pAddr, pData, nDataSize);
	ASSERT(nKeySize + nDataSize == nMaxSize);
	return TRUE;
}

BOOL DB_TDataRW::Stream2VD(const void *pStream, T_VD_DATA_PLIST* paVD, int nMaxSize)
{
	// 64bit에서 T_VD_DATA::pAddr -> nDymmy로 변경하면서 아래 함수는 무효.
	// 이 함수 호출하는 곳이 없으므로 이렇게 처리함.  by Hsshim.
	ASSERT(0);  
	return FALSE;

//   if (paVD == 0) return TRUE;
//   unsigned char* pAddr = (unsigned char*)pStream;
// 
//   int nTotal = 0;
//   UINT nBytes;
//   int nSize = paVD->GetSize();
//   for (int i = 0; i < nSize; i++)
//   {
//     if ((*paVD)[i]->IsValid() == FALSE)
//         return FALSE;
//     nBytes = (*paVD)[i]->nSize * (*paVD)[i]->nCount;
//     if (nBytes == 0) continue;
//     nTotal += nBytes;
//     if (nTotal > nMaxSize) return FALSE;
//     memcpy((*paVD)[i]->pAddr, pAddr, nBytes); pAddr += nBytes;
//   }
//   return TRUE;
}

BOOL DB_TDataRW::VD2Stream(void *pStream, T_VD_DATA_PLIST* paVD, int nMaxSize)
{
	// 64bit에서 T_VD_DATA::pAddr -> nDymmy로 변경하면서 아래 함수는 무효.
	// 이 함수 호출하는 곳이 없으므로 이렇게 처리함.  by Hsshim.
	ASSERT(0);  
	return FALSE;

//   if (paVD == 0) return TRUE;
//   unsigned char* pAddr = (unsigned char*)pStream;
// 
//   int nTotal = 0;
//   UINT nBytes;
//   int nSize = paVD->GetSize();
//   for (int i = 0; i < nSize; i++)
//   {
//     nBytes = (*paVD)[i]->nSize * (*paVD)[i]->nCount;
//     if (nBytes == 0) continue;
//     nTotal += nBytes;
//     if (nTotal > nMaxSize) return FALSE;
//     memcpy(pAddr, (*paVD)[i]->pAddr, nBytes); pAddr += nBytes;
//   }
//   return TRUE;
}



