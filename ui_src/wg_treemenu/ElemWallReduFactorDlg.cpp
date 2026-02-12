// ElemWallReduFactorDlg.cpp : implementation file
//
// 2003. 07. 29    by TAE
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemWallReduFactorDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemWallReduFactorDlg dialog


CElemWallReduFactorDlg::CElemWallReduFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemWallReduFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemWallReduFactorDlg)
	m_nAddOrDel = 0;
	//}}AFX_DATA_INIT

	m_WallPlate.RemoveAll();
	m_WallPlate.Add(IDC_TM_STATIC_GR5);
	m_WallPlate.Add(IDC_TM_SCALE_SHEAR_TXT);
	m_WallPlate.Add(IDC_TM_SCALE_BEND_TXT);
	m_WallPlate.Add(IDC_TM_REDU_SHEAR_EDIT);
	m_WallPlate.Add(IDC_TM_REDU_BENDING_EDIT);

	m_WallCRB.RemoveAll();
	m_WallCRB.Add(IDC_TM_STATIC_GR6);
	m_WallCRB.Add(IDC_TM_SCALE_AXIAL_CRB_TXT);
	m_WallCRB.Add(IDC_TM_REDU_AXIAL_CRB_EDIT);
	m_WallCRB.Add(IDC_TM_SCALE_SHEAR_CRB_TXT);
	m_WallCRB.Add(IDC_TM_REDU_SHEAR_CRB_EDIT);
	m_WallCRB.Add(IDC_TM_SCALE_BEND_CRB_TXT);
	m_WallCRB.Add(IDC_TM_REDU_BENDING_CRB_EDIT);

	m_WallType2.RemoveAll();
	m_WallType2.Add(IDC_TM_STATIC_GR4);
	m_WallType2.Add(IDC_TM_WALL_PLATE_RADIO);
	m_WallType2.Add(IDC_TM_WALL_CRB_RADIO);

	m_WallType.RemoveAll();
	m_WallType.Add(IDC_TM_WALL_PLATE_RADIO);
	m_WallType.Add(IDC_TM_WALL_CRB_RADIO);

	m_OutofPlane.RemoveAll();
	m_OutofPlane.Add(IDC_TM_STATIC_GR7);
	m_OutofPlane.Add(IDC_TM_OUT_SCALE_TORSION_CRB_TXT);
	m_OutofPlane.Add(IDC_TM_REDU_OUT_TORSION_CRB_EDIT);
	m_OutofPlane.Add(IDC_TM_OUT_SCALE_SHEAR_CRB_TXT);
	m_OutofPlane.Add(IDC_TM_REDU_OUT_SHEAR_CRB_EDIT);
	m_OutofPlane.Add(IDC_TM_OUT_SCALE_BEND_CRB_TXT);
	m_OutofPlane.Add(IDC_TM_REDU_OUT_BENDING_CRB_EDIT);

}

void CElemWallReduFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemWallReduFactorDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_ADD_REP_RADIO, m_nAddOrDel);

	DDX_Control(pDX, IDC_TM_REDU_SHEAR_EDIT, m_wndShear);
	DDX_Control(pDX, IDC_TM_REDU_BENDING_EDIT, m_wndBending);
	DDX_Control(pDX, IDC_TM_REDU_AXIAL_CRB_EDIT, m_wndCRBAxial);
	DDX_Control(pDX, IDC_TM_REDU_SHEAR_CRB_EDIT, m_wndCRBShear);
	DDX_Control(pDX, IDC_TM_REDU_BENDING_CRB_EDIT, m_wndCRBBending);
	DDX_Control(pDX, IDC_TM_REDU_OUT_TORSION_CRB_EDIT, m_wndOutTorsionBending);
	DDX_Control(pDX, IDC_TM_REDU_OUT_SHEAR_CRB_EDIT, m_wndOutShear);
	DDX_Control(pDX, IDC_TM_REDU_OUT_BENDING_CRB_EDIT, m_wndOutBending);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemWallReduFactorDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemWallReduFactorDlg)
	ON_BN_CLICKED(IDC_TM_ADD_REP_RADIO, OnTmOption)
	ON_BN_CLICKED(IDC_TM_DELETE_RADIO, OnTmOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)

	ON_BN_CLICKED(IDC_TM_WALL_PLATE_RADIO, OnTmWallType)
	ON_BN_CLICKED(IDC_TM_WALL_CRB_RADIO  , OnTmWallType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemWallReduFactorDlg message handlers

BOOL CElemWallReduFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	AlignControls();

	CDlgUtil::CtrlRadioSetCheck(this, m_WallType, 0);
	CtrlShowHide(m_WallCRB, FALSE);
	CtrlEnableDisable(m_OutofPlane, FALSE);

	T_WSSF_D data;
	data.Initialize();

	CString csFactor;
	csFactor.Format(_T("%g"), data.dShear);
	m_wndShear.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dBending);
	m_wndBending.SetWindowText(csFactor);

	csFactor.Format(_T("%g"), data.dShear);      m_wndCRBShear.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dBending);    m_wndCRBBending.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dAxial);      m_wndCRBAxial.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dOutTorsion); m_wndOutTorsionBending.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dOutShear);   m_wndOutShear.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dOutBending); m_wndOutBending.SetWindowText(csFactor);

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemWallReduFactorDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	CWnd* pWndLast=NULL;

	// Reference ctrl 좌표 구하기
	GetDlgItem(IDC_TM_STATIC_GR5)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_STATIC_GR6)->GetWindowRect(rToMove);

	nDistY = rRef.top - rToMove.top;

	CtrlMoveDistY(m_WallCRB, nDistY);
	CtrlMoveDistY(m_OutofPlane, nDistY);

	pWndLast = GetDlgItem(IDC_TM_STATIC_GR7);

	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+1;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}
void CElemWallReduFactorDlg::OnTmOption()
{
	UpdateData();
	GetDlgItem(IDC_TM_REDU_SHEAR_EDIT)->EnableWindow(m_nAddOrDel==0);  
	GetDlgItem(IDC_TM_REDU_BENDING_EDIT)->EnableWindow(m_nAddOrDel==0);
	this->m_wndGroupCombo.ShowHideAll(m_nAddOrDel==1);
}

