// DgnGenSlenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSlenDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_dbLock\LockOption.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSlenDlg dialog


CDgnGenSlenDlg::CDgnGenSlenDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenSlenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenSlenDlg)
	m_CLMT = 0.0;
	m_TLMT = 0.0;
	m_NotCheck = FALSE;
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenSlenDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenSlenDlg)
	DDX_Text(pDX, IDC_DGN_SLENDER_CLMT, m_CLMT);
	DDX_Text(pDX, IDC_DGN_SLENDER_TLMT, m_TLMT);
	DDX_Check(pDX, IDC_DGN_SLENDER_NOCHECK, m_NotCheck);
	DDX_Radio(pDX, IDC_DGN_SLENDER_AR, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenSlenDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenSlenDlg)
	ON_BN_CLICKED(IDC_DGN_SLENDER_AR, OnDgnSlenderAr)
	ON_BN_CLICKED(IDC_DGN_SLENDER_DEL, OnDgnSlenderDel)
	ON_BN_CLICKED(IDC_DGN_SLENDER_NOCHECK, OnDgnSlenderNocheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSlenDlg message handlers

BOOL CDgnGenSlenDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount())  m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	//  Data를 초기화합니다.
	Initial_Data();
	Update_DataByCode();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;
}

void CDgnGenSlenDlg::Initial_Data()
{
	m_nOption  = 0;			  //	add/replace
	m_NotCheck = FALSE;		//	Not Check

	T_DSTL_D DstlData;
	CDgnDataCtrl DataCtrl;
	DataCtrl.Get_DgnStlDstl(DstlData);
	
	// MNET:3774-Seungjun-20081030
	if(DstlData.DesignCode==_T("IS:800-2007"))
	{
		m_CLMT = 180.0; m_TLMT = 400.0;
	}
	else if (DstlData.DesignCode==_T("CSA-S6-14"))
	{
		m_CLMT = 120.0; m_TLMT = 200.0;  // UNBRACE MEMNER
	}
	else if (m_pDoc->IsSteelJapanJARA())
	{
		m_CLMT = 120.0; m_TLMT = 200.0;  // UNBRACE MEMNER
	}
	else
	{
		m_CLMT = 200.0; m_TLMT = 300.0;
	}

	GetDlgItem(IDC_DGN_SLENDER_CLMT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_SLENDER_TLMT)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CDgnGenSlenDlg::Execute() 
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
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
		}
		T_LTSR_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.bNotCheck = m_NotCheck;
			rData.CLMT      = m_CLMT;
			rData.TLMT      = m_TLMT;
			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddLtsr(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelLtsr(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}


void CDgnGenSlenDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnGenSlenDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_CLMT < 0.)	bCheck = FALSE;
	if(m_TLMT < 0.)	bCheck = FALSE;
	if(!bCheck)	
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_LIM_SLEN_RATIO1),MB_OK);
		return bCheck;
	}
	return TRUE;
}

void CDgnGenSlenDlg::OnDgnSlenderAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenSlenDlg::OnDgnSlenderDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnGenSlenDlg::OnDgnSlenderNocheck() 
{
	UpdateData(TRUE);

	if(m_NotCheck)  //  Not Check Slenderness Ratio
	{
	  m_CLMT = 0.;
		m_TLMT = 0.;

		GetDlgItem(IDC_DGN_SLENDER_CLMT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SLENDER_TLMT)->EnableWindow(FALSE);

		UpdateData(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SLENDER_CLMT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SLENDER_TLMT)->EnableWindow(TRUE);
	}
}

void CDgnGenSlenDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
		// do something...
			break;
		case D_UPDATE_UNIT:
		// do something...
			break;
		case D_UPDATE_SEL_ADD:
		// do something...
			break;
		case D_UPDATE_SEL_DEL:
		// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			// do something...
			UpdateBuffer();
			break;
		case D_UPDATE_PREF_CHANGED:
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnGenSlenDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;		
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_DSTL_ADD):
			case(UR_DSTL_DEL):
			{
				Update_DataByCode();
				break;
			}
			default:	break;
		}
	}
}

void CDgnGenSlenDlg::Update_DataByCode()
{
	T_DSTL_D DstlData;
	CDgnDataCtrl DataCtrl;
	DataCtrl.Get_DgnStlDstl(DstlData);
	if(DstlData.DesignCode==_T("GBJ17-88") || DstlData.DesignCode==_T("GB50017-03") || DstlData.DesignCode==_T("GB50017-15") || DstlData.DesignCode==_T("GB50017-17"))//add by maxiao(2015-9-25)GB50017-15
	{
		GetDlgItem(IDC_SLEN_FACT_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SLEN_FACT_STATIC2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SLEN_FACT_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SLEN_FACT_STATIC2)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
}