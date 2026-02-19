// DgnSrcCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSrcCodeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_main\wg_main.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcCodeDlg dialog


CDgnSrcCodeDlg::CDgnSrcCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSrcCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSrcCodeDlg)
	m_bSpecialEQ = FALSE;
	m_iSafeLevel = -1;
	m_iEqGrade = -1;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	//}}AFX_DATA_INIT
	m_aCodeEnable.RemoveAll();
	m_nOldCodeIndex = 0;

    m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnSrcCodeDlg::~CDgnSrcCodeDlg()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

#define IDSRC2(n)\
	arID2.Add(IDC_DGN_SAFETY_LEVEL##n);
#define IDSRC3(n)\
	arID3.Add(IDC_DGN_EQ_GRADE##n);
#define IDSRC4(n)\
	arID4.Add(IDC_DGN_EQ_FACTOR##n);

void CDgnSrcCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSrcCodeDlg)
	DDX_Control(pDX, IDC_DGN_SRC_DESIGNCODE, m_DesignCode);
	DDX_Check(pDX, IDC_DGN_SRCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Radio(pDX, IDC_DGN_SAFETY_LEVEL1, m_iSafeLevel);
	DDX_Radio(pDX, IDC_DGN_EQ_GRADE1, m_iEqGrade);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	DDX_Text(pDX,  IDC_DGN_EQ_FACTOR7, m_dAlpha1);
	DDX_Text(pDX,  IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	//}}AFX_DATA_MAP
	CArray<UINT,UINT> arID2;  IDSRC2(0)IDSRC2(1)IDSRC2(2)IDSRC2(3);
	CArray<UINT,UINT> arID3;  IDSRC3(0)IDSRC3(1)IDSRC3(2)IDSRC3(3)IDSRC3(4);
	CArray<UINT,UINT> arID4;  IDSRC4(0)IDSRC4(1)IDSRC4(2)IDSRC4(3)IDSRC4(4)IDSRC4(5)IDSRC4(6)IDSRC4(7)IDSRC4(8)IDSRC4(9)IDSRC4(10)IDSRC4(11)IDSRC4(12);
	int i=0;
	for(i=0; i<iDgn_SrcID2; i++)	m_iID2[i] = arID2[i];
	for(i=0; i<iDgn_SrcID3; i++)	m_iID3[i] = arID3[i];
	for(i=0; i<iDgn_SrcID4; i++)	m_iID4[i] = arID4[i];
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnSrcCodeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnSrcCodeDlg)
	ON_BN_CLICKED(IDC_DGN_SRCCTRL_SPECIAL, OnDgnSrcctrlSpecial)
	ON_CBN_SELCHANGE(IDC_DGN_SRC_DESIGNCODE, OnSelchangeDgnCtrldataDesigncode)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcCodeDlg message handlers

BOOL CDgnSrcCodeDlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	m_DesignCode.ResetContent();
	CStringArray SrcCodeName;
	CDBLib::GetSrcCodeNameList(SrcCodeName, &m_aCodeEnable);
	for(int i=0; i<SrcCodeName.GetSize(); i++)	
	{
		if (SrcCodeName[i] != _T("GB50068-2018"))
		{
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(SrcCodeName[i]);
#endif
			m_DesignCode.AddString(SrcCodeName[i]);
		}
	}
	m_iSafeLevel	= 0;
	m_iEqGrade		= 0;
	m_iAlphaType1 = 0;
	if(m_pDoc->m_pAttrCtrl->ExistDsrc())  Initial_ExistData();
	else                                  Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSrcCodeDlg::Initial_Data()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	CString CodeName;
	CodeName = rPref.DgnCode.SRCCode;
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(CodeName);
#endif
	int Index = m_DesignCode.FindStringExact(-1, CodeName);
	if(Index!=-1)	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;
	m_bSpecialEQ = FALSE;
	// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (JGJ138-01).
	m_pDgnDataCtrl->Get_DgnSrcChinaDef(rPref.DgnCode.SRCCode, m_iSafeLevel, m_iEqGrade);

	UpdateData(FALSE);
	// Change by ZINU.('02.8.19). For JGJ138-01. 
	// Change by ParkBong.(06.05.18) 
	SelchangeDgnCtrldataDesigncode(TRUE);
}

void CDgnSrcCodeDlg::Initial_ExistData()
{
	T_DSRC_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDsrc(rData);
	m_bSpecialEQ	= rData.bSpecialSeismic;
	// Change by ZINU.('05.11.25). For Seismic Design.
	int iStatus = GetDataStatus(rData.DesignCode);
	m_iSafeLevel	= (iStatus==2 ? rData.nClass : 0);
	m_iEqGrade		= (iStatus==3 ? rData.nClass : 0);

	int Index=0;
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(rData.DesignCode);
#endif
	if(rData.DesignCode!=_T(""))	Index = m_DesignCode.FindStringExact(-1,rData.DesignCode);
	else											Index = 0;
	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;
	UpdateData(FALSE);
	// Change by ZINU.('02.8.19). For JGJ138-01.
	// Change by ParkBong.(06.05.18)
	SelchangeDgnCtrldataDesigncode(FALSE);
}

void CDgnSrcCodeDlg::OnOK() 
{
	UpdateData(TRUE);
	T_DSRC_D rData;
	rData.Initialize();
	// Exist SRC Control Data at DB.
	if(m_pDoc->m_pAttrCtrl->ExistDsrc())	m_pDoc->m_pAttrCtrl->GetDsrc(rData);

	// Get Design Code.
	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index,rData.DesignCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(rData.DesignCode);
#endif
	// Get Special Seismic and Uni Axis.
	rData.bSpecialSeismic = m_bSpecialEQ;
	// Change by ZINU.('05.11.25). For Seismic Design.
	int iStatus = GetDataStatus(rData.DesignCode);
	if(iStatus==2)	    rData.nClass = m_iSafeLevel;
	else if(iStatus==3)	rData.nClass = m_iEqGrade;
	else								rData.nClass = 0;

	rData.iAlphaType  = m_iAlphaType1;
	rData.dAlpha1     = m_dAlpha1;
	rData.dAlpha2     = m_dAlpha2;

	// Save SRC Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDsrc(rData);
	if(bCheck)	
	{
		CDialogMove::OnOK();
	}
}

void CDgnSrcCodeDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnSrcCodeDlg::OnDgnSrcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeDgnCtrldataDesigncode();
}

void CDgnSrcCodeDlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CDgnSrcCodeDlg::SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef) 
{
	// TODO: Add your control notification handler code here
	// Change by ZINU.('02.8.19). For Dynamic Size.
	UpdateData(TRUE);

	CString strCode=_T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	int iStatus = GetDataStatus(strCode);
	//Initialize Parameter by Changing Code
	T_DSRC_D rData;
	rData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDsrc())	m_pDoc->m_pAttrCtrl->GetDsrc(rData);
	if(rData.DesignCode == strCode)
	{
		m_iAlphaType1 = rData.iAlphaType;
		m_dAlpha1 = rData.dAlpha1;
		m_dAlpha2 = rData.dAlpha2;
	}
	else
	{
		m_iAlphaType1 = 0;
		m_dAlpha1 = 0.0;
		m_dAlpha2 = 0.0;
	}
	 // Add by ParkBong.('06.05.18). Add bUpdateDef Option. 
	if(bUpdateDef)
	{
	// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (JGJ138-01).
	m_pDgnDataCtrl->Get_DgnSrcChinaDef(strCode, m_iSafeLevel, m_iEqGrade);
	m_pDgnDataCtrl->Get_DgnSrcSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	}
	int i=0;
	// For JGJ138-01.
	for(i=0; i<iDgn_SrcID2; i++)	GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_SrcID3; i++)	GetDlgItem(m_iID3[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_SrcID4; i++)	GetDlgItem(m_iID4[i])->ShowWindow(SW_HIDE);

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);
	// EQ.
	if(iStatus==2)			RectOrg = MoveCtrl(2,m_iID2);
	else if(iStatus==3)		RectOrg = MoveCtrl(3,m_iID3);
	else if(iStatus==9)		RectOrg = MoveCtrl(4,m_iID4);
	else if(iStatus==15)	RectOrg = MoveCtrl(4,m_iID4);
	else	GetDlgItem(IDC_DGN_SRCCTRL_SPECIAL)->GetWindowRect(&RectOrg);

	CRect rDlg;
	rDlg.left		= RectDlg.left;
	rDlg.top		= RectDlg.top;
	rDlg.right	=	RectDlg.right;
	rDlg.bottom	= RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(24); //(int)(2.5*RectOk.Height());
	CRect rOk;
	rOk.left		= RectOk.left;
	rOk.right		= RectOk.right;
	rOk.top			= rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8); //(int)(1.75*RectOk.Height());
	rOk.bottom	= rOk.top + RectOk.Height();
	CRect rCn;
	rCn.left		= RectCn.left;
	rCn.right		= RectCn.right;
	rCn.top			= rOk.top;
	rCn.bottom	= rOk.bottom;
	
	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK    )->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CDgnSrcCodeDlg::OnSelchangeDgnCtrldataDesigncode() 
{
	int Index = m_DesignCode.GetCurSel();
	if(m_aCodeEnable[Index]==FALSE)
	{
		CString strCodeName = _T("");
		m_DesignCode.GetLBText(Index, strCodeName);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
		GSaveHistoryFormatNF(strErrMsg);
		m_DesignCode.SetCurSel(m_nOldCodeIndex);

		return;
	}

	m_nOldCodeIndex = Index;

	SelchangeDgnCtrldataDesigncode(TRUE);
}

void CDgnSrcCodeDlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode=_T("");
	m_DesignCode.GetWindowText(strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	m_iAlphaType1 = 0;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;

	m_pDgnDataCtrl->Get_DgnSrcSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

CRect CDgnSrcCodeDlg::MoveCtrl(int iKind, int* pID)
{
	CRect RectOrg;
	int iArSize=0;
	if(iKind==2)			iArSize = iDgn_SrcID2;
	else if(iKind==3)	iArSize = iDgn_SrcID3;
	else if(iKind==4)	iArSize = iDgn_SrcID4;
	else	ASSERT(0);
	CRect* pRectCng = new CRect[iArSize];

	CRect Rect0, Rect1;
	GetDlgItem(m_iID2[0])->GetWindowRect(&Rect0);
	for(int i=0; i<iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if(i==0)	Rect1 = pRectCng[i];
		pRectCng[i].top		 -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
	}
	RectOrg = pRectCng[0];
	for(int i=0; i<iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;

	return RectOrg;
}

void CDgnSrcCodeDlg::ChangeDlgItemInfo()
{
	CString strCode=_T("");
	int iIndex = m_DesignCode.GetCurSel();
	if(iIndex >= 0)	m_DesignCode.GetLBText(iIndex, strCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strCode);
#endif
	CString strTxt=_T("");
	int iStatus	= GetDataStatus(strCode);
	strTxt = _LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN);
	GetDlgItem(IDC_DGN_SRCCTRL_SPECIAL)->SetWindowText(strTxt);
	if(iStatus < 0)
	{
		m_bSpecialEQ = FALSE;
		GetDlgItem(IDC_DGN_SRCCTRL_SPECIAL)->EnableWindow(FALSE);
	}
	else	GetDlgItem(IDC_DGN_SRCCTRL_SPECIAL)->EnableWindow(TRUE);

	if(iStatus==9 || iStatus==15)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(FALSE);

		if(m_iAlphaType1==0 || m_iAlphaType1==1)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
		}
		else if(m_iAlphaType1==2)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
		}
	}

	CString strQ=_T("");
	if(iStatus==9)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_AIJ));
	}
	else if(iStatus==15)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_TWN));
	}

	if(strQ!=_T(""))
	{
		CString strMax=_T(""), strMin=_T(""), strQ1=_T(""), strQ2=_T("");
		strQ1.Format(_T("%s1"), strQ);
		strQ2.Format(_T("%s2"), strQ);
		strMax.Format(_T("MAX(%s,%s)"), strQ1,strQ2);
		strMin.Format(_T("MIN(%s,%s)"), strQ1,strQ2);
		GetDlgItem(IDC_DGN_EQ_FACTOR1)->SetWindowText(strMax);
		GetDlgItem(IDC_DGN_EQ_FACTOR2)->SetWindowText(strMin);
		GetDlgItem(IDC_DGN_EQ_FACTOR3)->SetWindowText(strQ1);
		GetDlgItem(IDC_DGN_EQ_FACTOR4)->SetWindowText(strQ2);
		// Add by ZINU.('05.12.26). If AIJ-SRC01, Only iAlphaType=3.
		//*/Temp by ZINU. 계산할 수 있도록 일정과 계산방식이 결정되면 작업필요.
		if(strCode==_T("AIJ-SRC01"))
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR1)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_EQ_FACTOR2)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_EQ_FACTOR3)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_EQ_FACTOR4)->EnableWindow(TRUE);
		}
	}
}

int CDgnSrcCodeDlg::GetDataStatus(CString strCode)
{
	int iStatus=0;
	if(strCode==_T("AIJ-SRC01"))			  iStatus = (m_bSpecialEQ ? 9 : 1);
	else if(strCode==_T("JGJ138-01") || strCode == _T("GB50068-2019"))	  iStatus = (m_bSpecialEQ ? 3 : 2);
	else if(strCode==_T("TWN-SRC92"))		iStatus = (m_bSpecialEQ ? 15 : 0);
	else if(strCode==_T("TWN-SRC100"))	iStatus = (m_bSpecialEQ ? 15 : 0);
	else														iStatus = 0;
	
	return iStatus;
}