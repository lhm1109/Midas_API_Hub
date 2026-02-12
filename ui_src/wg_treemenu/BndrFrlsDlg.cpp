// BndrFrlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrFrlsDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrFrlsDlg dialog


CBndrFrlsDlg::CBndrFrlsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrFrlsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrFrlsDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_aTypeCtrl.Add(IDC_TM_RADIO_RELATIVE);
	m_aTypeCtrl.Add(IDC_TM_RADIO_VALUE);
}

void CBndrFrlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrFrlsDlg)
	DDX_Control(pDX, IDC_TM_TEXT_MXYZ_UNIT, m_textMxyzUnit);
	DDX_Control(pDX, IDC_TM_TEXT_FXYZ_UNIT, m_textFxyzUnit);
	DDX_Radio(pDX, IDC_TM_FRLS_OPT_REPLACE, m_nOption);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC8, m_wndPicture);

	//}}AFX_DATA_MAP
	UINT nID[2][FRLS_SIZE][2] = { 
		{
			{IDC_TM_FRLS_FLAG_I1, IDC_TM_FRLS_VALUE_I1},
			{IDC_TM_FRLS_FLAG_I2, IDC_TM_FRLS_VALUE_I2},
			{IDC_TM_FRLS_FLAG_I3, IDC_TM_FRLS_VALUE_I3},
			{IDC_TM_FRLS_FLAG_I4, IDC_TM_FRLS_VALUE_I4},
			{IDC_TM_FRLS_FLAG_I5, IDC_TM_FRLS_VALUE_I5},
			{IDC_TM_FRLS_FLAG_I6, IDC_TM_FRLS_VALUE_I6},
			{IDC_TM_FRLS_FLAG_I7, IDC_TM_FRLS_VALUE_I7},
		},
		{
			{IDC_TM_FRLS_FLAG_J1, IDC_TM_FRLS_VALUE_J1},
			{IDC_TM_FRLS_FLAG_J2, IDC_TM_FRLS_VALUE_J2},
			{IDC_TM_FRLS_FLAG_J3, IDC_TM_FRLS_VALUE_J3},
			{IDC_TM_FRLS_FLAG_J4, IDC_TM_FRLS_VALUE_J4},
			{IDC_TM_FRLS_FLAG_J5, IDC_TM_FRLS_VALUE_J5},
			{IDC_TM_FRLS_FLAG_J6, IDC_TM_FRLS_VALUE_J6},
			{IDC_TM_FRLS_FLAG_J7, IDC_TM_FRLS_VALUE_J7},
		},
	};

	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < FRLS_SIZE; i++)
		{
	    DDX_Control(pDX, nID[k][i][0], m_Flag[k][i]);
		  DDX_Control(pDX, nID[k][i][1], m_Value[k][i]);
		}
	}
}

