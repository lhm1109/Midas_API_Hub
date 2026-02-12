// CMThkItemStiffUser.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkItemStiffUser.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "CMThkItemStiffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCMThkItemStiffDlg*	CCMThkItemStiffUser::m_pParent;
/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffUser dialog


CCMThkItemStiffUser::CCMThkItemStiffUser(int nType, CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMThkItemStiffUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThkItemStiffUser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aAllCtrl.RemoveAll();

	m_aCtrlDlgItem2.Add(IDC_CMD_TP_ID_TITLE7);
	m_aCtrlDlgItem2.Add(IDC_CMD_TP_SECT_NAME);

	m_aCtrlDlgItem.Add(IDC_CMD_TP_ID_VALUE1);
	m_aCtrlDlgItem.Add(IDC_CMD_TP_ID_VALUE2);
	m_aCtrlDlgItem.Add(IDC_CMD_TP_ID_VALUE3);
	m_aCtrlDlgItem.Add(IDC_CMD_TP_ID_VALUE4);
	m_aCtrlDlgItem.Add(IDC_CMD_TP_ID_VALUE5);
	m_aCtrlDlgItem.Add(IDC_CMD_TP_ID_VALUE6);

	m_aAllCtrl.Add(IDC_CMD_TP_STIFFUSER_TYPE);
	m_aAllCtrl.Add(IDC_CMD_TP_DIST_RIBS);
	m_aAllCtrl.Add(IDC_CMD_TP_DIST_RIBS_UNIT);
	m_aAllCtrl.Add(IDC_CMD_TP_SECT_NAME);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE1);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE2);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE3);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE4);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE5);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE6);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE7);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_TITLE8);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_VALUE1);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_VALUE2);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_VALUE3);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_VALUE4);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_VALUE5);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_VALUE6);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_UNIT1);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_UNIT2);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_UNIT3);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_UNIT4);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_UNIT5);
	m_aAllCtrl.Add(IDC_CMD_TP_ID_UNIT6);

	m_aCtrlDisableInUser.Add(IDC_CMD_TP_SECT_NAME);
	m_aCtrlDisableInUser.Add(IDC_CMD_TP_ID_TITLE7);

	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_TITLE1);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_TITLE2);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_TITLE3);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_TITLE4);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_TITLE5);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_TITLE6);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_VALUE1);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_VALUE2);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_VALUE3);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_VALUE4);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_VALUE5);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_VALUE6);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_UNIT1);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_UNIT2);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_UNIT3);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_UNIT4);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_UNIT5);
	m_aCtrlDisableInDB.Add(IDC_CMD_TP_ID_UNIT6);

	m_pParent = (CCMThkItemStiffDlg*)pParent;
	m_nIdentity = nType;
	m_pImageList = 0;
}

CCMThkItemStiffUser::~CCMThkItemStiffUser()
{
	if (m_pImageList != 0) delete m_pImageList;
}

void CCMThkItemStiffUser::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkItemStiffUser)
	DDX_Control(pDX, IDC_CMD_TP_EXIST_RIBS, m_ExistRib);
	DDX_Control(pDX, IDC_CMD_TP_DIST_RIBS_UNIT, m_distUnit);
	DDX_Control(pDX, IDC_CMD_TP_SECT_NAME, m_sectName);
	DDX_Control(pDX, IDC_CMD_TP_DIST_RIBS, m_dist);
	DDX_Control(pDX, IDC_CMD_TP_STIFFUSER_TYPE, m_cboType);
	//}}AFX_DATA_MAP
	UINT aFirstID[][6] = {
		{ IDC_CMD_TP_ID_TITLE1, IDC_CMD_TP_ID_TITLE2, IDC_CMD_TP_ID_TITLE3,
			IDC_CMD_TP_ID_TITLE4, IDC_CMD_TP_ID_TITLE5, IDC_CMD_TP_ID_TITLE6 },
		{ IDC_CMD_TP_ID_VALUE1, IDC_CMD_TP_ID_VALUE2, IDC_CMD_TP_ID_VALUE3,
			IDC_CMD_TP_ID_VALUE4, IDC_CMD_TP_ID_VALUE5, IDC_CMD_TP_ID_VALUE6 },
		{ IDC_CMD_TP_ID_UNIT1,  IDC_CMD_TP_ID_UNIT2,  IDC_CMD_TP_ID_UNIT3,
			IDC_CMD_TP_ID_UNIT4,  IDC_CMD_TP_ID_UNIT5,  IDC_CMD_TP_ID_UNIT6 },
	};
	for (int i = 0; i < 6; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
}


