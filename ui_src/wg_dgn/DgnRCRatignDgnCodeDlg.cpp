// DgnRCRatignDgnCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatignDgnCodeDlg.h"

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
// CDgnRCRatignDgnCodeDlg dialog

CDgnRCRatingDgnCodeDlg::CDgnRCRatingDgnCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRCRatingDgnCodeDlg::IDD, pParent)
{

}

CDgnRCRatingDgnCodeDlg::~CDgnRCRatingDgnCodeDlg()
{
}

void CDgnRCRatingDgnCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGN_RC_RATING_CODE, m_DesignCode);
	
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnRCRatingDgnCodeDlg, CInternationalDlg)
#undef CDialog

END_MESSAGE_MAP()


// CDgnRCRatignDgnCodeDlg 메시지 처리기입니다.

BOOL CDgnRCRatingDgnCodeDlg::OnInitDialog()
{
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitDgnCodeCombo();

	if(m_pDoc->m_pAttrCtrl2->ExistRcdc())	
	{
		m_pDoc->m_pAttrCtrl2->GetRcdc(m_Data);
		Data2Dlg();
	}
	else
		Initial_Data();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingDgnCodeDlg::InitDgnCodeCombo()
{
	m_DesignCode.ResetContent();
	CStringArray arCodeName;
	
	CDBLib::GetRcRatCodeNameList(arCodeName);
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

void CDgnRCRatingDgnCodeDlg::Data2Dlg()
{
	CString srtRCRatingCode = _T("");
	srtRCRatingCode = CDBLib::GetRcRatCodeName(m_Data.nRatingCode);

#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(srtRCRatingCode);
#endif
	int Index = m_DesignCode.FindStringExact(-1, srtRCRatingCode);
	if(Index!=-1)	
		m_DesignCode.SetCurSel(Index);
}

void CDgnRCRatingDgnCodeDlg::Dlg2Data()
{
	int Index = m_DesignCode.GetCurSel();
	CString srtRCRatingCode = _T("");
	m_DesignCode.GetLBText(Index, srtRCRatingCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(srtRCRatingCode);
#endif
	m_Data.nRatingCode = CDBLib::GetRcRatCodeNo(srtRCRatingCode);
}

void CDgnRCRatingDgnCodeDlg::Initial_Data()
{
	m_Data.Initialize();

	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	m_Data.nRatingCode = CDBLib::GetRcRatCodeNo(rPref.DgnCode.RCCode);

	Data2Dlg();
}

void CDgnRCRatingDgnCodeDlg::OnOK() 
{
	Dlg2Data();
	m_pDoc->m_pDataCtrl->AddRcdc(m_Data); 

	CDialogMove::OnOK();
}

void CDgnRCRatingDgnCodeDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}