void CBndrFrlsDlg::Execute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize() - 1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}

	if (aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}
	T_FRLS_D data;
	data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aTypeCtrl, data.nType);
	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < FRLS_SIZE; i++)
		{
			data.Release[k][i] = (m_Flag[k][i].GetCheck()) ? '1' : '0';
			if (m_Flag[k][i].GetCheck())
				data.Fixity[k][i] = m_Value[k][i].GetEditValue();
		}
	}

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;

	BOOL bSuccess = FALSE;
	if (m_nOption == 0) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddFrls(aSelKey, data);
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelFrls(aSelKey, BngrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BEGIN_MESSAGE_MAP(CBndrFrlsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrFrlsDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_FRLS_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I1, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_BTN_PP, OnClickTypicalBtn)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I2, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I3, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I4, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I5, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I6, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_I7, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J1, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J2, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J3, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J4, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J5, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J6, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_FLAG_J7, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_FRLS_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_FRLS_BTN_PF, OnClickTypicalBtn)
	ON_BN_CLICKED(IDC_TM_FRLS_BTN_FP, OnClickTypicalBtn)
	ON_BN_CLICKED(IDC_TM_FRLS_BTN_FF, OnClickTypicalBtn)
	ON_BN_CLICKED(IDC_TM_RADIO_RELATIVE, OnTmChangeType)
	ON_BN_CLICKED(IDC_TM_RADIO_VALUE, OnTmChangeType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrFrlsDlg message handlers
BOOL CBndrFrlsDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	 
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_nOption = 0;
	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < FRLS_SIZE; i++)
		{
			m_Value[k][i].SetUnitType(D_UNITSYS_NONE);
			m_Flag[k][i].SetCheck((i==4||i==5)?TRUE:FALSE);
		}
	}
	
	SetControl_7thDOF();

	CDlgUtil::CtrlRadioSetCheck(this, m_aTypeCtrl, 0);
	OnTmChangeType(); // Unit ¼³Á¤
	OnToggleFlag();
	UpdateData(FALSE);	

	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_frls 1.svg"));
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void  CBndrFrlsDlg::SetControl_7thDOF()
{
	BOOL bHide = FALSE;
	if (!CSectDB::IsEnable7thDOF())
		bHide = TRUE;
	
	if(bHide)
	{
		GetDlgItem(IDC_TM_FRLS_M_STC)->SetWindowText(_T("Mx, My, Mz  "));
		GetDlgItem(IDC_STATIC_W)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_FRLS_FLAG_I7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_FRLS_VALUE_I7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_FRLS_FLAG_J7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_FRLS_VALUE_J7)->ShowWindow(SW_HIDE);

		CWnd  *pWnd = GetDlgItem(IDC_STATIC_W);
		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd); 
		this->ScreenToClient(&rcWnd);
		int nDy = rcWnd.Height()*1.5*(-1.0);
		
		MoveControls(nDy);
		ResizeControls(nDy);
	}
}

void CBndrFrlsDlg::MoveControls(int nDy)
{
	CRect Rect[10];
	CWnd *pWnd[10];
	pWnd[0] = GetDlgItem(IDC_TM_FRLS_F_STC);
	pWnd[1] = GetDlgItem(IDC_TM_FRLS_M_STC);
	pWnd[2] = GetDlgItem(IDC_TM_TEXT_FXYZ_UNIT);
	pWnd[3] = GetDlgItem(IDC_TM_TEXT_MXYZ_UNIT);
	pWnd[4] = GetDlgItem(IDC_TM_FRLS_BTN_PP);
	pWnd[5] = GetDlgItem(IDC_TM_FRLS_BTN_PF);
	pWnd[6] = GetDlgItem(IDC_TM_FRLS_BTN_FP);
	pWnd[7] = GetDlgItem(IDC_TM_FRLS_BTN_FF);
	pWnd[8] = GetDlgItem(IDC_TM_EXECUTE);
	pWnd[9] = GetDlgItem(IDC_TM_CLOSE);
	
	for(int i=0; i<10; i++)
	{
		pWnd[i]->GetWindowRect(Rect[i]); 
		Rect[i].top += nDy ; 
		ScreenToClient(Rect[i]);
		pWnd[i]->SetWindowPos(NULL, Rect[i].left, Rect[i].top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CBndrFrlsDlg::ResizeControls(int nDy)
{
	CRect Rect[4];
	CWnd *pWnd[4];
	
	pWnd[0] = GetDlgItem(IDC_TM_TYPE_GRB);
	pWnd[1] = GetDlgItem(IDC_TM_FRLS_I_NODE_GRB);
	pWnd[2] = GetDlgItem(IDC_TM_FRLS_J_NODE_GRB);
	pWnd[3] = this;
	for(int i=0; i<4; i++)
	{
		pWnd[i]->GetWindowRect(Rect[i]);
		Rect[i].SetRect(Rect[i].left,Rect[i].top,Rect[i].right,Rect[i].bottom+nDy);
		ScreenToClient(Rect[i]); 
		pWnd[i]->SetWindowPos(NULL, Rect[i].left, Rect[i].top, Rect[i].Width(), Rect[i].Height(),SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CBndrFrlsDlg::OnTmExecute() 
{
	Execute();
}

void CBndrFrlsDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrFrlsDlg::OnToggleFlag() 
{
	// TODO: Add your control notification handler code here
	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < FRLS_SIZE; i++)
		{
			if(!m_Flag[k][i].GetCheck()) m_Value[k][i].SetWindowText(_T("0"));
			m_Value[k][i].EnableWindow(m_Flag[k][i].GetCheck());
		}
	}
}


void CBndrFrlsDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 1) // delete
	{
		EnableValueEdit(FALSE); // disable all value edit
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);	
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
}

void CBndrFrlsDlg::EnableValueEdit(BOOL bEnable)
{
	if (bEnable) 
	{
		EnableFlag(TRUE, TRUE);
		OnToggleFlag();
	}
	else 
	{
		EnableFlag(FALSE, FALSE);
		EnableValue(FALSE, FALSE);
	}
	CDlgUtil::CtrlEnableDisable(this, m_aTypeCtrl, bEnable);
}

void CBndrFrlsDlg::EnableFlag(BOOL bEnableI, BOOL bEnableJ)
{
	for (int i = 0; i < FRLS_SIZE; i++)
	{
		m_Flag[0][i].EnableWindow(bEnableI);
		m_Flag[1][i].EnableWindow(bEnableJ);
	}
}

void CBndrFrlsDlg::EnableValue(BOOL bEnableI, BOOL bEnableJ)
{
	for (int i = 0; i < FRLS_SIZE; i++)
	{
		m_Value[0][i].EnableWindow(bEnableI);
		m_Value[1][i].EnableWindow(bEnableJ);
	}
}

void CBndrFrlsDlg::OnClickTypicalBtn() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
 
	int nRelease[2];
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_FRLS_BTN_PP:
		nRelease[0] = 2; nRelease[1] = 2; break;
	case IDC_TM_FRLS_BTN_PF:
		nRelease[0] = 2; nRelease[1] = 0; break;
	case IDC_TM_FRLS_BTN_FP:
		nRelease[0] = 0; nRelease[1] = 2; break;
	case IDC_TM_FRLS_BTN_FF:
		nRelease[0] = 0; nRelease[1] = 0; break;
	}
	for (int k = 0; k < 2; k++)
	{
		int i = 0;
		for (i = 0; i < FRLS_SIZE-nRelease[k]-1; i++)
			m_Flag[k][i].SetCheck(FALSE);
		for (; i < FRLS_SIZE-1; i++)
			m_Flag[k][i].SetCheck(TRUE);

		m_Flag[k][6].SetCheck(FALSE);
	}
	OnToggleFlag();
}

