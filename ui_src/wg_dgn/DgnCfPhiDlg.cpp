// DgnCfPhiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfPhiDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_main\wg_mainres2.h"
#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfPhiDlg dialog


CDgnCfPhiDlg::CDgnCfPhiDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCfPhiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCfPhiDlg)
	m_FPhiT1=0.0;
	m_FPhiT2=0.0;
	m_FPhiC=0.0;
	m_FPhiB=0.0;
	m_FPhiS=0.0;

	m_MRm0=0.0;
	m_MRm1=0.0;
	m_MRm2=0.0;
	m_M2Rm1=0.0;
	m_M2Rm2=0.0;
	m_M3Phi=0.0;
	m_M4Rm=0.0;

	m_strTitle = _T("");
	m_strCode = _T("");
	//}}AFX_DATA_INIT
}

void CDgnCfPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfPhiDlg)
	DDX_Text(pDX, IDC_DGN_STL_F_PHIT1, m_FPhiT1);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIT2, m_FPhiT2);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIC, m_FPhiC);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIB, m_FPhiB);
	DDX_Text(pDX, IDC_DGN_STL_F_PHIS, m_FPhiS);

	DDX_Text(pDX, IDC_DGN_STL_M_RM0, m_MRm0);
	DDX_Text(pDX, IDC_DGN_STL_M_RM1, m_MRm1);
	DDX_Text(pDX, IDC_DGN_STL_M_RM2, m_MRm2);
	DDX_Text(pDX, IDC_DGN_STL_M2_RM1, m_M2Rm1);
	DDX_Text(pDX, IDC_DGN_STL_M2_RM2, m_M2Rm2);
	DDX_Text(pDX, IDC_DGN_STL_M3_PHI, m_M3Phi);
	DDX_Text(pDX, IDC_DGN_STL_M4_RM,  m_M4Rm);

	//}}AFX_DATA_MAP

	// Strength Reduction Factors 
	m_iFPHIID[0] = IDC_DGN_STL_F_FRAME;
	m_iFPHIID[1] = IDC_DGN_STL_F_PHIT_STR;
	m_iFPHIID[2] = IDC_DGN_STL_F_PHIT1_STR;
	m_iFPHIID[3] = IDC_DGN_STL_F_PHIT2_STR;
	m_iFPHIID[4] = IDC_DGN_STL_F_PHIC_STR;
	m_iFPHIID[5] = IDC_DGN_STL_F_PHIB_STR;
	m_iFPHIID[6] = IDC_DGN_STL_F_PHIS_STR;
	m_iFPHIID[7] = IDC_DGN_STL_F_PHIT1;
	m_iFPHIID[8] = IDC_DGN_STL_F_PHIT2;
	m_iFPHIID[9] = IDC_DGN_STL_F_PHIC;
	m_iFPHIID[10] = IDC_DGN_STL_F_PHIB;
	m_iFPHIID[11] = IDC_DGN_STL_F_PHIS;

	//Partial Safety Factors
	m_iMPHIID1[0] = IDC_DGN_STL_M_FRAME;
	m_iMPHIID1[1] = IDC_DGN_STL_M_RM0_STR;
	m_iMPHIID1[2] = IDC_DGN_STL_M_RM1_STR;
	m_iMPHIID1[3] = IDC_DGN_STL_M_RM2_STR;
	m_iMPHIID1[4] = IDC_DGN_STL_M_RM0;
	m_iMPHIID1[5] = IDC_DGN_STL_M_RM1;
	m_iMPHIID1[6] = IDC_DGN_STL_M_RM2;

	//Partial Safety Factors
	m_iMPHIID2[0] = IDC_DGN_STL_M2_FRAME;
	m_iMPHIID2[1] = IDC_DGN_STL_M2_RM1_STR;
	m_iMPHIID2[2] = IDC_DGN_STL_M2_RM2_STR;
	m_iMPHIID2[3] = IDC_DGN_STL_M2_RM1;
	m_iMPHIID2[4] = IDC_DGN_STL_M2_RM2;

	//Safety Factor for Capacity of  Materials
	m_iMPHIID3[0] = IDC_DGN_STL_M3_FRAME;
	m_iMPHIID3[1] = IDC_DGN_STL_M3_PHI_STR;
	m_iMPHIID3[2] = IDC_DGN_STL_M3_PHI;
	
	//Partial Safety Factors
	m_iMPHIID4[0] = IDC_DGN_STL_M4_FRAME;
	m_iMPHIID4[1] = IDC_DGN_STL_M4_RM_STR;
	m_iMPHIID4[2] = IDC_DGN_STL_M4_RM;
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCfPhiDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnCfPhiDlg)
	ON_BN_CLICKED(IDC_DGN_PHI_UPDATE_BTN, OnDgnPhiUpdateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfPhiDlg message handlers

BOOL CDgnCfPhiDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	if(m_pDoc->m_pAttrCtrl->ExistDcfs())  Initial_ExistData();
	else                                  Initial_DefaultData();
	Update_InitDataByCode(m_strCode);

	return TRUE;
}