BEGIN_MESSAGE_MAP(CCMThkItemStiffUser, CDlgChild)
	//{{AFX_MSG_MAP(CCMThkItemStiffUser)
	ON_CBN_SELCHANGE(IDC_CMD_TP_STIFFUSER_TYPE, OnSelchangeCmdTpIdType)
	ON_CBN_SELCHANGE(IDC_CMD_TP_SECT_NAME, OnSelchangeCmdTpSectName)
	ON_EN_CHANGE(IDC_CMD_TP_ID_VALUE1, OnChangeSectSize)
	ON_EN_CHANGE(IDC_CMD_TP_ID_VALUE2, OnChangeSectSize)
	ON_EN_CHANGE(IDC_CMD_TP_ID_VALUE3, OnChangeSectSize)
	ON_EN_CHANGE(IDC_CMD_TP_ID_VALUE4, OnChangeSectSize)
	ON_EN_CHANGE(IDC_CMD_TP_ID_VALUE5, OnChangeSectSize)
	ON_EN_CHANGE(IDC_CMD_TP_ID_VALUE6, OnChangeSectSize)
	ON_BN_CLICKED(IDC_CMD_TP_EXIST_RIBS, OnCmdTpExistRibs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffUser message handlers


/////////////////////////////////////////////////////////////////////////////
// Initialize Data
BOOL CCMThkItemStiffUser::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDlgChild::OnInitDialog();

	m_distUnit.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Sect.Dist);
	
	// TODO: Add extra initialization here
	InitControls();
	InitialDBData();

	m_sectName.SetActionWhenKillFocus(2, -1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkItemStiffUser::InitialDBData()
{
	if(m_nIdentity == 0) // YZ Section
		YZInitialDBData();
	else // XZ Section
		XZInitialDBData();

	SetSectSizeData();
}

void CCMThkItemStiffUser::YZInitialDBData()
{
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_pParent->m_sDlgData.YZSect_Shape);
	m_cboType.SetCurSel(nShapeIndex);

//	if(m_pParent->m_StiffType == 2)
		m_dist.SetEditUnit(m_pParent->m_sDlgData.YZSect_Dist);

	for(int i=0; i < 10; i++)
		m_Size[i] = m_pParent->m_sDlgData.YZSect_Size[i];

	CString db = m_pParent->m_sDlgData.DBName;
	CString shape = m_pParent->m_sDlgData.YZSect_Shape;
	SetSectNameCombo(db, shape);
	m_sectName.SetWindowText(m_pParent->m_sDlgData.YZSect_SName);
	UpdateData(FALSE);
}

void CCMThkItemStiffUser::XZInitialDBData()
{
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_pParent->m_sDlgData.XZSect_Shape);
	m_cboType.SetCurSel(nShapeIndex);

//	if(m_pParent->m_StiffType == 2)
		m_dist.SetEditUnit(m_pParent->m_sDlgData.XZSect_Dist);

	for(int i=0; i < 10; i++)
		m_Size[i] = m_pParent->m_sDlgData.XZSect_Size[i];

	CString db = m_pParent->m_sDlgData.DBName;
	CString shape = m_pParent->m_sDlgData.XZSect_Shape;
	SetSectNameCombo(db, shape);
	m_sectName.SetWindowText(m_pParent->m_sDlgData.XZSect_SName);
	UpdateData(FALSE);
}

void CCMThkItemStiffUser::SetSectNameCombo(CString csDB, CString csShape)
{
	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDB, csShape, aSectNameList);

	// Section name list를 설정한다.
	m_sectName.ResetContent();
	int nSize = aSectNameList.GetSize();
	for (int i = 0; i < nSize; i++)
		m_sectName.AddString(aSectNameList.GetAt(i));
}

void CCMThkItemStiffUser::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();

	// Type == User : Enable, Type == DB : Disable
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDlgItem, m_pParent->m_StiffType == 1);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDlgItem2, m_pParent->m_StiffType == 2);

	if(m_nIdentity == 0) // YZ Section
		m_ExistRib.SetCheck(m_pParent->m_sDlgData.YZSect_Exist);
	else // XZ Section
		m_ExistRib.SetCheck(m_pParent->m_sDlgData.XZSect_Exist);

	OnCmdTpExistRibs();
}

