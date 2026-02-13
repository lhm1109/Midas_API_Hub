// DgnRatingStlCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRatingStlCodeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_LocaleLib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingStlCodeDlg dialog


CDgnRatingStlCodeDlg::CDgnRatingStlCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRatingStlCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRatingStlCodeDlg)
	//}}AFX_DATA_INIT
}

void CDgnRatingStlCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRatingStlCodeDlg)
	DDX_Control(pDX, IDC_DGN_RATING_STLCODE_CMB, m_DesignCode);
	//}}AFX_DATA_MAP  
}

BEGIN_MESSAGE_MAP(CDgnRatingStlCodeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRatingStlCodeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingStlCodeDlg message handlers

BOOL CDgnRatingStlCodeDlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitDgnCodeCombo();

	if(m_pDoc->m_pAttrCtrl2->ExistRcds())	
	{
		m_pDoc->m_pAttrCtrl2->GetRcds(m_Data);
		Data2Dlg();
	}
	else
		Initial_Data();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRatingStlCodeDlg::InitDgnCodeCombo()
{
	m_DesignCode.ResetContent();
	CStringArray arCodeName;

	CDBLib::GetStlRatCodeNameList(arCodeName);  
	int iCodeSize = arCodeName.GetSize();
	for (int i = 0; i < iCodeSize; i++)
	{
#if defined(_RUS)
		CInitCtrl::ConvertEngRusDesignCode(arCodeName[i]);
#endif
		m_DesignCode.AddString(arCodeName[i]);
	}
}

void CDgnRatingStlCodeDlg::Data2Dlg()
{
	CString srtRatingCode = _T("");
	srtRatingCode = CDBLib::GetStlRatCodeName(m_Data.nRatingCode);
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(srtRatingCode);
#endif
	int Index = m_DesignCode.FindStringExact(-1, srtRatingCode);
	if(Index!=-1)	
		m_DesignCode.SetCurSel(Index);
}

void CDgnRatingStlCodeDlg::Dlg2Data()
{
	int Index = m_DesignCode.GetCurSel();
	CString srtRatingCode = _T("");
	m_DesignCode.GetLBText(Index, srtRatingCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(srtRatingCode);
#endif
	m_Data.nRatingCode = CDBLib::GetStlRatCodeNo(srtRatingCode);
}

void CDgnRatingStlCodeDlg::Initial_Data()
{
	m_Data.Initialize();

	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	m_Data.nRatingCode = CDBLib::GetStlRatCodeNo(rPref.DgnCode.RatingSTL);

	Data2Dlg();
}

void CDgnRatingStlCodeDlg::OnOK() 
{
	Dlg2Data();
	m_pDoc->m_pDataCtrl->AddRcds(m_Data);

	CDialogMove::OnOK();
}

void CDgnRatingStlCodeDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}



