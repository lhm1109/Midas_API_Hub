// DgnRatingCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRatingCodeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_MsgDll.h"


#include "..\wg_main\wg_main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingCodeDlg dialog


CDgnRatingCodeDlg::CDgnRatingCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRatingCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRatingCodeDlg)
	
	//}}AFX_DATA_INIT
}

void CDgnRatingCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRatingCodeDlg)
	DDX_Control(pDX, IDC_DGN_CTRLDATA_DESIGNCODE, m_DesignCode);
	//}}AFX_DATA_MAP  
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnRatingCodeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnRatingCodeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingCodeDlg message handlers

BOOL CDgnRatingCodeDlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	InitDgnCodeCombo();
	
	if(m_pDoc->m_pAttrCtrl2->ExistRacd())	
	{
		m_pDoc->m_pAttrCtrl2->GetRacd(m_Data);
		Data2Dlg();
	}
	else
		Initial_Data();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRatingCodeDlg::InitDgnCodeCombo()
{
	m_DesignCode.ResetContent();
	CStringArray arCodeName;
	
	CDBLib::GetPscRatCodeNameList(arCodeName);  
	int iCodeSize = arCodeName.GetSize();

	/*XXXX
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);*/

	for(int i=0; i<iCodeSize; i++)
	{
#if defined(_RUS)
		CInitCtrl::ConvertEngRusDesignCode(arCodeName[i]);
#endif
		m_DesignCode.AddString(arCodeName[i]);
	}  
}

void CDgnRatingCodeDlg::Data2Dlg()
{
	CString srtRatingCode = _T("");
	srtRatingCode = CDBLib::GetPscRatCodeName(m_Data.nRatingCode);
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(srtRatingCode);
#endif
	int Index = m_DesignCode.FindStringExact(-1, srtRatingCode);
	if(Index!=-1)	
		m_DesignCode.SetCurSel(Index);
}

void CDgnRatingCodeDlg::Dlg2Data()
{
	int Index = m_DesignCode.GetCurSel();
	CString srtRatingCode = _T("");
	m_DesignCode.GetLBText(Index, srtRatingCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(srtRatingCode);
#endif
	m_Data.nRatingCode = CDBLib::GetPscRatCodeNo(srtRatingCode);
}

void CDgnRatingCodeDlg::Initial_Data()
{
	m_Data.Initialize();
	
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	
	m_Data.nRatingCode = CDBLib::GetPscRatCodeNo(rPref.DgnCode.RCCode);

	Data2Dlg();
}



void CDgnRatingCodeDlg::OnOK() 
{
	Dlg2Data();
	m_pDoc->m_pDataCtrl->AddRacd(m_Data);

	CDialogMove::OnOK();
}

void CDgnRatingCodeDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}