BOOL CDgnCfPhiDlg::Set_DefaultByCode(CString strCode)
{
	if(strCode==_T(""))	return FALSE;
	CDgnDataCtrl DataCtrl;
	double dPhi[5];
	if(!DataCtrl.Get_DgnCfsPhiDef(strCode, dPhi, m_nNationalAnnex))	ASSERT(0);

	int iStatus = GetDataStatus(m_strCode);
	if(iStatus==1)			
	{
	  m_FPhiT1  = dPhi[0];
	  m_FPhiT2  = dPhi[1];
	  m_FPhiC   = dPhi[2];
	  m_FPhiB   = dPhi[3];
	  m_FPhiS   = dPhi[4];
	}
	else if(iStatus==2)	
	{
	  m_MRm0 = dPhi[0];
	  m_MRm1 = dPhi[1];
	  m_MRm2 = dPhi[2];
	}
	else if(iStatus==3)	
	{
	  m_M2Rm1 = dPhi[0];
	  m_M2Rm2 = dPhi[1];
	}
	else if(iStatus==4)	
	{
	  m_M3Phi = dPhi[0];
	}
	else if(iStatus==5)
	{
	  m_M4Rm = dPhi[0];
	}
	else	ASSERT(0);

	return TRUE;
}

void CDgnCfPhiDlg::Initial_DefaultData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	m_strCode = rPref.DgnCode.CFSteelCode;
	m_nNationalAnnex = rPref.DgnCode.nCfsNationalAnnex;
	if(!Set_DefaultByCode(m_strCode))	ASSERT(0);
	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnCfPhiDlg::Initial_ExistData()
{
	T_DCFS_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcfs(rData);
	if(rData.DesignCode==_T(""))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		m_strCode = rPref.DgnCode.CFSteelCode;
		if(!Set_DefaultByCode(m_strCode))	ASSERT(0);
	}
	else
	{
		m_strCode = rData.DesignCode;
		m_nNationalAnnex = rData.nNationalAnnex;
		CDgnDataCtrl DataCtrl;
		double dPhi[5];
		if(!DataCtrl.Get_DgnCfsPhiDef(m_strCode, dPhi, m_nNationalAnnex))	ASSERT(0);
		int iStatus = GetDataStatus(m_strCode);
	  if(iStatus==1)			
		{
			m_FPhiT1  = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
	    m_FPhiT2  = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
	    m_FPhiC   = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
	    m_FPhiB   = (rData.dPhi[3] == 0.0 ? dPhi[3] : rData.dPhi[3]);
	    m_FPhiS   = (rData.dPhi[4] == 0.0 ? dPhi[4] : rData.dPhi[4]);
		}
	  else if(iStatus==2)	
		{
	    m_MRm0 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
	    m_MRm1 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
			m_MRm2 = (rData.dPhi[2] == 0.0 ? dPhi[2] : rData.dPhi[2]);
			
		}
	  else if(iStatus==3)	
		{
	    m_M2Rm1 = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
	    m_M2Rm2 = (rData.dPhi[1] == 0.0 ? dPhi[1] : rData.dPhi[1]);
		}
	  else if(iStatus==4)	
		{
	    m_M3Phi = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
		}
		else if(iStatus==5)
		{
	    m_M4Rm = (rData.dPhi[0] == 0.0 ? dPhi[0] : rData.dPhi[0]);
		}
		else	ASSERT(0);
	}
	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnCfPhiDlg::Update_InitDataByCode(CString strCodeName)
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCfs(ID_DGN_CF_DCFS_PSFT, m_strTitle, bShow);

	SetWindowText(m_strTitle);

	int iStatus = GetDataStatus(m_strCode);
	if(iStatus==1)			GetDlgItem(IDC_DGN_STL_F_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==2)	GetDlgItem(IDC_DGN_STL_M_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==3)	GetDlgItem(IDC_DGN_STL_M2_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==4)	GetDlgItem(IDC_DGN_STL_M3_FRAME)->SetWindowText(m_strTitle);
	else if(iStatus==5)	GetDlgItem(IDC_DGN_STL_M4_FRAME)->SetWindowText(m_strTitle);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	else	ASSERT(0);

	if(bShow)
	{
		GetDlgItem(IDC_DGN_STL_F_PHIT1 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_F_PHIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_F_PHIB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_F_PHIS )->EnableWindow(TRUE);

		GetDlgItem(IDC_DGN_STL_M_RM0 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M_RM1 )->EnableWindow(TRUE);
	  GetDlgItem(IDC_DGN_STL_M_RM2 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M2_RM1 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M2_RM2 )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M3_PHI )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_M4_RM )->EnableWindow(TRUE);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	}
	else
	{
		GetDlgItem(IDC_DGN_STL_F_PHIT1 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_F_PHIS )->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_STL_M_RM0 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M_RM1 )->EnableWindow(FALSE);
	  GetDlgItem(IDC_DGN_STL_M_RM2 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M2_RM1 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M2_RM2 )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M3_PHI )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_M4_RM )->EnableWindow(FALSE);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	}

	GetDlgItem(IDC_DGN_STL_M2_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_PHI_EC_RM1));
	GetDlgItem(IDC_DGN_STL_M2_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_PHI_EC_RM2));

	if(m_strCode == _T("Eurocode3-1-3:06"))
	{
		GetDlgItem(IDC_DGN_STL_M_RM0_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_05_RM0));
		GetDlgItem(IDC_DGN_STL_M_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_05_RM1));
		GetDlgItem(IDC_DGN_STL_M_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_05_RM2));  
	}
	else
	{
		GetDlgItem(IDC_DGN_STL_M_RM0_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_93_RM0));
		GetDlgItem(IDC_DGN_STL_M_RM1_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_93_RM1));
		GetDlgItem(IDC_DGN_STL_M_RM2_STR)->SetWindowText(_LS(IDS_DGN_STL_EC3_93_RM2));
	}

	Set_SubCodeTitle();
}