void CCMThkItemStiffUser::SetSectionShapeCombo()
{
	std::vector<CString> aBitmapID;
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\ThkRegIcon01.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon02.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon03.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\ThkRegIcon04.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon05.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon06.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\ThkRegIcon07.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon08.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon09.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon10.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon11.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon12.svg"));

	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section), _LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe), 
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle), _LS(IDS_WG_CMD__ADDD__Solid_Round),
		_LS(IDS_WG_CMD__ADDD__Cold_Formed_Channel), _LS(IDS_WG_CMD__ADD2__U_RIB)
	};
	//if (CLocaleLib::IsEnglish()) // 영어권만(일본은 H로 씀)
	#if defined(_US) || defined(_RUS)
	{
		aItemName[2] = _LS(IDS_WG_CMD__ADDD__I_Section);
	}
	#endif

	int nCount = 12;

	for (int i = 0; i < nCount; i++)
	{
		m_cboType.AddString(aItemName[i], 0);
	}
	m_cboType.SetImageList(aBitmapID);
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));
}

int CCMThkItemStiffUser::GetCurShapeIndex()
{
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) return LB_ERR;

	return nShapeIndex;
}
/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
void CCMThkItemStiffUser::SetSectSizeData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	int nSizeNum;

	nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) 
	{
//    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
	if(nShapeIndex==1 || nShapeIndex == 2) // KJH20040125, Section에서 H, C에 r1, r2 넣은거 빼기
	{
		aSizeTitle.RemoveAt(6,2);
	}
	nSizeNum = aSizeTitle.GetSize();

	BOOL bEnable = (m_pParent->m_sDlgData.DBName == _T("")) ? TRUE : FALSE;
	CString csValue;
	int i = 0;
	for (i=0; i < nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), m_Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
//    m_wndFirstSize[i].EnableWindow(bEnable);
	}
	for (; i < 6; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);
	}
}

