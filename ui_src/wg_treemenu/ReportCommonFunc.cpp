// ReportCommonFunc.cpp: implementation of the CReportCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReportCommonFunc.h"
#include "LoadWordLib.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_FileCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringArray CReportCommonFunc::m_aAllProjStr;

CReportCommonFunc::CReportCommonFunc()
{
	InitAllProjStr();
}

CReportCommonFunc::~CReportCommonFunc()
{
	m_aAllProjStr.RemoveAll();
}

void CReportCommonFunc::InsertReportHeaderFooter()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	BOOL bIsLoaded=FALSE;
	bIsLoaded = CLoadWordLib::GetInstance()->IsLoadedWordLibrary();
	if(bIsLoaded)
	{
		HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
		CString strHeader=_T(""), strFooter=_T("");
		if(!GetHeaderFooterStr(strHeader, strFooter))
		{
			hMod = NULL; return;
		}

		EditorHeader pEditorHeader=NULL;
		pEditorHeader = (EditorHeader) GetProcAddress(hMod, "EditorHeader");
		pEditorHeader((LPTSTR)(LPCTSTR)strHeader);
		
		EditorFooter pEditorFooter=NULL;
		pEditorFooter = (EditorFooter) GetProcAddress(hMod, "EditorFooter");
		pEditorFooter((LPTSTR)(LPCTSTR)strFooter);

		pEditorHeader=NULL;
		pEditorFooter=NULL;
		hMod = NULL;
	}
}

BOOL CReportCommonFunc::GetHeaderFooterStr(CString& strHeader, CString& strFooter)
{
	strHeader = _T("");
	strFooter = _T("");
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	
	T_UFTR_D UftrD;
	if(!pDoc->m_pAttrCtrl2->GetUftr(UftrD)) return FALSE;

	CArray<UINT,UINT> aHeader, aFooter;
	aHeader.SetSize(UftrD.nHeaderNum);
	aFooter.SetSize(UftrD.nFooterNum);
	for(int i=0; i<UftrD.nHeaderNum; i++)
		aHeader[i] = UftrD.aHeader[i];
	for(int i=0; i<UftrD.nFooterNum; i++)
		aFooter[i] = UftrD.aFooter[i];

	if(UftrD.nHeaderNum > 0) GetSelectedStr(aHeader, strHeader);
	if(UftrD.nFooterNum > 0) GetSelectedStr(aFooter, strFooter);

	if(strHeader.IsEmpty() && strFooter.IsEmpty()) return FALSE;

	return TRUE;
}

void CReportCommonFunc::GetSelectedStr(CArray<UINT,UINT>& aSelected, CString& strRes)
{
	strRes = _T("");
	if(aSelected.GetSize() <= 0) return;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	T_PJCF_D PjcfD; PjcfD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetPjcf(PjcfD)) PjcfD.Initialize();
	
	BOOL bNoInfo=FALSE;
	CString strPath = pDoc->GetPathName();
	bNoInfo = strPath.IsEmpty()? TRUE : FALSE;
	
	CFileCtrl file(strPath);
	CFileStatus fileStatus;
	file.GetFileStatus(fileStatus);
	
	int nIndex=0;
	CString str=_T(""), str2=_T("");
	for(int i=0; i<aSelected.GetSize(); i++)
	{
		nIndex = aSelected[i];
		switch(nIndex)
		{
		case D_UFTR_PROJECT_NAME:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strProject);
			break;
		case D_UFTR_REVISION:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strRevision);
			break;
		case D_UFTR_USER_NAME:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strUser);
			break;
		case D_UFTR_EMAIL:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strMail);
			break;
		case D_UFTR_ADDRESS:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strAddress);
			break;
		case D_UFTR_TELEPHONE:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strPhone);
			break;
		case D_UFTR_FAX:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strFax);
			break;
		case D_UFTR_CLIENT:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strClient);
			break;
		case D_UFTR_TITLE:
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], PjcfD.strTitle);
			break;
		case D_UFTR_FILE_NAME:
			if(bNoInfo) str2 = _T("");
			else        str2 = file.GetFileNameWithoutExtension();
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], str2);
			break;
		case D_UFTR_CREATED:
			if(bNoInfo) str2 = _T("");
			else        str2 = fileStatus.m_ctime.Format(_T("%d/%m/%Y  %H:%M"));
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], str2);
			break;
		case D_UFTR_DIRECTORY:
			if(bNoInfo) str2 = _T("");
			else
			{
				str2 = file.GetFilePath();
				int nLast = str2.GetLength()-1;
				if(str2.GetAt(nLast) == '\\') str2 = str2.Left(nLast);
			}
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], str2);
			break;
		case D_UFTR_MODIFIED:
			if(bNoInfo) str2 = _T("");
			else        str2 = fileStatus.m_mtime.Format(_T("%d/%m/%Y  %H:%M"));
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], str2);
			break;
			break;
		case D_UFTR_FILE_SIZE:
			if(bNoInfo) str2 = _T("");
			else
			{
				int nSize = fileStatus.m_size/1024;
				if(nSize <= 0)  str2.Format(_T("%d %s"), fileStatus.m_size, (fileStatus.m_size > 1) ? _T("bytes") : _T("byte"));
				else            str2.Format(_T("%d %s"), nSize, (nSize > 1) ? _T("Kbytes") : _T("Kbyte"));
			}
			str.Format(_T("%s : %s"), m_aAllProjStr[nIndex], str2);
			break;
			break;
		default:
			ASSERT(0); break;
		}
		if(i!=aSelected.GetSize()-1)
			str += _T("\n");
		
		strRes += str;
	}
}

