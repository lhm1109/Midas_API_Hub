// RCPrintOption.cpp: implementation of the CRCPrintOption class.
// Add by sshan. MNET:2067.('20061220)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCPrintOption.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainres2.h"	

#include "CRCDataCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CRCPrintOption::CRCPrintOption(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CRCPrintOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCPrintOption)
	m_nOption = -1;
	m_nMPOption = -1;
	m_nMNOption = -1;
	m_nSOption = -1;  
	m_nTOption = -1;
	m_nTenOption = -1;
	m_nCompOption = -1;
	m_nRPOption = -1;
	m_nRNOption = -1;
	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CRCPrintOption::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCPrintOption)
	DDX_Radio(pDX, IDC_DGN_RC_PORC_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_MP1, m_nMPOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_MN1, m_nMNOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_V1, m_nSOption);  
	DDX_Radio(pDX, IDC_DGN_RC_PORC_T1, m_nTOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_TEN1, m_nTenOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_COMP1, m_nCompOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_RP1, m_nRPOption);
	DDX_Radio(pDX, IDC_DGN_RC_PORC_RN1, m_nRNOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCPrintOption, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CRCPrintOption)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_RC_PORC_AR, OnDgnCRCPorcAr)
	ON_BN_CLICKED(IDC_DGN_RC_PORC_DEL, OnDgnCRCPorcDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CRCPrintOption::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	SetControlLocation();

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCPrintOption::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CRCPrintOption::OnDgnExecute() 
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			//modify by maxiao 2007.01.08
			T_ELEM_D ElemD;
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,ElemD))  ElemD.Initialize();
			T_MATD_D MatdD;
			m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD);
			if(MatdD.Type==_T("C")) //concret 단면만 가능
				rSelKey.Add(key);     
		}

		T_PORC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.iMomChk[0] = m_nMPOption;   // 휨검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
			rData.iMomChk[1] = m_nMNOption;
			rData.iShrChk    = m_nSOption;    // 전단검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)  
			rData.iTorChk    = m_nTOption;    // 비틀림검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
			rData.iTenChk    = m_nTenOption;
			rData.iComChk    = m_nCompOption;
			rData.iBarChk[0] = m_nRPOption;   // 철근검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
			rData.iBarChk[1] = m_nRNOption;      

			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
		    if(m_pDoc->m_pDataCtrl->AddPorc(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelPorc(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CRCPrintOption::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CRCPrintOption::Initial_Data()
{
	m_nOption  = 0;	// add/replace

	m_nMPOption = 3;
	m_nMNOption = 3;
	m_nSOption = 3;  
	m_nTOption = 3;
	m_nTenOption = 3;
	m_nCompOption = 3;
	m_nRPOption = 3;
	m_nRNOption = 3;  

	UpdateData(FALSE);
}

void CRCPrintOption::OnDgnCRCPorcAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CRCPrintOption::OnDgnCRCPorcDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CRCPrintOption::SetControlLocation() 
{
	T_CHRP_D RcD; RcD.Initialize();
	m_pDoc->m_pAttrCtrl->GetChrp(RcD);

	CString strCodeName=_T("");  
	strCodeName = CDBLib::GetCHRcCodeName(RcD.iDgnCode);

	static CArray<UINT, UINT> arCtrl;
	CDlgUtil::GetCtrlIDByIncRect(this, arCtrl, IDC_DGN_RC_RAIL_HOLDER, FALSE);

	CWnd* pTensHolder = GetDlgItem(IDC_DGN_TENS_HOLDER);
	CWnd* pTorsHolder = GetDlgItem(IDC_DGN_TORS_HOLDER);
	CWnd* pRailHolder = GetDlgItem(IDC_DGN_RC_RAIL_HOLDER);
	CRect TorsHRect, TensHRect, RailHRect;
	pTensHolder->GetWindowRect(TensHRect);
	pTorsHolder->GetWindowRect(TorsHRect);
	pRailHolder->GetWindowRect(RailHRect);
	
	//if(strCodeName == _T("JTG D62-04") || strCodeName == _T("CJJ11-2011"))
	if(RcD.iDgnCode == RC_JTG_D62_04 || RcD.iDgnCode == RC_CJJ_11_2011)
	{
		GetDlgItem(IDC_DGN_TORS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_PORC_T1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_PORC_T2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_PORC_T3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_PORC_T4)->ShowWindow(SW_SHOW);

//		GetDlgItem(IDC_DGN_REIN_STATIC)->SetWindowText(_T("Reinforcement"));

		if(TensHRect.top == RailHRect.top)
			return;
		else
		{
			int DY = TensHRect.top - RailHRect.top;  
			CDlgUtil::CtrlMoveDistY(this, arCtrl, DY, TRUE);
		}
		
	}
	//else if(strCodeName == TB_10002_3_05)
	else if(RcD.iDgnCode == TB_10002_3_05)
	{
		GetDlgItem(IDC_DGN_TORS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_PORC_T1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_PORC_T2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_PORC_T3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_PORC_T4)->ShowWindow(SW_HIDE);

//		GetDlgItem(IDC_DGN_REIN_STATIC)->SetWindowText(_T("Reinforcement Estimation"));
		GetDlgItem(IDC_DGN_REIN_STATIC)->SetWindowText(_LS(IDS_DGN_DILG_REINFORCR_STRESS));

//		if(TorsHRect.top == RailHRect.top)
		if(TensHRect.top == RailHRect.top)
			return;
		else
		{
			int DY = TorsHRect.top - RailHRect.top;  
			CDlgUtil::CtrlMoveDistY(this, arCtrl, DY, TRUE);
		}		
	}
	else
	{
		ASSERT(0);
		return;
	}
}

void CRCPrintOption::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{  
		case D_UPDATE_BUFFER_BEFORE:
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		default:
			//ASSERT(FALSE);
		break;
	}
}

void CRCPrintOption::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_CHRP_ADD):
		case(UR_CHRP_DEL): 
		{
			SetControlLocation();
			return;
		}
		default:
		  break;
		}
	}
}
