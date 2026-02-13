// DgnStlRatingRivetsDlg.cpp: implementation of the CDgnStlRatingRivetsDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingRivetsDlg.h"
#include "DgnStlRatingServiceParamTabIDlg.h"
#include "DgnStlRatingServiceParamTabJDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CDgnStlRatingRivetsDlg::CDgnStlRatingRivetsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlRatingRivetsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranStifDlg)
	m_nOption = 0;
	//}}AFX_DATA_INITz

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
	m_DataI.Initialize();
	m_DataJ.Initialize();

	m_bBothIJ = TRUE;
}

void CDgnStlRatingRivetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CDgnStlRatingRivetsDlg)
	DDX_Radio(pDX, IDC_DGN_OPTION_AR_RDO, m_nOption);
	DDX_Control(pDX, IDC_DGN_IJ_BOTH_CHK, m_Ctrl_BothIJ);
	DDX_Control(pDX, IDC_DGN_RIVETS_TAB, m_Tab);
	
	//}}AFX_DATA_MAP
}

void CDgnStlRatingRivetsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
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

void CDgnStlRatingRivetsDlg::UpdateBuffer()
{
	if (m_Tab.GetCurSel() == 0)
	{
		m_pSubDlg_I->ShowHideControls();
	}
	else
	{
		m_pSubDlg_J->ShowHideControls();
	}
}

void CDgnStlRatingRivetsDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

BOOL CDgnStlRatingRivetsDlg::Dlg2Data()
{
	m_Data.bSameIJ = m_Ctrl_BothIJ.GetCheck();

	if (m_Tab.GetCurSel() == 0)
	{
		if (m_Data.bSameIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetRivtData(m_DataI);
			m_Data.RivtSub[0] = m_DataI;
			m_Data.RivtSub[1] = m_DataI;
		}
		else
		{
			m_pSubDlg_I->GetRivtData(m_DataI);
			m_Data.RivtSub[0] = m_DataI;
			m_Data.RivtSub[1] = m_DataJ;
		}
	}
	else
	{
		if (m_Data.bSameIJ)
		{
			m_Data.RivtSub[0] = m_DataI;
			m_Data.RivtSub[1] = m_DataI;
		}
		else
		{
			m_pSubDlg_J->GetRivtData(m_DataJ);
			m_Data.RivtSub[0] = m_DataI;
			m_Data.RivtSub[1] = m_DataJ;
		}
	}

	return TRUE;
}

void CDgnStlRatingRivetsDlg::EnableDisableControls()
{
	m_bBothIJ = m_Ctrl_BothIJ.GetCheck();

	BOOL bEnable = TRUE;

	if (m_Tab.GetCurSel() == 0)
	{
		if (m_nOption == 1) bEnable = FALSE;

		m_pSubDlg_I->EnableDisableControls(bEnable);
	}
	else
	{
		if (m_bBothIJ) bEnable = FALSE;
		if (m_nOption == 1) bEnable = FALSE;

		m_pSubDlg_J->EnableDisableControls(bEnable);
	}
}

BEGIN_MESSAGE_MAP(CDgnStlRatingRivetsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlRatingRivetsDlg)
	ON_BN_CLICKED(IDC_DGN_OPTION_AR_RDO, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_OPTION_DEL_RDO, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_IJ_BOTH_CHK, OnDgnBothIJChk)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_RIVETS_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnStlRatingRivetsDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	if (m_pDoc == NULL)	return TRUE;

	m_Ctrl_BothIJ.SetCheck(TRUE);

	m_Tab.DeleteAllItems();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_RIVETS_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_Tab.SetPlaceHolder(pPlaceHolder, FALSE);

	// I
	m_pSubDlg_I = new CDgnStlRatingRivetsTabDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_STL_RATING_I), CDgnStlRatingRivetsTabDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CDgnStlRatingRivetsTabDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_STL_RATING_J), CDgnStlRatingRivetsTabDlg::IDD, TRUE);

	m_Tab.ShowTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingRivetsDlg::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		if (m_nOption == 0)	// add/replace
		{
			Dlg2Data();
			if (m_pDoc->m_pDataCtrl->AddRivt(aSelKey, m_Data))	Initial_SelectItem();
		}
		else	// Delete
		{
			if (m_pDoc->m_pDataCtrl->DelRivt(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_STL_RATING_BEAM_NON_MEMBER));
}

void CDgnStlRatingRivetsDlg::OnDgnOption()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_IJ_BOTH_CHK)->EnableWindow(m_nOption == 0);

	EnableDisableControls();
}

void CDgnStlRatingRivetsDlg::OnDgnBothIJChk()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnStlRatingRivetsDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);

	EnableDisableControls();
}