void CReportCommonFunc::GetAllProjStr(CStringArray& aAllProjStr)
{
	InitAllProjStr();
	aAllProjStr.Copy(m_aAllProjStr);
}

void CReportCommonFunc::InitAllProjStr()
{
	m_aAllProjStr.RemoveAll();
	m_aAllProjStr.SetSize(D_UFTR_END);
	m_aAllProjStr[D_UFTR_START       ] = _T("");
	m_aAllProjStr[D_UFTR_PROJECT_NAME] = _LS(IDS_TM_DYNA_REPORT_Project_Name);
	m_aAllProjStr[D_UFTR_REVISION    ] = _LS(IDS_TM_DYNA_REPORT_Revision    );
	m_aAllProjStr[D_UFTR_USER_NAME   ] = _LS(IDS_TM_DYNA_REPORT_User_Name   );
	m_aAllProjStr[D_UFTR_EMAIL       ] = _LS(IDS_TM_DYNA_REPORT_E_mail      );
	m_aAllProjStr[D_UFTR_ADDRESS     ] = _LS(IDS_TM_DYNA_REPORT_Address     );
	m_aAllProjStr[D_UFTR_TELEPHONE   ] = _LS(IDS_TM_DYNA_REPORT_Telephone   );
	m_aAllProjStr[D_UFTR_FAX         ] = _LS(IDS_TM_DYNA_REPORT_Fax         );
	m_aAllProjStr[D_UFTR_CLIENT      ] = _LS(IDS_TM_DYNA_REPORT_Client      );
	m_aAllProjStr[D_UFTR_TITLE       ] = _LS(IDS_TM_DYNA_REPORT_Title       );
	m_aAllProjStr[D_UFTR_FILE_NAME   ] = _LS(IDS_TM_DYNA_REPORT_File_Name   );
	m_aAllProjStr[D_UFTR_CREATED     ] = _LS(IDS_TM_DYNA_REPORT_Created     );
	m_aAllProjStr[D_UFTR_DIRECTORY   ] = _LS(IDS_TM_DYNA_REPORT_Directory   );
	m_aAllProjStr[D_UFTR_MODIFIED    ] = _LS(IDS_TM_DYNA_REPORT_Modified    );
	m_aAllProjStr[D_UFTR_FILE_SIZE   ] = _LS(IDS_TM_DYNA_REPORT_File_Size   );
}

// nNum : GEN_IFTAG의 개수 (Default=1개)
void CReportCommonFunc::ExportData2Word(GEN_IFTAG* pIftag, int nNum)
{
	if(pIftag==NULL) return;

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nNum, &pIftag);
	}
	hMod = NULL;
}

// nNum : GEN_IFTAG의 개수 (Default=1개)
void CReportCommonFunc::ChangeStructStr2Char(GEN_IFTAG_STR* pSrc, GEN_IFTAG* pTgt, int nNum)
{
	if(pSrc==NULL || pTgt==NULL) return;

	for(int i=0; i<nNum; i++)
	{
		pTgt[i].sProductInfo = (LPTSTR)(LPCTSTR)pSrc[i].sProductInfo;
		pTgt[i].sCaption     = (LPTSTR)(LPCTSTR)pSrc[i].sCaption    ;
		pTgt[i].sType        = (LPTSTR)(LPCTSTR)pSrc[i].sType       ;
		pTgt[i].sFullPath    = (LPTSTR)(LPCTSTR)pSrc[i].sFullPath   ;
		pTgt[i].sExtInfo     = (LPTSTR)(LPCTSTR)pSrc[i].sExtInfo    ;
		pTgt[i].sContents    = (LPTSTR)(LPCTSTR)pSrc[i].sContents   ;
		pTgt[i].sExt         = (LPTSTR)(LPCTSTR)pSrc[i].sExt        ;
		pTgt[i].iAppendFlag  = pSrc[i].iAppendFlag;
	}
}

void CReportCommonFunc::ExportData2Word_ByStyle( GEN_IFTAG* pIftag, int nStyleNum/*=0*/ )
{
	if(pIftag==NULL) return;
	
	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)
	{
		EditorSetupStyle pExport=NULL;
		pExport = (EditorSetupStyle) GetProcAddress(hMod, "EditorSetupStyle");
		pExport(nStyleNum);
	}
	hMod = NULL;
	ExportData2Word(pIftag,1);
	
}

void CReportCommonFunc::ExportInsertPageBreak()
{
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)
	{
		EditorPageBreak pExport=NULL;
		pExport = (EditorPageBreak) GetProcAddress(hMod, "EditorPageBreak");
		pExport();
	}
	hMod = NULL;
}

void CReportCommonFunc::ExportInsertCatalog(int nLevel)
{
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)
	{
		EditorInsertContents pExport=NULL;
		pExport = (EditorInsertContents) GetProcAddress(hMod, "EditorInsertContents");
		pExport(nLevel);
	}
	hMod = NULL;
}

void CReportCommonFunc::ExportUpdateCatalog()
{
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)
	{
		EditorUpdateContents pExport=NULL;
		pExport = (EditorUpdateContents) GetProcAddress(hMod, "EditorUpdateContents");
		pExport();
	}
	hMod = NULL;
}

void CReportCommonFunc::ExportInsertPageNumber( int nHeaderFooter, int nAlignment, bool bFirstPage )
{
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)
	{
		EditorInsertPageNumber pExport=NULL;
		pExport = (EditorInsertPageNumber) GetProcAddress(hMod, "EditorInsertPageNumber");
		pExport(nHeaderFooter,nAlignment,bFirstPage);
	}
	hMod = NULL;
}