void CDgnCfPhiDlg::Set_SubCodeTitle()
{
	BOOL bShowSubCode = (m_strCode == _T("Eurocode3-1-3:06")) ? TRUE : FALSE;
	
	CString strCodeName = m_strCode;
	if (bShowSubCode)
	{
		CString strSubCode = CNationalAnnexTool::Get_Alpha_3_code(m_nNationalAnnex);
		if(!strSubCode.IsEmpty())
		{
			strCodeName.Format(_T("%s[%s]"), strCodeName, strSubCode);
		}
	}
	GetDlgItem(IDC_DGN_STATIC_RCCODE)->SetWindowText(strCodeName);
}

void CDgnCfPhiDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnCfPhiDlg::OnOK() 
{
	UpdateData(TRUE);

	T_DCFS_D rData;
	rData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDcfs())	m_pDoc->m_pAttrCtrl->GetDcfs(rData);
	else
	{
		rData.DesignCode = m_strCode;
		rData.nNationalAnnex = m_nNationalAnnex;
	}

	int iStatus = GetDataStatus(m_strCode);
	if(iStatus==1)			
	{
	  rData.dPhi[0] = m_FPhiT1;
	  rData.dPhi[1] = m_FPhiT2;
	  rData.dPhi[2] = m_FPhiC;
	  rData.dPhi[3] = m_FPhiB;
	  rData.dPhi[4] = m_FPhiS;
	}
	else if(iStatus==2)	
	{
	  rData.dPhi[0] = m_MRm0;
	  rData.dPhi[1] = m_MRm1;
	  rData.dPhi[2] = m_MRm2;
	}
	else if(iStatus==3)	
	{
	  rData.dPhi[0] = m_M2Rm1;
	  rData.dPhi[1] = m_M2Rm2;
	}
	else if(iStatus==4)	
	{
	  rData.dPhi[0] = m_M3Phi;
	}
	else if(iStatus==5)
	{
	  rData.dPhi[0] = m_M4Rm;
	}
	else	ASSERT(0);

	// Save RC Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcfs(rData);
	if(bCheck)  CDialogMove::OnOK();
}