void CCMThkItemStiffUser::OnSelchangeCmdTpIdType() 
{
	// TODO: Add your control notification handler code here
	SetSectSizeData();

	int nShapeIndex = GetCurShapeIndex();
	CString shape;
	CString db;

	if (nShapeIndex < 0) 
	{
//    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	db = m_pParent->m_sDlgData.DBName;
	shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
 	SetSectNameCombo(db, shape);

	if(m_nIdentity == 0) // YZ Section
		m_pParent->m_sDlgData.YZSect_Shape = shape;
	else // XZ Section
		m_pParent->m_sDlgData.XZSect_Shape = shape;

	m_pParent->ChangeBitmap(shape);

	OnSelchangeCmdTpSectName();
}

void CCMThkItemStiffUser::OnSelchangeCmdTpSectName() 
{
	// TODO: Add your control notification handler code here
	T_THIK_STIFFENED_SECT sectdata;
	sectdata.Initialize();

	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) 
	{
//    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	CString db = m_pParent->m_sDlgData.DBName;
	CString shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	sectdata.Shape = shape;
	m_sectName.GetWindowText(sectdata.SName);

	CSectUtil::GetSectOfThik(db, sectdata);

	for(int i=0; i < 10; i++)
		m_Size[i] = sectdata.Size[i];

	SetSectSizeData();

	// section view ///////////////
	Dlg2Data();
	m_pParent->InvalidateSecView();
	///////////////////////////////
}

void CCMThkItemStiffUser::UpdateSectNameCobx()
{
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) 
	{
//    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}
	CString db = m_pParent->m_sDlgData.DBName;
	CString shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	SetSectNameCombo(db, shape);
}

BOOL CCMThkItemStiffUser::OnApplyPublic()
{
	if(m_nIdentity == 0) // YZ Section
	{
		int nShapeIndex = GetCurShapeIndex();
		if (nShapeIndex < 0)
		{
	//    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
			return FALSE;
		}
		CString shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

		CStringArray aSizeTitle;
		CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
		if(nShapeIndex==1 || nShapeIndex == 2) // KJH20040125, Section에서 H, C에 r1, r2 넣은거 빼기
		{
			aSizeTitle.RemoveAt(6,2);
		}
		int nSizeNum = aSizeTitle.GetSize();

		m_pParent->m_sDlgData.YZSect_Shape = shape;
		m_pParent->m_sDlgData.YZSect_Dist = m_dist.GetEditValue();
		m_sectName.GetWindowText(m_pParent->m_sDlgData.YZSect_SName);

		for(int i=0; i < nSizeNum; i++)
			m_pParent->m_sDlgData.YZSect_Size[i] = m_wndFirstSize[i].GetEditValue();
	}
	else // XZ Section
	{
		int nShapeIndex = GetCurShapeIndex();
		if (nShapeIndex < 0)
		{
	//    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
			return FALSE;
		}
		CString shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

		CStringArray aSizeTitle;
		CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
		if(nShapeIndex==1 || nShapeIndex == 2) // KJH20040125, Section에서 H, C에 r1, r2 넣은거 빼기
		{
			aSizeTitle.RemoveAt(6,2);
		}
		int nSizeNum = aSizeTitle.GetSize();

		m_pParent->m_sDlgData.XZSect_Shape = shape;
		m_pParent->m_sDlgData.XZSect_Dist = m_dist.GetEditValue();
		m_sectName.GetWindowText(m_pParent->m_sDlgData.XZSect_SName);

		for(int i=0; i < nSizeNum; i++)
			m_pParent->m_sDlgData.XZSect_Size[i] = m_wndFirstSize[i].GetEditValue();
	}

	return TRUE;
}

BOOL CCMThkItemStiffUser::GetCurCalcData(T_THIK_D& thik)
{
	OnApplyPublic();

	CString dbName;
	CStringArray aSizeTitle;
	int nShapeIndex, nSizeNum;
	T_THIK_STIFFENED_SECT yzSectData, xzSectData;
	yzSectData.Initialize(); xzSectData.Initialize();

	dbName = m_pParent->m_sDlgData.DBName;

	int i;

	// YZ Section
	if(m_pParent->m_sDlgData.YZSect_Exist)
	{
		nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_pParent->m_sDlgData.YZSect_Shape);
		CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
		if(nShapeIndex==1 || nShapeIndex == 2) // KJH20040125, Section에서 H, C에 r1, r2 넣은거 빼기
		{
			aSizeTitle.RemoveAt(6,2);
		}
		nSizeNum = aSizeTitle.GetSize();

		if(m_pParent->m_sDlgData.YZSect_Dist == 0) return FALSE;
		yzSectData.Dist = m_pParent->m_sDlgData.YZSect_Dist;
		yzSectData.Shape = m_pParent->m_sDlgData.YZSect_Shape;
		yzSectData.SName = m_pParent->m_sDlgData.YZSect_SName;
		for(i=0; i < nSizeNum; i++)
			yzSectData.Size[i] = m_pParent->m_sDlgData.YZSect_Size[i];
		if(m_pParent->m_StiffType == 1) // User
			CSectUtil::CalcThikStiffness(yzSectData);
		else if(m_pParent->m_StiffType == 2) // DB
			CSectUtil::GetSectOfThik(dbName, yzSectData);
	}


	// XZ Section
	if(m_pParent->m_sDlgData.XZSect_Exist)
	{
		nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_pParent->m_sDlgData.XZSect_Shape);
		CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
		if(nShapeIndex==1 || nShapeIndex == 2) // KJH20040125, Section에서 H, C에 r1, r2 넣은거 빼기
		{
			aSizeTitle.RemoveAt(6,2);
		}
		nSizeNum = aSizeTitle.GetSize();

		if(m_pParent->m_sDlgData.XZSect_Dist == 0) return FALSE;
		xzSectData.Dist = m_pParent->m_sDlgData.XZSect_Dist;
		xzSectData.Shape = m_pParent->m_sDlgData.XZSect_Shape;
		xzSectData.SName = m_pParent->m_sDlgData.XZSect_SName;
		for(i=0; i < nSizeNum; i++)
			xzSectData.Size[i] = m_pParent->m_sDlgData.XZSect_Size[i];
		if(m_pParent->m_StiffType == 1) // User
			CSectUtil::CalcThikStiffness(xzSectData);
		else if(m_pParent->m_StiffType == 2) // DB
			CSectUtil::GetSectOfThik(dbName, xzSectData);
	}


	m_pParent->m_sDlgData.nType = m_pParent->m_StiffType;
	m_pParent->m_sDlgData.dPlateThk = m_pParent->m_dThkofPlate.GetEditValue();
	m_pParent->m_sDlgData.dWeightThk = m_pParent->m_dWeight.GetEditValue();


	thik.nType = 1;
	thik.Stiffened.DBName = m_pParent->m_sDlgData.DBName;
	thik.Stiffened.dPlateThik = m_pParent->m_sDlgData.dPlateThk;
	thik.Stiffened.nType = m_pParent->m_sDlgData.nType;

	if(m_pParent->m_sDlgData.YZSect_Exist)
	{
		thik.Stiffened.yzSect.bRibAttached = TRUE;	//추가(김영민)
		thik.Stiffened.yzSect.Dist = m_pParent->m_sDlgData.YZSect_Dist;
		thik.Stiffened.yzSect.Shape = m_pParent->m_sDlgData.YZSect_Shape;
		thik.Stiffened.yzSect.SName = m_pParent->m_sDlgData.YZSect_SName;
		thik.Stiffened.yzSect.PeriIn = yzSectData.PeriIn;
		thik.Stiffened.yzSect.PeriOut = yzSectData.PeriOut;
		thik.Stiffened.yzSect.Stiffness = yzSectData.Stiffness;
		for(i=0; i < 10; i++)
			thik.Stiffened.yzSect.Size[i] = m_pParent->m_sDlgData.YZSect_Size[i];
	}

	if(m_pParent->m_sDlgData.XZSect_Exist)
	{
		thik.Stiffened.xzSect.bRibAttached = TRUE;	//추가(김영민)
		thik.Stiffened.xzSect.Dist = m_pParent->m_sDlgData.XZSect_Dist;
		thik.Stiffened.xzSect.Shape = m_pParent->m_sDlgData.XZSect_Shape;
		thik.Stiffened.xzSect.SName = m_pParent->m_sDlgData.XZSect_SName;
		thik.Stiffened.xzSect.PeriIn = xzSectData.PeriIn;
		thik.Stiffened.xzSect.PeriOut = xzSectData.PeriOut;
		thik.Stiffened.xzSect.Stiffness = xzSectData.Stiffness;
		for(i=0; i < 10; i++)
			thik.Stiffened.xzSect.Size[i] = m_pParent->m_sDlgData.XZSect_Size[i];
	}

	m_pDoc->m_pSectDB->CalcThikData(thik);
	return TRUE;
}

