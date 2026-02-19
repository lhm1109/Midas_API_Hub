// DgnStlBoltDeductionDlg.cpp: implementation of the CDgnStlBoltDeductionDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlBoltDeductionDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_dbLock\LockOption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnStlBoltDeductionDlg::CDgnStlBoltDeductionDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlBoltDeductionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranStifDlg)
	m_nOption = 0;
	m_bBoth = TRUE;
	//}}AFX_DATA_INIT

	m_Data_I.Initialize();
	m_Data_J.Initialize();

	m_Data.Initialize();
}

void CDgnStlBoltDeductionDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDgnStlBoltDeductionDlg)
	DDX_Radio(pDX, IDC_DGN_STL_OPT_ADD_RDO, m_nOption);
	DDX_Check(pDX, IDC_DGN_STL_BOTH_CHK   , m_bBoth);
	DDX_Control(pDX, IDC_DGN_STL_STBD_TAB , m_Tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnStlBoltDeductionDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlBoltDeductionDlg)
	//ON_BN_CLICKED(IDC_DGN_CLOSE  , OnDgnClose)
	//ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_STL_OPT_ADD_RDO, OnDgnAddDel)
	ON_BN_CLICKED(IDC_DGN_STL_OPT_DEL_RDO, OnDgnAddDel)
	ON_BN_CLICKED(IDC_DGN_STL_BOTH_CHK   , OnBothIJChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnStlBoltDeductionDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	if (m_pDoc == NULL)	return TRUE;

	m_strDgnCode = CDBLib::GetStlCodeName();

	m_Tab.DeleteAllItems();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_STL_STBD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_Tab.SetPlaceHolder(pPlaceHolder);

	m_pSubDlg_I = new CDgnStlBoltDeductionPage(this, 0);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CDgnStlBoltDeductionPage::IDD, TRUE);
	m_pSubDlg_J = new CDgnStlBoltDeductionPage(this, 1);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CDgnStlBoltDeductionPage::IDD, TRUE);

	m_Tab.ShowTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// void CDgnStlBoltDeductionDlg::OnDgnClose()
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnStlBoltDeductionDlg::Execute()
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

	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		if (m_nOption == 0)	// add/replace
		{
			Dlg2Data();
			if (m_pDoc->m_pDataCtrl->AddStbd(aSelKey, m_Data))	Initial_SelectItem();
		}
		else	// Delete
		{
			if (m_pDoc->m_pDataCtrl->DelStbd(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
}

void CDgnStlBoltDeductionDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnStlBoltDeductionDlg::SetDgnCode(int nDgnCode)
{
	ASSERT(0);
	//m_nDgnCode = nDgnCode;
}

int  CDgnStlBoltDeductionDlg::GetDgnCode()
{
	ASSERT(0);
	//return m_nDgnCode;
	return 0;
}

void CDgnStlBoltDeductionDlg::SubDlgCtrlEnableDisable()
{
	OnInitDialog();
	OnDgnAddDel();
}

void CDgnStlBoltDeductionDlg::OnBothIJChk()
{
	UpdateData(TRUE);

	if (m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->ControlsEnableDisable();
}

BOOL CDgnStlBoltDeductionDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bBothIJ = m_bBoth;

	if (m_Tab.GetCurSel() == 0) m_pSubDlg_I->GetDohlData();
	else                        m_pSubDlg_J->GetDohlData();

	if (m_Data.bBothIJ)
	{
		m_Data.StbdType[0] = m_Data_I;
		m_Data.StbdType[1] = m_Data_I;
	}
	else
	{
		m_Data.StbdType[0] = m_Data_I;
		m_Data.StbdType[1] = m_Data_J;
	}

	return TRUE;
}

void CDgnStlBoltDeductionDlg::OnDgnAddDel()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STL_BOTH_CHK)->EnableWindow(m_nOption == 0);

	if (m_Tab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
	else                       m_pSubDlg_J->ControlsEnableDisable();
}

void CDgnStlBoltDeductionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnStlBoltDeductionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bOther = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
			case(UR_DSTL_ADD):
			case(UR_DSTL_DEL):
			{
				const CString& strDgnCode = CDBLib::GetStlCodeName();

				BOOL bAllowCode = FALSE;

				if (strDgnCode == STLCODE_JROAD_H24 || strDgnCode == STLCODE_JROAD_H14 ||
					strDgnCode == STLCODE_JROAD_H29 || strDgnCode == STLCODE_JROAD_R07)
				{
					bAllowCode = TRUE;
				}

				GetDlgItem(IDC_DGN_STL_BOTH_CHK)->EnableWindow(bAllowCode && m_nOption == 0);

				if (m_Tab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
				else                        m_pSubDlg_J->ControlsEnableDisable();
				break;
			}
			default:	
				break;
		}
	}
}