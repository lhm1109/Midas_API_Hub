#if !defined(__CMSectSODDeckStiffenerDlgUtil_H__)
#define __CMSectSODDeckStiffenerDlgUtil_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCMSectSODDeckStiffenerDlgUtil
{
public:
	CCMSectSODDeckStiffenerDlgUtil();
	virtual ~CCMSectSODDeckStiffenerDlgUtil();

	static void GetCbxDeckPartItem(T_SECT_D& SectD, int nDeckPos, std::vector<CString>* pArStrCbxDeckPartItem);
	static void GetDeckName(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, CString* pDeckName);
	static void GetDefStiffName(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, CString* pDefStiffName);
	static void GetDefStiffPos(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int* pDefStiffPos);
	static void GetStiffPosItemList(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, CString* pStiffPosItemList);
	static void GetStiffDirItemList(T_SECT_D& SectD, int nDeckPos, CString* pStiffPosItemList);
	static CString GetBitmapID(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
protected:
	static void GetSODStiffDlgData(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, std::vector<CString>* pArStrCbxDeckPartItem, CString* pDeckName, CString* pDefStiffName, int* pDefStiffPos, CString* pStiffPosItemList);
	static void GetSODStiffDirData(T_SECT_D& SectD, int nDeckPos, CString* pStiffPosItemList);
	static void GetSODStiffDlgDataMCell(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, std::vector<CString>* pArStrCbxDeckPartItem, CString* pDeckName, CString* pDefStiffName, int* pDefStiffPos, CString* pStiffPosItemList);
	static void GetSODStiffDlgDataMCellGen(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, std::vector<CString>* pArStrCbxDeckPartItem, CString* pDeckName, CString* pDefStiffName, int* pDefStiffPos, CString* pStiffPosItemList);
	static CString GetBitmapIDMCell(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellGen(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellRebar1(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellRebar2(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellClosed(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellClosedJ1J1(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellClosedJ1J2(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellClosedJ2J1(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
	static CString GetBitmapIDMCellClosedJ2J2(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos);
};

#endif // !defined(__CMSectSODDeckStiffenerDlgUtil_H__)
