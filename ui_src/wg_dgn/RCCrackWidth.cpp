// RCCrackWidth.cpp : implementation file
// Add by sshan. MNET:2607.('20061220)

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCCrackWidth.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_PostCtrl.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCCrackWidth dialog


CRCCrackWidth::CRCCrackWidth(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CRCCrackWidth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCCrackWidth)
	m_nOption = -1;
	m_dC1 = 0.0;
	m_dC2 = 0.0;
	m_dC3 = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CRCCrackWidth::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCCrackWidth)
	DDX_Radio(pDX, IDC_DGN_RC_CRACK_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_RC_CRACK_C1, m_dC1);
	DDX_Text(pDX, IDC_DGN_RC_CRACK_C2, m_dC2);
	DDX_Text(pDX, IDC_DGN_RC_CRACK_C3, m_dC3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCCrackWidth, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CRCCrackWidth)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_RC_CRACK_AR, OnDgnCrackAr)
	ON_BN_CLICKED(IDC_DGN_RC_CRACK_DEL, OnDgnCrackDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCCrackWidth message handlers

void CRCCrackWidth::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CRCCrackWidth::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
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
		
		T_CWRC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
			CString strCodeName=_T("");  
			strCodeName = GetRCCHCodeName();
			if(strCodeName == _T("JTG D62-04") || strCodeName == _T("CJJ11-2011"))
			{
				rData.dC1 = m_dC1;
				rData.dC2 = m_dC2;
				rData.dC3 = m_dC3;
			}
			else if(strCodeName == _T("TB 10002.3-05"))
			{
				rData.dK1 = m_dC1;
				rData.dA	= m_dC2;
			}
			else ASSERT(0);

			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddCwrc(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelCwrc(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}


void CRCCrackWidth::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}


BOOL CRCCrackWidth::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	SetControl();

	// Default 값 초기화
	Initial_Data();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCCrackWidth::Initial_Data()
{
	m_nOption = 0;				//	add/replace
	CString strCodeName=_T("");  
	strCodeName = GetRCCHCodeName();
	if(strCodeName == _T("JTG D62-04") || strCodeName == _T("CJJ11-2011"))
	{
		m_dC1 = 1.0;	        	
		m_dC2 = 1.0;
		m_dC3 = 1.0;
	}
	else if(strCodeName == _T("TB 10002.3-05"))
	{
		m_dC1 = 0.8;	        	
		m_dC2 = 0.3;
	}
	else ASSERT(0);

	UpdateData(FALSE);
}

BOOL CRCCrackWidth::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_dC1< 1.0)	bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_CTC_FACTOR),MB_OK);

	return bCheck;
}

void CRCCrackWidth::OnDgnCrackAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CRCCrackWidth::OnDgnCrackDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CRCCrackWidth::SetControl() 
{
	CString strCodeName=_T("");  
	strCodeName = GetRCCHCodeName();

	if(strCodeName == _T("JTG D62-04") || strCodeName == _T("CJJ11-2011"))
	{
		GetDlgItem(IDC_C1_STATIC)->SetWindowText(_T("C1"));
		GetDlgItem(IDC_C2_STATIC)->SetWindowText(_T("C2"));

		GetDlgItem(IDC_C3_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_CRACK_C3)->ShowWindow(SW_SHOW);
	}
	else if(strCodeName == _T("TB 10002.3-05"))
	{
		GetDlgItem(IDC_C1_STATIC)->SetWindowText(_T("K1"));
		GetDlgItem(IDC_C2_STATIC)->SetWindowText(_T("a"));

		GetDlgItem(IDC_C3_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_CRACK_C3)->ShowWindow(SW_HIDE);
	}
	else ASSERT(0);
}

CString CRCCrackWidth::GetRCCHCodeName() 
{
	T_CHRP_D RcD; RcD.Initialize();
	m_pDoc->m_pAttrCtrl->GetChrp(RcD);

	CString strCodeName=_T("");  
	strCodeName = CDBLib::GetCHRcCodeName(RcD.iDgnCode);

	return strCodeName;
}

void CRCCrackWidth::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CRCCrackWidth::UpdateBuffer()
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
			OnInitDialog();
			return;
		}
		default:
		  break;
		}
	}
}