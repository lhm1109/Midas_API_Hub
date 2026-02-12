// CMThkItemValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkItemValueDlg.h"

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


CCMThkItemValueDlg::CCMThkItemValueDlg(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMThkItemValueDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bModify = FALSE;
	m_sDlgData.Initialize();

	//{{AFX_DATA_INIT(CCMThkItemValueDlg)
	m_nID = 0;
	m_dbThkBoth = 0.0;
	m_dbThkIn = 0.0;
	m_dbThkOut = 0.0;
	m_nPlaneType = 0;
	m_bOffset = FALSE;
	m_nOffset = 0;
	//}}AFX_DATA_INIT

	m_arPlaneBoth.Add(IDC_CMD_TP_INOUTPLANE_EDIT);
	m_arPlaneBoth.Add(IDC_CMD_TP_INOUTPLANE_UNIT);

	m_arPlaneEach.Add(IDC_CMD_TP_INPLANE_EDIT);
	m_arPlaneEach.Add(IDC_CMD_TP_OUTPLANE_EDIT);
	m_arPlaneEach.Add(IDC_CMD_TP_INPLANE_UNIT);
	m_arPlaneEach.Add(IDC_CMD_TP_OUTPLANE_UNIT);
}


void CCMThkItemValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkItemValueDlg)
	DDX_Control(pDX, IDC_CMD_TP_INOUTPLANE_UNIT, m_UnitBoth);
	DDX_Control(pDX, IDC_CMD_TP_INPLANE_UNIT, m_UnitIn);
	DDX_Control(pDX, IDC_CMD_TP_OUTPLANE_UNIT, m_UnitOut);
	DDX_Text(pDX, IDC_CMD_TP_VALUE_ID, m_nID);
	DDX_Text(pDX, IDC_CMD_TP_NAME_EDIT, m_strName);
	DDX_Text(pDX, IDC_CMD_TP_INOUTPLANE_EDIT, m_dbThkBoth);
	DDX_Text(pDX, IDC_CMD_TP_INPLANE_EDIT, m_dbThkIn);
	DDX_Text(pDX, IDC_CMD_TP_OUTPLANE_EDIT, m_dbThkOut);
	DDX_Radio(pDX, IDC_CMD_TP_INOUTPLANE_TITLE, m_nPlaneType);
	DDX_Check(pDX, IDC_CMD_TP_OFFSET_CHK, m_bOffset);
	DDX_Radio(pDX, IDC_CMD_TP_RATIO_RDO, m_nOffset);
	DDX_Control(pDX, IDC_CMD_TP_RATIO_EDT, m_edtRatio);
	DDX_Control(pDX, IDC_CMD_TP_VALUE_EDT, m_edtValue);
	DDX_Control(pDX, IDC_CMD_TP_VALUE_UNT, m_untValue);
	DDX_Control(pDX, IDC_WG_CMD_STATIC2, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThkItemValueDlg, CDlgChild)
	//{{AFX_MSG_MAP(CCMThkItemValueDlg)
	ON_BN_CLICKED(IDC_CMD_TP_INOUTPLANE_TITLE, OnCmdTpThickControl)
	ON_BN_CLICKED(IDC_CMD_TP_INOUTPLANE_TITLE2, OnCmdTpThickControl)
	ON_BN_CLICKED(IDC_CMD_TP_RATIO_RDO, OnCmdTpOffsetType)
	ON_BN_CLICKED(IDC_CMD_TP_VALUE_RDO, OnCmdTpOffsetType)
	ON_BN_CLICKED(IDC_CMD_TP_OFFSET_CHK, OnCmdTpOffset)
	ON_EN_CHANGE(IDC_CMD_TP_INOUTPLANE_EDIT, OnCmdAutoChangeName)
	ON_EN_CHANGE(IDC_CMD_TP_INPLANE_EDIT, OnCmdAutoChangeName)
	ON_EN_CHANGE(IDC_CMD_TP_OUTPLANE_EDIT, OnCmdAutoChangeName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemValueDlg message handlers
BOOL CCMThkItemValueDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);

	MakeUnitSystem();
