#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnSeisBrgInfoFile.h"

#include "..\wg_db\FileRW.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_base\TextFileOut.h"


UINT TDataRW_SBRI::GetSize()
{
	// layout
	int nSize = sizeof(SBRI_RW);
	CStringA strTitle;
	strTitle = CTextFileOut::CString2Mbcs(m_data.strBrgName     );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strLocation    );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strManager     );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strLineNum     );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strOrgLoc      );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strMvhlType    );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strMvhlName    );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strUpper       );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strAbutmentType);  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strAbutmentBase);  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strPierType    );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strPierBase    );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strSupportMatl );  nSize += strTitle.GetLength();
	strTitle = CTextFileOut::CString2Mbcs(m_data.strPierSect    );  nSize += strTitle.GetLength();

	nSize += (int)m_data.aSpan.GetSize() * sizeof(double);

	return nSize;
}

BOOL TDataRW_SBRI::Stream2Data(const void* pStream, int nMaxSize)
{
	SBRI_RW RW;
	int nStart = 0;
	return RW.S2D(m_data, (unsigned char*)pStream, nStart, nMaxSize);
}

BOOL TDataRW_SBRI::Data2Stream(void* pStream, int nMaxSize)
{
	SBRI_RW RW;
	int nStart = 0;
	return RW.D2S(m_data, (unsigned char*)pStream, nStart, nMaxSize);
}

// Save할 때 호출
void BrgInfoRW::GetNextData(POSITION& rPos, DB_TDataRW* pDataRW)
{
	TDataRW_SBRI* pData = (TDataRW_SBRI*)pDataRW;
	pData->m_data = m_data;
	rPos = 0;
}

// Load할 때 호출
BOOL BrgInfoRW::AddToDB(UINT nVer, DB_TDataRW* pDataRW)
{
	// 이전 버전이면 최신 버전으로 데이타 변환  
	if (nVer == D_PGM_VER(8,8,3,0))
	{
		TDataRW_SBRI* pData = (TDataRW_SBRI*)pDataRW;
		m_data = pData->m_data;
	}
	else { ASSERT(0); return FALSE; }
	m_bAdded = TRUE;
	return TRUE;
}

// Load할 때 호출 
DB_TDataRW* BrgInfoRW::CreateData(UINT nVer)
{
	if (nVer == D_PGM_VER(8,8,3,0))
	{
		return new TDataRW_SBRI;
	}
	else ASSERT(0);
	return 0;
}

CDgnSeisBrgInfoFile::CDgnSeisBrgInfoFile()
{

}

CDgnSeisBrgInfoFile::~CDgnSeisBrgInfoFile()
{
	
}

BOOL CDgnSeisBrgInfoFile::SaveToFile(T_SBRI_D& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CFileDialog dlg(FALSE, _T(".wzd"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES, _T("WZD Files(*.wzd)|*.wzd||"));
	CString csFileName;
	if(dlg.DoModal() == IDOK) csFileName = dlg.GetPathName();
	else return FALSE;

	CFileRW FileRW;
	BrgInfoRW DataRW;  
	DataRW.m_data = rData;  
	pDoc->m_pUnitCtrl->ConvertUnitSbriIn(DataRW.m_data);
	BOOL bResult = FileRW.Save(csFileName, &DataRW);
	if(bResult) GSaveHistoryFormatNF(_LS(IDS_DGN_SEIS_FILE_WRITE));
	return bResult;
}

BOOL CDgnSeisBrgInfoFile::ReadFromFile(T_SBRI_D& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CFileDialog dlg(TRUE, _T(".wzd"), NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, _T("WZD Files(*.wzd)|*.wzd||"));
	CString csFileName;
	if(dlg.DoModal()==IDOK) csFileName = dlg.GetPathName();
	else return FALSE;

	CFileRW FileRW;
	BrgInfoRW DataRW;
	if (!FileRW.Load(csFileName, &DataRW)) return FALSE;  
	if (DataRW.m_bAdded == FALSE)
	{
		//     AfxMessageBox(_LS(IDS_WG_SW_GRIL_Data_Not_Exist));
		return FALSE; // 데이타 없는 경우
	}
	pDoc->m_pUnitCtrl->ConvertUnitSbriOut(DataRW.m_data);
	rData = DataRW.m_data;

	GSaveHistoryFormatNF(_LS(IDS_DGN_SEIS_FILE_READ));
	return TRUE;
}