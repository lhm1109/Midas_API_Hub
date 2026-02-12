#include "stdafx.h"

#include "ReportImageFileDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CReportImageFileDlg dialog

CReportImageFileDlg::CReportImageFileDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CReportImageFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportImageFileDlg)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT

	m_strImageFilePath = _T("");
}

CReportImageFileDlg::~CReportImageFileDlg()
{
}

void CReportImageFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportImageFileDlg)
	DDX_Text(pDX, IDC_TM_REPORT_IMAGEFILE_MSG_EDT, m_strFilePath);
	DDX_Control(pDX, IDC_TM_REPORT_IMAGEFILE_LST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportImageFileDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportImageFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportImageFileDlg message handlers

BOOL CReportImageFileDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

#if defined(_ORG)
	this->SetWindowText(CString(_T("Smart Report Image File")));
#else
	this->SetWindowText(_LS(IDS_DB_DT_UIMG));
#endif
	
	Data2Dlg();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless y
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CReportImageFileDlg::OnOK() 
{
	UpdateData(TRUE);
	Dlg2Data();

	CDialogMove::OnOK();
}

BOOL CReportImageFileDlg::Data2Dlg()
{
	m_strFilePath = _T("");
	m_strImageFilePath = _T("");

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();

	CFileCtrl FileCtrl(strModelPath);
	CString strTmp=_T("");
	strTmp = FileCtrl.GetFilePath();
	if(strTmp!=_T(""))
	{
		m_strImageFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files), strTmp);
	}

	if(m_strImageFilePath==_T("") || !FileCtrl.FileExists(m_strImageFilePath))
		m_strFilePath = _LS(IDS_TM_DYNA_REPORT_IMAGE_FILES_FOLDER_ERROR);
	else
		m_strFilePath = _LS(IDS_TM_DYNA_REPORT_Location) + m_strImageFilePath;

	MakeFileItemList();

	return TRUE;
}

void CReportImageFileDlg::Dlg2Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	UpdateData(TRUE);  

	int nCount = m_List.GetSelCount();
	int *aIndex = new int[nCount];
	m_List.GetSelItems(nCount, aIndex);
	int nIndex=0;

	CString strName=_T("");
	CArray<T_UIMG_D, T_UIMG_D&> aUimgD;
	aUimgD.SetSize(nCount);
	for(int i=0; i<nCount; i++)
	{
		strName=_T("");
		nIndex = aIndex[i];
		m_List.GetText(nIndex, strName);

		aUimgD[i].Initialize();
		aUimgD[i].strFilePath = strName;
		//aUimgD[i].strFilePath = m_strImageFilePath + "\\" + strName;
		
		if(IsSymbol(strName))
			strName = DeleteSymbol(strName);
		aUimgD[i].strName = strName;
	}
	if(nCount>0 && !pDoc->m_pDataCtrl->AddUimg(aUimgD))
	{
		ASSERT(0);
	}
	delete []aIndex;
}