BOOL CBndrFrlsDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_FRLS_K Key;
	T_FRLS_D data;
	Key.keymap = key;
	m_pDoc->m_pAttrCtrl->GetFrls(Key, data);

	CDlgUtil::CtrlRadioSetCheck(this, m_aTypeCtrl, data.nType);
	OnTmChangeType();

	for(int k=0; k < 2; k++)
	{
		for(int i=0; i < FRLS_SIZE; i++)
		{
			if(data.Release[k][i] == '1')
			{
				m_Flag[k][i].SetCheck(TRUE);
				m_Value[k][i].SetEditUnit(data.Fixity[k][i]);
			}
			else
				m_Flag[k][i].SetCheck(FALSE);
		}
	}
	OnToggleFlag();

	m_nOption = 0;

	// Groupµµ Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}
	UpdateData(FALSE);
	return TRUE;
}

void CBndrFrlsDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}

void CBndrFrlsDlg::OnTmChangeType() 
{
	// TODO: Add your control notification handler code here
	int nType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aTypeCtrl, nType);
	if (nType == 0)
	{
		m_textFxyzUnit.SetUnitType(D_UNITSYS_NONE);
		m_textMxyzUnit.SetUnitType(D_UNITSYS_NONE);
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < FRLS_SIZE; j++)
				m_Value[i][j].SetUnitType(D_UNITSYS_NONE);
	}
	else
	{
		m_textFxyzUnit.SetUnitType(CUnitCtrl::m_FRLS_UNIT.dFxyz);
		m_textMxyzUnit.SetUnitType(CUnitCtrl::m_FRLS_UNIT.dMxyz);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)
				m_Value[i][j].SetUnitType(CUnitCtrl::m_FRLS_UNIT.dFxyz);
			for (int j = 3; j < FRLS_SIZE; j++)
				m_Value[i][j].SetUnitType(CUnitCtrl::m_FRLS_UNIT.dMxyz);
		}
	}
}
