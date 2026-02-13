// FatigueCheckDlg.cpp: implementation of the CFatigueCheckDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "FatigueCheckDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CFatigueCheckDlg::CFatigueCheckDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CFatigueCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFatigueCheckDlg)
	m_nOption = 0;
	m_bBothIJ = TRUE;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
}

void CFatigueCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFatigueCheckDlg)
									 
	DDX_Radio  (pDX, IDC_DGN_DESIGN_FATI_CHK_ADD_RDO,    m_nOption);                   
	DDX_Check  (pDX, IDC_DGN_DESIGN_FATI_CHK_BOTH_CHK,   m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_DESIGN_FATI_CHK_TAB,        m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFatigueCheckDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CFatigueCheckDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,                          OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE,                        OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_DESIGN_FATI_CHK_ADD_RDO,        OnDgnFatiChkOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_FATI_CHK_DEL_RDO,        OnDgnFatiChkOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_FATI_CHK_BOTH_CHK,       OnDgnFatiChkBothChk)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_DESIGN_FATI_CHK_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFatigueCheckDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		case D_UPDATE_BUFFER_BEFORE:
		case D_UPDATE_UNIT:
		case D_UPDATE_SEL_ADD:
		case D_UPDATE_SEL_DEL:
		// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CFatigueCheckDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bOther=FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_PSCD_ADD):
			case(UR_PSCD_DEL):
				T_PSCD_D rData;
				m_pDoc->m_pAttrCtrl->GetDgnPscd(rData);
				m_nDgnCode = rData.iDgnCode;

				ControlsEnableDisable();
			break;
		}
	}  
}

void CFatigueCheckDlg::InitTab()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_PLACEHOLDER);
	ASSERT(pPlaceHolder);
 	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pSubDlg_I = new CFatigueCheckTabIDlg(this);
	m_ctrlTab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CFatigueCheckTabIDlg::IDD, TRUE);
	m_pSubDlg_J = new CFatigueCheckTabJDlg(this);
	m_ctrlTab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CFatigueCheckTabJDlg::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CFatigueCheckDlg::ControlsEnableDisable()
{
	BOOL bEnable = FALSE;
	BOOL bAllowCode = FALSE;
	
	if(m_nOption==0) bEnable = TRUE;
	else             bEnable = FALSE;
	
	if(m_nDgnCode==EUROCODE2_2_05_PSC) bAllowCode = TRUE;
	else                               bAllowCode = FALSE;

	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_OPTION_GRP)->EnableWindow(bAllowCode);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_ADD_RDO   )->EnableWindow(bAllowCode);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_DEL_RDO   )->EnableWindow(bAllowCode);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_BOTH_CHK  )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_DESIGN_FATI_CHK_TAB       )->EnableWindow(bAllowCode && bEnable);
	GetDlgItem(IDC_DGN_EXECUTE                   )->EnableWindow(bAllowCode);

	if(m_ctrlTab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
	else                           m_pSubDlg_J->ControlsEnableDisable();
}

void CFatigueCheckDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bBothIJ = m_bBothIJ;

	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pSubDlg_I->Dlg2Data();
		m_pSubDlg_I->GetData(m_Data);
	}
	else
	{
		m_pSubDlg_J->Dlg2Data();
		m_pSubDlg_J->GetData(m_Data);
	}

	if(m_Data.bBothIJ)
	{
		m_Data.dt0[1] = m_Data.dt0[0];
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CFatigueCheckDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	T_PSCD_D rData;
	m_pDoc->m_pAttrCtrl->GetDgnPscd(rData);
	m_nDgnCode = rData.iDgnCode;

	InitTab();

	UpdateData(FALSE);

	ControlsEnableDisable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFatigueCheckDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CFatigueCheckDlg::OnDgnExecute() 
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
			T_ELEM_D eData;
			eData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
			ElemPairK EPairK(key, EN_EL_BEAM);
			if(CDBLib::IsPscDgnFor1st(EPairK,TRUE))	rSelKey.Add(key);
		}

		if(m_nOption==0)	// add/replace
	  {
		  Dlg2Data();

			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
		    if(m_pDoc->m_pDataCtrl->AddFack(rSelKey,m_Data))
				{
					if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
				}
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelFack(rSelKey))
			{
				if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CFatigueCheckDlg::OnDgnFatiChkOptionRdo() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CFatigueCheckDlg::OnDgnFatiChkBothChk() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CFatigueCheckDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ControlsEnableDisable();
}