void CReportImageFileDlg::MakeFileItemList()
{
	m_List.ResetContent();
	if(m_strImageFilePath==_T("")) return;
	CFileCtrl FileCtrl(m_strImageFilePath);
	if(!FileCtrl.FileExists(m_strImageFilePath)) return;
	
	CString strFolder = m_strImageFilePath;
	CStringArray aFileType;
	aFileType.Add(strFolder + _T("\\*.emf"));
	aFileType.Add(strFolder + _T("\\*.wmf"));
	aFileType.Add(strFolder + _T("\\*.jpg"));
	aFileType.Add(strFolder + _T("\\*.jpeg"));
	aFileType.Add(strFolder + _T("\\*.jfif"));
	aFileType.Add(strFolder + _T("\\*.jpe"));
	aFileType.Add(strFolder + _T("\\*.png"));
	aFileType.Add(strFolder + _T("\\*.bmp"));
	aFileType.Add(strFolder + _T("\\*.dib"));
	aFileType.Add(strFolder + _T("\\*.rle"));
	aFileType.Add(strFolder + _T("\\*.bmz"));
	aFileType.Add(strFolder + _T("\\*.gif"));
	aFileType.Add(strFolder + _T("\\*.gfa"));
	aFileType.Add(strFolder + _T("\\*.emz"));
	aFileType.Add(strFolder + _T("\\*.wmz"));
	aFileType.Add(strFolder + _T("\\*.psz"));
	aFileType.Add(strFolder + _T("\\*.tif"));
	aFileType.Add(strFolder + _T("\\*.tiff"));
	aFileType.Add(strFolder + _T("\\*.cgm"));
	aFileType.Add(strFolder + _T("\\*.eps"));
	aFileType.Add(strFolder + _T("\\*.pct"));
	aFileType.Add(strFolder + _T("\\*.pict"));
	aFileType.Add(strFolder + _T("\\*.wpg"));

	int nType=0;
	nType = aFileType.GetSize();

	CString strFileType=_T("");
	BOOL bOK = FALSE;
	CFileFind ff;
	int nIndex=0;

	for(int i=0; i<nType; i++)
	{
		strFileType=aFileType[i];

		bOK = ff.FindFile(strFileType);
		while(bOK)
		{
			bOK = ff.FindNextFile();
			if(ff.IsDirectory()) continue;
			
			CString strName = ff.GetFileName();
			nIndex = m_List.AddString(strName);
			
			// 우선 하위 디렉토리 지원하지 말자. 귀차나;; 그냥 Image File 폴더에다 다 넣고 써라...
			//     if(ff.IsDirectory()) 
			//     {
			//       CString csSubFolder = ff.GetFileName();
			//       if (csSubFolder == "." || csSubFolder == "..") continue;
			//       csSubFolder = ff.GetFilePath();
			//     }
		}
	}
	ff.Close();
}

BOOL CReportImageFileDlg::IsSymbol(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	int r=-1;
	
	r = strTemp.Find(_T("~"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("!"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("@"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("#"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("$"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("%"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("^"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("&"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("*"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("("));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(")"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("-"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("_"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("="));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("+"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("\\"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("|"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("["));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("{"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("]"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("}"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(";"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(":"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("\'"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("\""));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(","));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("<"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("."));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(">"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("/"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("?"));
	if(r>=0) return TRUE;
	
	return FALSE; 
}

CString CReportImageFileDlg::DeleteSymbol(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	strTemp.Replace(_T("_"), _T("")); 
	strTemp.Replace(_T("~"),_T(""));
	strTemp.Replace(_T("!"),_T(""));
	strTemp.Replace(_T("@"),_T(""));
	strTemp.Replace(_T("#"),_T(""));
	strTemp.Replace(_T("$"),_T(""));
	strTemp.Replace(_T("%"),_T(""));
	strTemp.Replace(_T("^"),_T(""));
	strTemp.Replace(_T("&"),_T(""));
	strTemp.Replace(_T("*"),_T(""));
	strTemp.Replace(_T("("),_T(""));
	strTemp.Replace(_T(")"),_T(""));
	strTemp.Replace(_T("-"),_T(""));
	strTemp.Replace(_T("+"),_T(""));
	strTemp.Replace(_T("="),_T(""));
	strTemp.Replace(_T("\\"),_T(""));
	strTemp.Replace(_T("|"),_T(""));
	strTemp.Replace(_T("["),_T(""));
	strTemp.Replace(_T("{"),_T(""));
	strTemp.Replace(_T("]"),_T(""));
	strTemp.Replace(_T("}"),_T(""));
	strTemp.Replace(_T(":"),_T(""));
	strTemp.Replace(_T(";"),_T(""));
	strTemp.Replace(_T("\'"),_T(""));
	strTemp.Replace(_T("\""),_T(""));
	strTemp.Replace(_T("<"),_T(""));
	strTemp.Replace(_T(","),_T(""));
	strTemp.Replace(_T("."),_T(""));
	strTemp.Replace(_T(">"),_T(""));
	strTemp.Replace(_T("/"),_T(""));
	strTemp.Replace(_T("?"),_T(""));
	
	return strTemp; 
}