#ifdef _CIVIL
	GetDlgItem(IDC_CMD_TP_NAME_EDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_TP_NAME)->ShowWindow(SW_HIDE);
#endif
	if (m_bModify)
		SetData();
	else
		InitDlgItems(TRUE);

	m_wndPicture.SetImage("SVG\\Illustration\\Dialog\\cmd_thik.svg");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkItemValueDlg::MakeUnitSystem() 
{
	m_UnitBoth.SetUnitType(CUnitCtrl::m_THIK_UNIT.Value.ThickIn);
	m_UnitIn.SetUnitType(CUnitCtrl::m_THIK_UNIT.Value.ThickIn);
	m_UnitOut.SetUnitType(CUnitCtrl::m_THIK_UNIT.Value.ThickOut);

	m_edtRatio.SetUnitType(0);
	m_edtValue.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untValue.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

BOOL CCMThkItemValueDlg::SaveData()
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

void CCMThkItemValueDlg::GetData(T_THIK_K &Key, T_THIK_D &Data)
{
	UpdateData(TRUE);
	Key = m_nID;
	Data.Value.strName = m_strName;
	Data.Value.nThickType = m_nPlaneType;
	if(m_nPlaneType)
	{
		Data.Value.ThickIn = m_dbThkIn;
		Data.Value.ThickOut = m_dbThkOut;
	}
	else
		Data.Value.ThickIn = m_dbThkBoth;

	Data.Value.bOffset = m_bOffset;
	Data.Value.nOffsetType = m_nOffset;
	Data.Value.dRatio = m_edtRatio.GetEditValue();
	Data.Value.dValue = m_edtValue.GetEditValue();
}

void CCMThkItemValueDlg::SetData()
{
	m_nID = m_sDlgData.ID;
	m_strName = m_sDlgData.strName;
	m_nPlaneType = m_sDlgData.nThickType;
	if(m_nPlaneType)
	{
		m_dbThkIn = m_sDlgData.ThickIn;
		m_dbThkOut = m_sDlgData.ThickOut;
	}
	else
	{
		m_dbThkBoth = m_sDlgData.ThickIn;
	}
	m_bOffset = m_sDlgData.bOffset;
	m_nOffset = m_sDlgData.nOffsetType;
	m_edtRatio.SetEditUnit(m_sDlgData.dRatio);
	m_edtValue.SetEditUnit(m_sDlgData.dValue);

	UpdateData(FALSE);

	CDlgUtil::CtrlEnableDisable(this, m_arPlaneBoth, m_nPlaneType == 0);
	CDlgUtil::CtrlEnableDisable(this, m_arPlaneEach, m_nPlaneType == 1);
	OnCmdTpOffset();

	m_strName = m_sDlgData.strName;
}

void CCMThkItemValueDlg::SetModifyData(T_THIK_K Key, T_THIK_D &Data)
{
	m_Key = Key;
	m_Data = Data;
	m_bModify = TRUE;
}

void CCMThkItemValueDlg::InitDlgItems(BOOL bOnInitDialog)
{
	m_dbThkBoth = 0.0;
	m_dbThkIn = 0.0;
	m_dbThkOut = 0.0;

	if (bOnInitDialog) // OnInitDialog에서 호출(최초 실행일 경우)
	{
		m_nID = m_pDoc->m_pAttrCtrl->GetStartNumThik();
	}
	else // Apply에서 실행될 경우
	{
		m_nID = m_pDoc->m_pAttrCtrl->GetNextNumThik(m_nID); // type은 다시 설정 않는다.
	}

	m_bOffset = FALSE;
	m_nOffset = 0;

	UpdateData(FALSE);

	OnCmdTpThickControl();
	OnCmdTpOffset();
}

void CCMThkItemValueDlg::OnCmdTpThickControl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_arPlaneBoth, m_nPlaneType == 0);
	CDlgUtil::CtrlEnableDisable(this, m_arPlaneEach, m_nPlaneType == 1);

	if(m_nPlaneType)
		((CButton*)GetDlgItem(IDC_CMD_TP_INPLANE_EDIT))->SetFocus();
	else
		((CButton*)GetDlgItem(IDC_CMD_TP_INOUTPLANE_EDIT))->SetFocus();