void CElemWallReduFactorDlg::Execute()
{
	if (!UpdateData(TRUE)) return;
	
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	ASSERT(pDoc);

	// Wall List 가져오기 - Wall 요소에만 적용 
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	I_GENModelBase::GetCurMySelfST()->GetSelectedElemKeyList(aElemList);

	T_ELEM_D ElemD;
	CArray<T_ELEM_K, T_ELEM_K> aWallList; 
	CArray<T_ELEM_K, T_ELEM_K> aCRBWallList; 
	for(int i = 0; i< aElemList.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetElem(aElemList[i], ElemD)) continue;    
		if (pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) 
		{
			if     (ElemD.nWallType==0) aWallList.Add(aElemList[i]);
			else if(ElemD.nWallType==1) aCRBWallList.Add(aElemList[i]);
			else { ASSERT(0); }
		}
	}
	if (aWallList.GetSize() == 0 && aCRBWallList.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Wall));
		return;
	}

	// Data 가져오기 
	T_WSSF_D data; data.Initialize();
	m_wndGroupCombo.GetSelectedBngr(data.GroupKey);
	CString csFactor;
	m_wndShear.GetWindowText(csFactor);
	data.dShear = _tstof(csFactor);
	m_wndBending.GetWindowText(csFactor);
	data.dBending = _tstof(csFactor);

	T_WSSF_D dataCRB; dataCRB.Initialize();
	m_wndGroupCombo.GetSelectedBngr(dataCRB.GroupKey);
	m_wndCRBShear.GetWindowText(csFactor);
	dataCRB.dShear = _tstof(csFactor);
	m_wndCRBBending.GetWindowText(csFactor);
	dataCRB.dBending = _tstof(csFactor);
	m_wndCRBAxial.GetWindowText(csFactor);
	dataCRB.dAxial= _tstof(csFactor);

	m_wndOutTorsionBending.GetWindowText(csFactor);
	dataCRB.dOutTorsion = _tstof(csFactor);
	m_wndOutShear.GetWindowText(csFactor);
	dataCRB.dOutShear = _tstof(csFactor);
	m_wndOutBending.GetWindowText(csFactor);
	dataCRB.dOutBending= _tstof(csFactor);

	// DB
	int nWallType;
	CDlgUtil::CtrlRadioGetCheck(this, m_WallType, nWallType);

	BOOL bCRBWall = FALSE;
	if(nWallType==1) bCRBWall = TRUE;

	BOOL bOK = FALSE;
	if (m_nAddOrDel == 0)   // Add/Replace
	{
		if(bCRBWall != TRUE)
		{
			if(aWallList.GetSize() > 0)    
			{
				bOK = pDoc->m_pDataCtrl->AddWssf(aWallList, data);
			}
			else
			{
				GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Wall_PLATE));
				return;
			}
		}    
		else
		{
			if(aCRBWallList.GetSize() > 0) 
			{
				bOK = pDoc->m_pDataCtrl->AddWssf(aCRBWallList, dataCRB);
			}
			else
			{
				GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Wall_CRB));
				return;
			}
		}    
	}
	else if (m_nAddOrDel == 1)    // Delete 
	{
		if(bCRBWall != TRUE)
		{
			if(aWallList.GetSize() > 0)    
			{
				bOK = pDoc->m_pDataCtrl->DelWssf(aWallList, data.GroupKey);
			}
			else
			{
				GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Wall_PLATE));
				return;
			}
		}
		else
		{
			if(aCRBWallList.GetSize() > 0) 
			{
				bOK = pDoc->m_pDataCtrl->DelWssf(aCRBWallList, data.GroupKey);
			}
			else
			{
				GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Wall_CRB));
				return;
			}
		}    
	}
	else ASSERT(0);

	if (bOK) 
		pDoc->m_pViewCtrl->UnselectAll(NULL);  
}

// For WorkTree
BOOL CElemWallReduFactorDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	T_WSSF_K Key;
	T_WSSF_D data;
	Key.keymap = key;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetWssf(Key, data);

	m_wndGroupCombo.ChangeSelect(data.GroupKey);
	CString csFactor;
	csFactor.Format(_T("%g"), data.dShear);
	m_wndShear.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dBending);
	m_wndBending.SetWindowText(csFactor);

	csFactor.Format(_T("%g"), data.dShear);      m_wndCRBShear.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dBending);    m_wndCRBBending.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dAxial);      m_wndCRBAxial.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dOutTorsion); m_wndOutTorsionBending.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dOutShear);   m_wndOutShear.SetWindowText(csFactor);
	csFactor.Format(_T("%g"), data.dOutBending); m_wndOutBending.SetWindowText(csFactor);

	m_nAddOrDel = 0;
	UpdateData(FALSE);

	return TRUE;
}

void CElemWallReduFactorDlg::OnTmDefineGroupButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_DEF_BNDR_GROUP,0));	
}

void CElemWallReduFactorDlg::OnTmWallType()
{
	int nWallType;
	CDlgUtil::CtrlRadioGetCheck(this, m_WallType, nWallType);

	BOOL bCRBWall = FALSE;
	if(nWallType==1) bCRBWall = TRUE;
	
	CtrlShowHide(m_WallCRB, bCRBWall);
	CtrlShowHide(m_WallPlate, !bCRBWall);
	CtrlEnableDisable(m_OutofPlane, bCRBWall);
}