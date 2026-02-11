// WindAddLoadKBC2015Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindAddLoadKBC2015Dlg.h"
#include "CMWindItemDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindAddLoadKBC2015Dlg dialog


CWindAddLoadKBC2015Dlg::CWindAddLoadKBC2015Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindAddLoadKBC2015Dlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pWind = 0;
	m_bAcross = TRUE;
	m_bTorsinal = TRUE;
	//{{AFX_DATA_INIT(CWindAddLoadKBC2015Dlg)
	//}}AFX_DATA_INIT
}


void CWindAddLoadKBC2015Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindAddLoadKBC2015Dlg)
	DDX_Control(pDX, IDC_CMD_STORY_TITLE1, m_wndTitle1);
	DDX_Control(pDX, IDC_CMD_STORY_TITLE2, m_wndTitle2);
	DDX_Control(pDX, IDC_CMD_RZ_TITLE, m_wndTitle5);
	DDX_Control(pDX, IDC_CMD_RZ_TITLE2, m_wndTitle6);
	DDX_Control(pDX, IDC_CMD_X_UNIT, m_wndXUnit);
	DDX_Control(pDX, IDC_CMD_Y_UNIT, m_wndYUnit);
	DDX_Control(pDX, IDC_CMD_X_UNIT2, m_wndXUnit2);
	DDX_Control(pDX, IDC_CMD_Y_UNIT2, m_wndYUnit2);	
	DDX_Control(pDX, IDC_CMD_X_EDIT, m_wndXEdit);
	DDX_Control(pDX, IDC_CMD_Y_EDIT, m_wndYEdit);
	DDX_Control(pDX, IDC_CMD_X_EDIT2, m_wndXEdit2);
	DDX_Control(pDX, IDC_CMD_Y_EDIT2, m_wndYEdit2);	
	DDX_Control(pDX, IDC_CMD_RZ_EDIT, m_wndRZEdit);
	DDX_Control(pDX, IDC_CMD_RZ_EDIT2, m_wndRZEdit2);
	DDX_Control(pDX, IDC_CMD_RZ_UNIT, m_wndRZUnit);
	DDX_Control(pDX, IDC_CMD_RZ_UNIT2, m_wndRZUnit2);
	DDX_Control(pDX, IDC_CMD_STORY_END, m_wndStoryEnd);
	DDX_Control(pDX, IDC_CMD_STORY_START, m_wndStoryStart);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------
// Interface
void CWindAddLoadKBC2015Dlg::SetMode(void* pWind, BOOL bAcross, BOOL bTorsinal)
{
	ASSERT(pWind);
	m_pWind = pWind;
	m_bAcross = bAcross;
	m_bTorsinal = bTorsinal;
}

//--------------------------------------------------------------------------
// Implementations
void CWindAddLoadKBC2015Dlg::FillStoryCombo(CComboBox& wndCombo, int nStart)
{
	wndCombo.ResetContent();

	CArray<T_STOR_K, T_STOR_K> KeyList;

	// get all keys
	m_pDoc->m_pAttrCtrl->GetStorKeyList(KeyList);
	
	// get filtered keys
	if(KeyList.GetSize() != 0)
	{
		T_STOR_K StorK_start = KeyList[nStart];
		m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(KeyList, &StorK_start, NULL);
	}
	T_STOR_D Stor;
	for (int i = 0; i < KeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(KeyList[i], Stor);
		int nIndex = wndCombo.AddString(Stor.StoryName);
		wndCombo.SetItemData(nIndex, KeyList[i]);
	}
}

void CWindAddLoadKBC2015Dlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}

void CWindAddLoadKBC2015Dlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STOR_ADD):
		case(UR_STOR_DEL):
		case(UR_STOR_MFD):
		case(UR_STOR_MFS): 
			bStorChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bStorChanged)
	{
		// Combo의 내용을 변경한다.
		FillStoryCombo(m_wndStoryStart, 0);
		FillStoryCombo(m_wndStoryEnd, 0);
		m_wndStoryStart.SetCurSel(0);
		m_wndStoryEnd.SetCurSel(0);
	}
}

BOOL CWindAddLoadKBC2015Dlg::ApplyOrOK()
{
	if (m_wndStoryStart.GetCount() == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Story_is_not_selected_));
		return FALSE;
	}
	
	T_STOR_K StartStorKey, EndStorKey;
	int nIndex = m_wndStoryStart.GetCurSel();
	StartStorKey = m_wndStoryStart.GetItemData(nIndex);
	nIndex = m_wndStoryEnd.GetCurSel();
	EndStorKey = m_wndStoryEnd.GetItemData(nIndex);

	double dblX=0., dblY=0., dblX2=0., dblY2=0., dblRZ=0., dblRZ2=0.;
	dblX = m_wndXEdit.GetEditValue();
	dblY = m_wndYEdit.GetEditValue();
	dblX2 = m_wndXEdit2.GetEditValue();
	dblY2 = m_wndYEdit2.GetEditValue();
	dblRZ = m_wndRZEdit.GetEditValue();
	dblRZ2 = m_wndRZEdit2.GetEditValue();

	// Call Parent's update method according to mode
	((CCMWindItemDlg*)m_pWind)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY, dblX2, dblY2, dblRZ, dblRZ2);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWindAddLoadKBC2015Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindAddLoadKBC2015Dlg)
	ON_BN_CLICKED(IDC_CMD_STORY_BTN, OnCmdStoryBtn)
	ON_CBN_SELCHANGE(IDC_CMD_STORY_START, OnSelchangeCmdStoryStart)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindAddLoadKBC2015Dlg message handlers

BOOL CWindAddLoadKBC2015Dlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	
	m_wndXEdit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddX);
	m_wndXUnit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddX);
	m_wndYEdit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddY);
	m_wndYUnit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddY);
	m_wndXEdit2.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddX);
	m_wndXUnit2.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddX);
	m_wndYEdit2.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddY);
	m_wndYUnit2.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddY);
	m_wndRZEdit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddR);
	m_wndRZUnit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddR);
	m_wndRZEdit2.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddR);
	m_wndRZUnit2.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddR);
	
	FillStoryCombo(m_wndStoryStart, 0);
	FillStoryCombo(m_wndStoryEnd, 0);
	if (m_wndStoryStart.GetCount() > 0)
	{
		m_wndStoryStart.SetCurSel(0);
		m_wndStoryEnd.SetCurSel(0);
	}

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_STORY_TITLE3, m_bAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_STORY_TITLE4, m_bAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RZ_TITLE, m_bTorsinal);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RZ_TITLE2, m_bTorsinal);

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_X_EDIT2, m_bAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_Y_EDIT2, m_bAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RZ_EDIT, m_bTorsinal);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RZ_EDIT2, m_bTorsinal);

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_X_UNIT2, m_bAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_Y_UNIT2, m_bAcross);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RZ_UNIT, m_bTorsinal);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_RZ_UNIT2, m_bTorsinal);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindAddLoadKBC2015Dlg::OnCmdStoryBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STORY,0));	
}

void CWindAddLoadKBC2015Dlg::OnSelchangeCmdStoryStart() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndStoryStart.GetCurSel();
	FillStoryCombo(m_wndStoryEnd, nCurSel);
	m_wndStoryEnd.SetCurSel(0);	
}

void CWindAddLoadKBC2015Dlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CWindAddLoadKBC2015Dlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}