void CCMThkItemStiffUser::OnChangeSectSize() 
{
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
		case IDC_CMD_TP_ID_VALUE1 : nIx = 0; break;
		case IDC_CMD_TP_ID_VALUE2 : nIx = 1; break;
		case IDC_CMD_TP_ID_VALUE3 : nIx = 2; break;
		case IDC_CMD_TP_ID_VALUE4 : nIx = 3; break;
		case IDC_CMD_TP_ID_VALUE5 : nIx = 4; break;
		case IDC_CMD_TP_ID_VALUE6 : nIx = 5; break;
		default: ASSERT(0); break;
	}

	Dlg2Data();
	m_pParent->InvalidateSecView();
}

BOOL CCMThkItemStiffUser::Dlg2Data()
{
//	m_Data.Initialize();
	m_pParent->m_SectData.nStype = D_SECT_TYPE_REGULAR;

	// Shape
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	m_pParent->m_SectData.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	// First
	T_SECT_SECTBASE_D* pSectF = &(m_pParent->m_SectData.SectBefore.SectI);
	{
		// DB
		pSectF->DBName = m_pParent->m_sDlgData.DBName;

		// Shape : 없음
		pSectF->Shape = _T("");
		// Size
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_pParent->m_SectData.SectBefore.Shape);
		// KJH20040125, Section에서 H, C에 r1, r2 넣은거 빼기
		if(nShapeIndex==1 || nShapeIndex == 2) nSizeNum -= 2;

		for (int i = 0; i < nSizeNum; i++)
			pSectF->Size[i] = m_wndFirstSize[i].GetEditValue();
	}

/*
	// cc 관련
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CC)
		m_Data.SectBefore.SectI.Shape = GetCurCCShape();
*/
	return TRUE;
}

void CCMThkItemStiffUser::OnCmdTpExistRibs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int ribexist = m_ExistRib.GetCheck();

	CDlgUtil::CtrlEnableDisable(this, m_aAllCtrl, ribexist);
	if (ribexist)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisableInUser, m_pParent->m_StiffType != 1);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisableInDB, m_pParent->m_StiffType != 2);
	}

	if(m_nIdentity == 0) // YZ Section
		m_pParent->m_sDlgData.YZSect_Exist = ribexist;
	else // XZ Section
		m_pParent->m_sDlgData.XZSect_Exist = ribexist;
}
