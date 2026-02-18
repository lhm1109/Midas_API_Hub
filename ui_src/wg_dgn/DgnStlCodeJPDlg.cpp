// DgnStlCodeJPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlCodeJPDlg.h"

#include "DgnStlEnvelopeDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#include "..\wg_main\wg_main.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCodeJPDlg dialog


CDgnStlCodeJPDlg::CDgnStlCodeJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlCodeJPDlg::IDD, pParent)
{
	m_EnvelopeData.Initialize();

	m_nShearStressMethod=0;
	m_nDesignForceMethod=0;

	//{{AFX_DATA_INIT(CDgnStlCodeJPDlg)	
	//}}AFX_DATA_INIT

	m_aPrintCtrlLst.RemoveAll();
	m_aPrintCtrlLst.Add(IDC_DGN_SECT_CHECK1);
	m_aPrintCtrlLst.Add(IDC_DGN_SECT_CHECK2);
	m_aPrintCtrlLst.Add(IDC_DGN_SECT_CHECK3);
	m_aPrintCtrlLst.Add(IDC_DGN_SELECT_ALL_BTN);
	m_aPrintCtrlLst.Add(IDC_DGN_UNSELECT_ALL_BTN);
}

void CDgnStlCodeJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlCodeJPDlg)
	DDX_Control(pDX, IDC_DGN_CTRLDATA_DESIGNCODE, m_DesignCode);		
	DDX_Control(pDX, IDC_DNG_SECT_FORCE_RADIO1, m_radioSectForce1);
	DDX_Control(pDX, IDC_DNG_SECT_FORCE_RADIO2, m_radioSectForce2);
	DDX_Control(pDX, IDC_DNG_SECT_FORCE_RADIO3, m_radioSectForce3);
	DDX_Control(pDX, IDC_DGN_SECT_CHECK1, m_chkSectCheck1);
	DDX_Control(pDX, IDC_DGN_SECT_CHECK2, m_chkSectCheck2);
	DDX_Control(pDX, IDC_DGN_SECT_CHECK3, m_chkSectCheck3);
	DDX_Control(pDX, IDC_DGN_ENVELOPE_BTN, m_btnEnvelope);
	DDX_Control(pDX, IDC_DGN_SELECT_ALL_BTN, m_btnSelectAll);
	DDX_Control(pDX, IDC_DGN_UNSELECT_ALL_BTN, m_btnUnSelectAll);

	DDX_Radio(pDX, IDC_DGN_CALC_OPTTON_SHEAR_STRESS01_RDO, m_nShearStressMethod);
	DDX_Radio(pDX, IDC_DGN_CALC_OPTTON_DESIGN_FORCE01_RDO, m_nDesignForceMethod);

	//}}AFX_DATA_MAP	
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlCodeJPDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnStlCodeJPDlg)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_DESIGNCODE, OnChangeDgnCodeCmb)
	ON_BN_CLICKED(IDC_DGN_ENVELOPE_BTN, OnDgnEnvelopeBtn)
	ON_BN_CLICKED(IDC_DGN_SELECT_ALL_BTN, OnDgnSelectAllBtn)
	ON_BN_CLICKED(IDC_DGN_UNSELECT_ALL_BTN, OnDgnUnSelectAllBtn)
	ON_BN_CLICKED(IDC_DNG_SECT_FORCE_RADIO1, OnDgnSectionForceRadio)
	ON_BN_CLICKED(IDC_DNG_SECT_FORCE_RADIO2, OnDgnSectionForceRadio)
	ON_BN_CLICKED(IDC_DNG_SECT_FORCE_RADIO3, OnDgnSectionForceRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCodeJPDlg message handlers

BOOL CDgnStlCodeJPDlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitCtrl();

	if(m_pDoc->m_pAttrCtrl->ExistDstl()){Initial_ExistData();}
	else{Initial_Data();}

	OnDgnSectionForceRadio();

	EnableDisable_Ctrl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlCodeJPDlg::Initial_Data()
{  
	T_DSTL_D rData;
	rData.Initialize();  

	if(!m_pDoc->m_pAttrCtrl->GetDstl(rData))
	{
	  T_PREFERENCE rPref;
	  rPref.Initialize();
	  m_pDoc->m_pInitCtrl->GetPreference(rPref);
	  int Index = m_DesignCode.FindStringExact(-1, rPref.DgnCode.SteelCode);
	  if(Index!=-1)	m_DesignCode.SetCurSel(Index);	
	}

	if(rData.iSelDgnLcom == 0)
	{
		m_radioSectForce1.SetCheck(TRUE);
		m_radioSectForce2.SetCheck(FALSE);
		m_radioSectForce3.SetCheck(FALSE);
	}
	else if(rData.iSelDgnLcom == 1)
	{
		m_radioSectForce1.SetCheck(FALSE);
		m_radioSectForce2.SetCheck(TRUE);
		m_radioSectForce3.SetCheck(FALSE);
	}
	else if(rData.iSelDgnLcom == 2)
	{
		m_radioSectForce1.SetCheck(FALSE);
		m_radioSectForce2.SetCheck(FALSE);
		m_radioSectForce3.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	m_chkSectCheck1.SetCheck(rData.bPrintChk[0]);
	m_chkSectCheck2.SetCheck(rData.bPrintChk[1]);
	m_chkSectCheck3.SetCheck(rData.bPrintChk[2]);

	m_EnvelopeData.bP_max	= rData.bEnvelopeForce[0];
	m_EnvelopeData.bP_min	= rData.bEnvelopeForce[1];
	m_EnvelopeData.bVy_max	= rData.bEnvelopeForce[2];
	m_EnvelopeData.bVy_min	= rData.bEnvelopeForce[3];
	m_EnvelopeData.bVz_max	= rData.bEnvelopeForce[4];
	m_EnvelopeData.bVz_min	= rData.bEnvelopeForce[5];
	m_EnvelopeData.bT_max	= rData.bEnvelopeForce[6];
	m_EnvelopeData.bT_min	= rData.bEnvelopeForce[7];
	m_EnvelopeData.bMy_max	= rData.bEnvelopeForce[8];
	m_EnvelopeData.bMy_min	= rData.bEnvelopeForce[9];
	m_EnvelopeData.bMz_max	= rData.bEnvelopeForce[10];
	m_EnvelopeData.bMz_min	= rData.bEnvelopeForce[11];

	m_nShearStressMethod = rData.nShearStressMethod;
	m_nDesignForceMethod = rData.nDesignForceMethod;
}

void CDgnStlCodeJPDlg::InitCtrl()
{
	// Design Code Combo Box
	m_DesignCode.ResetContent();
	m_DesignCode.SetItemData(m_DesignCode.AddString(STLCODE_JROAD_H24), 0); 
	m_DesignCode.SetItemData(m_DesignCode.AddString(STLCODE_JROAD_H14), 1); 
	m_DesignCode.SetItemData(m_DesignCode.AddString(STLCODE_JROAD_H29), 2);
	m_DesignCode.SetItemData(m_DesignCode.AddString(STLCODE_JROAD_R07), 3);

	int nCurSel = 0;

	T_DSTL_D DstlD;
	if(m_pDoc->m_pAttrCtrl->GetDstl(DstlD))
	{
		if     (DstlD.DesignCode.CompareNoCase(STLCODE_JROAD_H24)==0){nCurSel = 0;}
		else if(DstlD.DesignCode.CompareNoCase(STLCODE_JROAD_H14)==0){nCurSel = 1;}
		else if(DstlD.DesignCode.CompareNoCase(STLCODE_JROAD_H29)==0){nCurSel = 2;}
		else if(DstlD.DesignCode.CompareNoCase(STLCODE_JROAD_R07)==0){nCurSel = 3;}
		else { ASSERT(0); nCurSel = 0; }
	}
	else
	{
		nCurSel = 0;
	}

	m_DesignCode.SetCurSel(nCurSel);  
}

void CDgnStlCodeJPDlg::EnableDisable_Ctrl()
{
// 	int nCurSelIndex = m_DesignCode.GetCurSel();
// 	int nStlDgnCode = m_DesignCode.GetItemData(nCurSelIndex);
// 	CDlgUtil::CtrlEnableDisable(this, m_aPrintCtrlLst, nStlDgnCode == 2);
}

void CDgnStlCodeJPDlg::Initial_ExistData()
{
	T_DSTL_D rData;  rData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetDstl(rData))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		int Index = m_DesignCode.FindStringExact(-1, rPref.DgnCode.SteelCode);
		if(Index!=-1)	m_DesignCode.SetCurSel(Index);	
	}

	if(rData.iSelDgnLcom == 0)
	{
		m_radioSectForce1.SetCheck(TRUE);
		m_radioSectForce2.SetCheck(FALSE);
		m_radioSectForce3.SetCheck(FALSE);
	}
	else if(rData.iSelDgnLcom == 1)
	{
		m_radioSectForce1.SetCheck(FALSE);
		m_radioSectForce2.SetCheck(TRUE);
		m_radioSectForce3.SetCheck(FALSE);
	}
	else if(rData.iSelDgnLcom == 2)
	{
		m_radioSectForce1.SetCheck(FALSE);
		m_radioSectForce2.SetCheck(FALSE);
		m_radioSectForce3.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	m_chkSectCheck1.SetCheck(rData.bPrintChk[0]);
	m_chkSectCheck2.SetCheck(rData.bPrintChk[1]);
	m_chkSectCheck3.SetCheck(rData.bPrintChk[2]);

	m_EnvelopeData.bP_max	= rData.bEnvelopeForce[0];
	m_EnvelopeData.bP_min	= rData.bEnvelopeForce[1];
	m_EnvelopeData.bVy_max	= rData.bEnvelopeForce[2];
	m_EnvelopeData.bVy_min	= rData.bEnvelopeForce[3];
	m_EnvelopeData.bVz_max	= rData.bEnvelopeForce[4];
	m_EnvelopeData.bVz_min	= rData.bEnvelopeForce[5];
	m_EnvelopeData.bT_max	= rData.bEnvelopeForce[6];
	m_EnvelopeData.bT_min	= rData.bEnvelopeForce[7];
	m_EnvelopeData.bMy_max	= rData.bEnvelopeForce[8];
	m_EnvelopeData.bMy_min	= rData.bEnvelopeForce[9];
	m_EnvelopeData.bMz_max	= rData.bEnvelopeForce[10];
	m_EnvelopeData.bMz_min	= rData.bEnvelopeForce[11];

	m_nShearStressMethod = rData.nShearStressMethod;
	m_nDesignForceMethod = rData.nDesignForceMethod;
}

void CDgnStlCodeJPDlg::OnOK() 
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	UpdateData(TRUE);

	T_DSTL_D rData;
	rData.Initialize();
	// Exist Steel Control Data at DB.
	if(m_pDoc->m_pAttrCtrl->ExistDstl())	m_pDoc->m_pAttrCtrl->GetDstl(rData);

	// Get Design Code.
	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index, rData.DesignCode);
	
	if(m_radioSectForce1.GetCheck())
		rData.iSelDgnLcom = 0;
	else if(m_radioSectForce2.GetCheck())
		rData.iSelDgnLcom = 1;
	else if(m_radioSectForce3.GetCheck())
		rData.iSelDgnLcom = 2;
	else
		rData.iSelDgnLcom = 0;

	rData.bPrintChk[0] = m_chkSectCheck1.GetCheck();
	rData.bPrintChk[1] = m_chkSectCheck2.GetCheck();
	rData.bPrintChk[2] = m_chkSectCheck3.GetCheck();

	rData.bEnvelopeForce[0] = m_EnvelopeData.bP_max;
	rData.bEnvelopeForce[1] = m_EnvelopeData.bP_min;
	rData.bEnvelopeForce[2] = m_EnvelopeData.bVy_max;
	rData.bEnvelopeForce[3] = m_EnvelopeData.bVy_min;
	rData.bEnvelopeForce[4] = m_EnvelopeData.bVz_max;
	rData.bEnvelopeForce[5] = m_EnvelopeData.bVz_min;
	rData.bEnvelopeForce[6] = m_EnvelopeData.bT_max;
	rData.bEnvelopeForce[7] = m_EnvelopeData.bT_min;
	rData.bEnvelopeForce[8] = m_EnvelopeData.bMy_max;
	rData.bEnvelopeForce[9] = m_EnvelopeData.bMy_min;
	rData.bEnvelopeForce[10] = m_EnvelopeData.bMz_max;
	rData.bEnvelopeForce[11] = m_EnvelopeData.bMz_min;
	
	rData.nShearStressMethod = m_nShearStressMethod;
	rData.nDesignForceMethod = m_nDesignForceMethod;

	// Save Steel Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDstl(rData);
	if(bCheck)	
	{
		CDialogMove::OnOK();
	}
}

void CDgnStlCodeJPDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnStlCodeJPDlg::OnChangeDgnCodeCmb() 
{
	EnableDisable_Ctrl();
}

void CDgnStlCodeJPDlg::OnDgnEnvelopeBtn() 
{
	CDgnStlEnvelopeDlg dlg;

	dlg.m_EnvelopeData = m_EnvelopeData;
	
	if(dlg.DoModal() == IDOK)
		m_EnvelopeData = dlg.m_EnvelopeData;	
}

void CDgnStlCodeJPDlg::OnDgnSelectAllBtn() 
{
	m_chkSectCheck1.SetCheck(TRUE);
	m_chkSectCheck2.SetCheck(TRUE);
	m_chkSectCheck3.SetCheck(TRUE);
}

void CDgnStlCodeJPDlg::OnDgnUnSelectAllBtn() 
{
	m_chkSectCheck1.SetCheck(FALSE);
	m_chkSectCheck2.SetCheck(FALSE);
	m_chkSectCheck3.SetCheck(FALSE);
}

void CDgnStlCodeJPDlg::OnDgnSectionForceRadio() 
{
	if(m_radioSectForce1.GetCheck())
		m_btnEnvelope.EnableWindow(TRUE);
	else if(m_radioSectForce2.GetCheck())
		m_btnEnvelope.EnableWindow(FALSE);
	else if(m_radioSectForce3.GetCheck())
		m_btnEnvelope.EnableWindow(FALSE);
	else
		ASSERT(0);
}