#ifdef _MGEN_CH
	m_strName = _T("");
#endif

	UpdateData(FALSE);
}

BOOL CCMThkItemValueDlg::OnApplyPublic()
{
	UpdateData();

	m_sDlgData.ID = m_nID;
	m_sDlgData.strName = m_strName;
#ifdef _MGEN
	if(m_strName.IsEmpty())
	{
		MessageBox(_LS(IDS_DB_THIK_ADD_NAME), _T("midas Gen"), MB_ICONWARNING);
		return FALSE;
	}
#endif // _MGEN
	
	m_sDlgData.nThickType = m_nPlaneType;
	if(m_nPlaneType)
	{
		m_sDlgData.ThickIn = m_dbThkIn;
		m_sDlgData.ThickOut = m_dbThkOut;
	}
	else
		m_sDlgData.ThickIn = m_dbThkBoth;

	m_sDlgData.bOffset = m_bOffset;
	m_sDlgData.nOffsetType = m_nOffset;
	m_sDlgData.dRatio = m_edtRatio.GetEditValue();
	m_sDlgData.dValue = m_edtValue.GetEditValue();

	UpdateData(FALSE);
	return TRUE;
}

void CCMThkItemValueDlg::OnCmdTpOffset()
{
	UpdateData();
	GetDlgItem(IDC_CMD_TP_RATIO_RDO)->EnableWindow(m_bOffset);
	GetDlgItem(IDC_CMD_TP_RATIO_STC)->EnableWindow(m_bOffset && m_nOffset==0);
	GetDlgItem(IDC_CMD_TP_RATIO_EDT)->EnableWindow(m_bOffset && m_nOffset==0);

	GetDlgItem(IDC_CMD_TP_VALUE_RDO)->EnableWindow(m_bOffset);
	GetDlgItem(IDC_CMD_TP_VALUE_STC)->EnableWindow(m_bOffset && m_nOffset==1);
	GetDlgItem(IDC_CMD_TP_VALUE_EDT)->EnableWindow(m_bOffset && m_nOffset==1);
	GetDlgItem(IDC_CMD_TP_VALUE_UNT)->EnableWindow(m_bOffset && m_nOffset==1);
}

void CCMThkItemValueDlg::OnCmdTpOffsetType()
{
	UpdateData();
	GetDlgItem(IDC_CMD_TP_RATIO_STC)->EnableWindow(m_nOffset==0);
	GetDlgItem(IDC_CMD_TP_RATIO_EDT)->EnableWindow(m_nOffset==0);
	GetDlgItem(IDC_CMD_TP_VALUE_STC)->EnableWindow(m_nOffset==1);
	GetDlgItem(IDC_CMD_TP_VALUE_EDT)->EnableWindow(m_nOffset==1);
	GetDlgItem(IDC_CMD_TP_VALUE_UNT)->EnableWindow(m_nOffset==1);
}

void  CCMThkItemValueDlg::OnCmdAutoChangeName()
{
#ifdef _MGEN_CH
	CString strThk = _T("");
	GetDlgItem(IDC_CMD_TP_INOUTPLANE_EDIT)->GetWindowText(strThk);
	CString strInThk = _T("");
	CString strOutThk = _T("");
	GetDlgItem(IDC_CMD_TP_INPLANE_EDIT)->GetWindowText(strInThk);	
	GetDlgItem(IDC_CMD_TP_OUTPLANE_EDIT)->GetWindowText(strOutThk);

	if (IsDlgButtonChecked(IDC_CMD_TP_INOUTPLANE_TITLE))
		m_strName = strThk;
	else
		m_strName = strInThk + _T("(") + strOutThk + _T(")");
	GetDlgItem(IDC_CMD_TP_NAME_EDIT)->SetWindowText(m_strName);
#endif

}
