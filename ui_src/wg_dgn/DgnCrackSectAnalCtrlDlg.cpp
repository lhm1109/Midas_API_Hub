// DgnCrackSectAnalCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCrackSectAnalCtrlDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCrackSectAnalCtrlDlg dialog


CDgnCrackSectAnalCtrlDlg::CDgnCrackSectAnalCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCrackSectAnalCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCrackSectAnalCtrlDlg)
	//}}AFX_DATA_INIT
}


void CDgnCrackSectAnalCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCrackSectAnalCtrlDlg)
	DDX_Control(pDX, IDC_DGN_CRACK_SECT_ANAL_CTRL_EXTOL_UNIT, m_unitExTolerance);
	DDX_Control(pDX, IDC_DGN_CRACK_SECT_ANAL_CTRL_EXTOL_EDIT, m_edtExTolerance);
	DDX_Control(pDX, IDC_DGN_CRACK_SECT_ANAL_CTRL_EXLCASE_EDIT, m_edtExLoadCase);	
	DDX_Control(pDX, IDC_DGN_CRACK_SECT_ANAL_CTRL_CREEP_CHK, m_chkCreep);
	DDX_Control(pDX, IDC_DGN_CRACK_SECT_ANAL_CTRL_CREEP_EDT, m_edtCreep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCrackSectAnalCtrlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnCrackSectAnalCtrlDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_CRACK_SECT_ANAL_CTRL_EXLCASE_SPIN, OnDeltaposExlcaseSpin)
	ON_BN_CLICKED(IDC_DGN_CRACK_SECT_ANAL_CTRL_CREEP_CHK, OnClickCreep2Chk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCrackSectAnalCtrlDlg message handlers

BOOL CDgnCrackSectAnalCtrlDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_edtExLoadCase.SetUnitType(0);
	m_edtExTolerance.SetUnitType(CUnitCtrl::m_CCTL_UNIT.dblTolerance);
	m_unitExTolerance.SetUnitType(CUnitCtrl::m_CCTL_UNIT.dblTolerance);
	m_edtCreep.SetUnitType(D_UNITSYS_NONE);
	
	InitDialogData();  

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCrackSectAnalCtrlDlg::InitDialogData()
{  
	T_CCTL_D data;
	if (!m_pDoc->m_pAttrCtrl->GetCctl(data)) 
		data.Initialize();
	
	m_edtExLoadCase.SetEditUnit(data.nIterPerLoad);
	m_edtExTolerance.SetEditUnit(data.dblTolerance);
	
	m_edtCreep.SetEditUnit(data.dCreepCoeff);
	if (data.bCreep) m_chkCreep.SetCheck(BST_CHECKED);
	else m_chkCreep.SetCheck(BST_UNCHECKED);

	const CString& strConCode = CDBLib::GetConCodeName();
	
	BOOL bEnableCreep = FALSE;
	if (strConCode == CONCODE_EC2_04 || strConCode == CONCODE_SP_63_13330_2018)
	{ 
		bEnableCreep = TRUE;
	}

	CArray<UINT, UINT> aCreep2Ctrls;
	aCreep2Ctrls.Add(IDC_WG_DGN_STATIC4);
	aCreep2Ctrls.Add(IDC_DGN_CRACK_SECT_ANAL_CTRL_CREEP_CHK);
	aCreep2Ctrls.Add(IDC_DGN_CRACK_SECT_ANAL_CTRL_CREEP_EDT);
	aCreep2Ctrls.Add(IDC_DGN_CRACK_SECT_ANAL_CTRL_CREEP_STC);
	CDlgUtil::CtrlShowHide(this, aCreep2Ctrls, bEnableCreep);
	if (bEnableCreep) m_edtCreep.EnableWindow(data.bCreep);
}

void CDgnCrackSectAnalCtrlDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	T_CCTL_D data; data.Initialize();
	data.nIterPerLoad = m_edtExLoadCase.GetEditValue();
	data.dblTolerance = m_edtExTolerance.GetEditValue();

	data.dCreepCoeff = m_edtCreep.GetEditValue();
	if (m_chkCreep.GetCheck() == BST_CHECKED) data.bCreep = TRUE;
	else data.bCreep = FALSE;

	if (!m_pDoc->m_pDataCtrl->AddCctl(data)) return;

	CDialogMove::OnOK();
}

void CDgnCrackSectAnalCtrlDlg::OnClickCreep2Chk()
{
	BOOL bCreepCoeff = FALSE;
	if (m_chkCreep.GetCheck() == BST_CHECKED) bCreepCoeff = TRUE;
	m_edtCreep.EnableWindow(bCreepCoeff);
}

void CDgnCrackSectAnalCtrlDlg::OnDeltaposExlcaseSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int nVal = m_edtExLoadCase.GetEditValue();

	if(pNMUpDown->iDelta < 0)
	{
		++nVal;
		m_edtExLoadCase.SetEditUnit(nVal);
	}
	else
	{
		if(nVal >= 2)
		{
			--nVal;
			m_edtExLoadCase.SetEditUnit(nVal);
		}
		else
			m_edtExLoadCase.SetEditUnit(1);
	} 
	
	*pResult = 0;
}
