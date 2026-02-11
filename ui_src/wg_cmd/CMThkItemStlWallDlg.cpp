// CMThkItemValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkItemStlWallDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMThkItemValueDlg dialog


CCMThkItemStlWallDlg::CCMThkItemStlWallDlg(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMThkItemStlWallDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bModify = FALSE;
	m_sDlgData.Initialize();

	//{{AFX_DATA_INIT(CCMThkItemStlWallDlg)
	m_nID = 0;
	m_dbThkCon = 0.0;
	m_dbThkSteel = 0.0;
	//}}AFX_DATA_INIT
}


void CCMThkItemStlWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkItemStlWallDlg)
	DDX_Text(pDX, IDC_CMD_TP_STLWALL_ID, m_nID);
	DDX_Control(pDX, IDC_CMD_TP_CON_UNIT, m_UnitCon);
	DDX_Control(pDX, IDC_CMD_TP_STEEL_UNIT, m_UnitSteel);
	DDX_Text(pDX, IDC_CMD_TP_CON_EDIT, m_dbThkCon);
	DDX_Text(pDX, IDC_CMD_TP_STEEL_EDIT, m_dbThkSteel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThkItemStlWallDlg, CDlgChild)
	//{{AFX_MSG_MAP(CCMThkItemStlWallDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStlWallDlg message handlers
BOOL CCMThkItemStlWallDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	MakeUnitSystem();

	if (m_bModify)
		SetData();
	else
		InitDlgItems(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkItemStlWallDlg::MakeUnitSystem() 
{
	m_UnitCon.SetUnitType(CUnitCtrl::m_THIK_UNIT.StlWall.ThickCon);
	m_UnitSteel.SetUnitType(CUnitCtrl::m_THIK_UNIT.StlWall.ThickSteel);
}

BOOL CCMThkItemStlWallDlg::SaveData()
{
	T_THIK_K Key;
	T_THIK_D Data;
	
	Data.Initialize();

	GetData(Key, Data);
	if (m_bModify)
	{
		T_THIK_K KeyOld = m_Key;
		m_Key = Key;
		return m_pDoc->m_pDataCtrl->ModifyThik(KeyOld, Key, Data);
	}
	else return m_pDoc->m_pDataCtrl->AddThik(Key, Data);
}

void CCMThkItemStlWallDlg::GetData(T_THIK_K &Key, T_THIK_D &Data)
{
	UpdateData(TRUE);
	Key = m_nID;
	Data.StlWall.ThickCon = m_dbThkCon;
	Data.StlWall.ThickSteel = m_dbThkSteel;
}

void CCMThkItemStlWallDlg::SetData()
{
	m_nID = m_sDlgData.ID;
	m_dbThkCon = m_sDlgData.ThickCon;
	m_dbThkSteel = m_sDlgData.ThickSteel;
	UpdateData(FALSE);
}

void CCMThkItemStlWallDlg::SetModifyData(T_THIK_K Key, T_THIK_D &Data)
{
	m_Key = Key;
	m_Data = Data;
	m_bModify = TRUE;
}

void CCMThkItemStlWallDlg::InitDlgItems(BOOL bOnInitDialog)
{
	m_dbThkCon = 0.0;
	m_dbThkSteel = 0.0;

	if (bOnInitDialog) // OnInitDialog에서 호출(최초 실행일 경우)
	{
		m_nID = m_pDoc->m_pAttrCtrl->GetStartNumThik();
	}
	else // Apply에서 실행될 경우
	{
		m_nID = m_pDoc->m_pAttrCtrl->GetNextNumThik(m_nID); // type은 다시 설정 않는다.
	}
	UpdateData(FALSE);
}

BOOL CCMThkItemStlWallDlg::OnApplyPublic()
{
	UpdateData();
	m_sDlgData.ID = m_nID;
	m_sDlgData.ThickCon = m_dbThkCon;
	m_sDlgData.ThickSteel = m_dbThkSteel;
	UpdateData(FALSE);
	return TRUE;
}