void CDgnCfPhiDlg::OnDgnPhiUpdateBtn() 
{
	Set_DefaultByCode(m_strCode);
	UpdateData(FALSE);	
	Set_SubCodeTitle();
}

void CDgnCfPhiDlg::InitDlgCtrls() 
{
	// TODO: Add your control notification handler code here
	// Change by ZINU.('02.7.8). For Dynamic Size.
	UpdateData(TRUE);
	int i=0;
	for(i=0; i<iDgn_CFS_FPHIID; i++)	GetDlgItem(m_iFPHIID[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_CFS_MPHIID1; i++)	GetDlgItem(m_iMPHIID1[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_CFS_MPHIID2; i++)	GetDlgItem(m_iMPHIID2[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_CFS_MPHIID3; i++)	GetDlgItem(m_iMPHIID3[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_CFS_MPHIID4; i++)	GetDlgItem(m_iMPHIID4[i])->ShowWindow(SW_HIDE);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	int iStatus = GetDataStatus(m_strCode);
	if(iStatus==1)			RectOrg = MoveCtrl(1,m_iFPHIID);
	else if(iStatus==2)	RectOrg = MoveCtrl(2,m_iMPHIID1);
	else if(iStatus==3)	RectOrg = MoveCtrl(3,m_iMPHIID2);
	else if(iStatus==4)	RectOrg = MoveCtrl(4,m_iMPHIID3);
	else if(iStatus==5)	RectOrg = MoveCtrl(5,m_iMPHIID4);// Coded by Seungjun MNET:No.2086 BS5950-2K ('20060627)
	else								ASSERT(0);

	CRect rDlg;
	rDlg.left		= RectDlg.left;
	rDlg.top		= RectDlg.top;
	rDlg.right	=	RectDlg.right;
	rDlg.bottom	= RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(24);//(int)(2.5*RectOk.Height());

	CRect rOk;
	rOk.left		= RectOk.left;
	rOk.right		= RectOk.right;
	rOk.top			= rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8);
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

	Set_SubCodeTitle();
}

CRect CDgnCfPhiDlg::MoveCtrl(int iKind, int* pID)
{
	CRect RectOrg;
	int iArSize=0;
	if(iKind==1)			iArSize = iDgn_CFS_FPHIID;
	else if(iKind==2)	iArSize = iDgn_CFS_MPHIID1;
	else if(iKind==3)	iArSize = iDgn_CFS_MPHIID2;
	else if(iKind==4)	iArSize = iDgn_CFS_MPHIID3;
	else if(iKind==5)	iArSize = iDgn_CFS_MPHIID4;
	else	ASSERT(0);

	CRect* pRectCng = new CRect[iArSize];

	CRect Rect0, Rect1;
	GetDlgItem(m_iFPHIID[0])->GetWindowRect(&Rect0);
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

int CDgnCfPhiDlg::GetDataStatus(CString strCode)
{
	int iStatus=0;

	if(strCode==_T("Eurocode3-1-3:06"))	  iStatus = 2;
	else														  iStatus = 1;
	
	return iStatus;
}
