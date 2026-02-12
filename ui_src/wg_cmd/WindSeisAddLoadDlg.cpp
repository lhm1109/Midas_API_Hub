// WindSeisAddLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindSeisAddLoadDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"


#include "CMWindItemDlg.h"
#include "SeisItemDlgNew.h"
#include "PsllItemDlg.h"
//#include "SeisItemDlg2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindSeisAddLoadDlg dialog


CWindSeisAddLoadDlg::CWindSeisAddLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindSeisAddLoadDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pWind = m_pSeis = m_pPsll = nullptr;
	//{{AFX_DATA_INIT(CWindSeisAddLoadDlg)
	//}}AFX_DATA_INIT
}


void CWindSeisAddLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindSeisAddLoadDlg)
	DDX_Control(pDX, IDC_CMD_STORY_TITLE2, m_wndTitle2);
	DDX_Control(pDX, IDC_CMD_STORY_TITLE1, m_wndTitle1);
	DDX_Control(pDX, IDC_CMD_RZ_TITLE, m_wndTitle3);
	DDX_Control(pDX, IDC_CMD_Y_UNIT, m_wndYUnit);
	DDX_Control(pDX, IDC_CMD_X_UNIT, m_wndXUnit);
	DDX_Control(pDX, IDC_CMD_Y_EDIT, m_wndYEdit);
	DDX_Control(pDX, IDC_CMD_X_EDIT, m_wndXEdit);
	DDX_Control(pDX, IDC_CMD_RZ_EDIT, m_wndRZEdit);  
	DDX_Control(pDX, IDC_CMD_RZ_UNIT, m_wndRZUnit);  
	DDX_Control(pDX, IDC_CMD_STORY_END, m_wndStoryEnd);
	DDX_Control(pDX, IDC_CMD_STORY_START, m_wndStoryStart);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------
// Interface
void CWindSeisAddLoadDlg::SetMode(void* pWind, void* pSeis, void* pPsll)
{
	ASSERT(pWind || pSeis || pPsll);
	ASSERT(!(pWind && pSeis));
	ASSERT(!(pWind && pPsll));
	ASSERT(!(pSeis && pPsll));
	m_pWind = pWind;
	m_pSeis = pSeis;
	m_pPsll = pPsll;
}

//--------------------------------------------------------------------------
// Implementations
void CWindSeisAddLoadDlg::FillStoryCombo(CComboBox& wndCombo, int nStart)
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
	//int nCount = KeyList.GetSize();
	//for (int i = nStart; i < nCount; i++)
	T_STOR_D Stor;
	for (int i = 0; i < KeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(KeyList[i], Stor);
		int nIndex = wndCombo.AddString(Stor.StoryName);
		wndCombo.SetItemData(nIndex, KeyList[i]);
	}
}

void CWindSeisAddLoadDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		//ASSERT(FALSE);
		break;
	}
}

void CWindSeisAddLoadDlg::UpdateBuffer()
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

BOOL CWindSeisAddLoadDlg::ApplyOrOK()
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

	double dblX, dblY, dblRZ;
	dblX = m_wndXEdit.GetEditValue();
	dblY = m_wndYEdit.GetEditValue();
	dblRZ = m_wndRZEdit.GetEditValue();

	if(m_pWind)
		((CCMWindItemDlg*)m_pWind)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY, dblRZ);
	else if(m_pSeis)
		((CSeisItemDlgNew*)m_pSeis)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY, dblRZ);
	else if(m_pPsll)
		((CPsllItemDlg*)m_pPsll)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY, dblRZ);

	// Call Parent's update method according to mode
//   if (m_bWind)
//     ((CCMWindItemDlg*)m_pWind)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY, dblRZ);
//   else
//     ((CSeisItemDlgNew*)m_pSeis)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY, dblRZ);
// //    ((CSeisItemDlg2*)m_pSeis)->AddAdditionalLoad(StartStorKey, EndStorKey, dblX, dblY);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWindSeisAddLoadDlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindSeisAddLoadDlg)
	ON_BN_CLICKED(IDC_CMD_STORY_BTN, OnCmdStoryBtn)
	ON_CBN_SELCHANGE(IDC_CMD_STORY_START, OnSelchangeCmdStoryStart)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindSeisAddLoadDlg message handlers

BOOL CWindSeisAddLoadDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
//   m_aRZ.Add(IDC_CMD_RZ_TITLE);
//   m_aRZ.Add(IDC_CMD_RZ_EDIT);
//   m_aRZ.Add(IDC_CMD_RZ_UNIT);
//   CDlgUtil::CtrlShowHide(this, m_aRZ, m_bWind);

	// TODO: Add extra initialization here
	if (m_pWind == nullptr)
	{
		SetWindowText(_LS(IDS_WG_CMD__ADDD__Add_Modify_Additional_Seismic_Loa));
		m_wndTitle1.SetWindowText(_LS(IDS_WG_CMD__ADDD__Additional_Seis__X__));
		m_wndTitle2.SetWindowText(_LS(IDS_WG_CMD__ADDD__Additional_Seis__Y__));
		m_wndTitle3.SetWindowText(_LS(IDS_WG_CMD__ADDD__Additional_Seis__RZ__));
		m_wndXEdit.SetUnitType(CUnitCtrl::m_SEIS_UNIT.AddX);
		m_wndXUnit.SetUnitType(CUnitCtrl::m_SEIS_UNIT.AddX);
		m_wndYEdit.SetUnitType(CUnitCtrl::m_SEIS_UNIT.AddY);
		m_wndYUnit.SetUnitType(CUnitCtrl::m_SEIS_UNIT.AddY);    
		m_wndRZEdit.SetUnitType(CUnitCtrl::m_SEIS_UNIT.AddR);
		m_wndRZUnit.SetUnitType(CUnitCtrl::m_SEIS_UNIT.AddR);
	}
	else
	{
		m_wndXEdit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddX);
		m_wndXUnit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddX);
		m_wndYEdit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddY);
		m_wndYUnit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddY);
		m_wndRZEdit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddR);
		m_wndRZUnit.SetUnitType(CUnitCtrl::m_WIND_UNIT.AddR);
	}

	if(m_pPsll)
	{
		auto L_HideCtrl = [&](int nCtrlID)
		{
			GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
		};
		L_HideCtrl(IDC_CMD_RZ_TITLE);
		L_HideCtrl(IDC_CMD_RZ_EDIT);
		L_HideCtrl(IDC_CMD_RZ_UNIT);
	}
	
	FillStoryCombo(m_wndStoryStart, 0);
	FillStoryCombo(m_wndStoryEnd, 0);
	if (m_wndStoryStart.GetCount() > 0)
	{
		m_wndStoryStart.SetCurSel(0);
		m_wndStoryEnd.SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindSeisAddLoadDlg::OnCmdStoryBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STORY,0));	
}

void CWindSeisAddLoadDlg::OnSelchangeCmdStoryStart() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndStoryStart.GetCurSel();
	FillStoryCombo(m_wndStoryEnd, nCurSel);
	m_wndStoryEnd.SetCurSel(0);	
}

void CWindSeisAddLoadDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CWindSeisAddLoadDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}
