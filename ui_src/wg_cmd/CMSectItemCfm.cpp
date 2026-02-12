// CMSectItemCfm.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemCfm.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\SectDBRussiaUtil.h"

#include "CMSecPageStiffDlg.h"
#include "CMSectOffsetDlg.h"

// Default Shape
#define D_SECT_CFM_DEF_SHAPE D_SECT_SHAPE_COLF_CC
#define D_SECT_DB_KS        _T("KS")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCfm dialog
#define   CDialog   CCMSectItemBase

CCMSectItemCfm::CCMSectItemCfm(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemCfm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemCfm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBitmap = NULL;
	m_pImageList = NULL;
}

CCMSectItemCfm::~CCMSectItemCfm()
{
	if (m_pBitmap != NULL) { delete m_pBitmap; m_pBitmap = NULL; }
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList = NULL; }
}

void CCMSectItemCfm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemCfm)
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_SP_ID_BUILTUP, m_wndBuiltUp);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME, m_wndFirstName);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	//}}AFX_DATA_MAP

	UINT aFirstID[][5] = {
		{ IDC_CMD_SP_ID_TITLE1, IDC_CMD_SP_ID_TITLE2, IDC_CMD_SP_ID_TITLE3,
			IDC_CMD_SP_ID_TITLE4, IDC_CMD_SP_ID_TITLE5, },
		{ IDC_CMD_SP_ID_VALUE1, IDC_CMD_SP_ID_VALUE2, IDC_CMD_SP_ID_VALUE3,
			IDC_CMD_SP_ID_VALUE4, IDC_CMD_SP_ID_VALUE5, },
		{ IDC_CMD_SP_ID_UNIT1,  IDC_CMD_SP_ID_UNIT2,  IDC_CMD_SP_ID_UNIT3,
			IDC_CMD_SP_ID_UNIT4,  IDC_CMD_SP_ID_UNIT5,  },
	};
	for (int i = 0; i < 5; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemCfm::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemCfm::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		CString strID;
		strID.Format(_T("%d"), m_Key);
		m_wndID.SetWindowText(strID);
		//m_Data.SName = _T("");
		//m_wndName.SetWindowText(m_Data.SName);
		//m_wndName.SetFocus();
		m_wndFirstName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemCfm::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	CCMStiffDlg dlg(m_pDoc, &m_Data, NULL, this);
	dlg.DoModal();
}

// View에 Offset Point를 그려준다. 
void CCMSectItemCfm::DisplayOffsetPoint()
{
	m_wndSecView.Invalidate();
}

void CCMSectItemCfm::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemCfm::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemCfm::Dlg2Data()
{
	// offset data backup
	int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}
	
	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_COLDFORMED;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexCfm(nShapeIndex);

	// Built-Up
	if (m_wndBuiltUp.GetCheck() == 1)
		m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // built-up
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // rolled

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// First
	T_SECT_SECTBASE_D* pSectF = &(m_Data.SectBefore.SectI);
	{
		// DB
		pSectF->DBName = GetDB();

		// Shape : 없음
		pSectF->Shape = _T("");
		// SName : 
		if (pSectF->DBName == _T("")) pSectF->SName = _T("");
		else m_wndFirstName.GetWindowText(pSectF->SName);

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSectF->DBName, pSectF->SName);
#endif

		// Size
		int nSizeNum = CSectUtil::GetSizeCountByCfmShape(m_Data.SectBefore.Shape);
		for (int i = 0; i < nSizeNum; i++)
			pSectF->Size[i] = m_wndFirstSize[i].GetEditValue();
	}

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	return TRUE;
}

BOOL CCMSectItemCfm::GetPeriAndStiff()
{
	return CSectUtil::CalcSectData(m_Data);
}

void CCMSectItemCfm::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	// Init DB name and section name combo
	SetDBNameList();
	m_wndDB.SetCurSel(0);
	m_wndFirstName.SetActionWhenKillFocus(2, -1);

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// Unit control
	for (int i = 0; i < 5; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
}

void CCMSectItemCfm::SetSectionShapeCombo()
{
	UINT aBitmapID[] = { 
		IDB_CMD_SEC_CFM_01, IDB_CMD_SEC_CFM_02, IDB_CMD_SEC_CFM_03,
		IDB_CMD_SEC_CFM_04, IDB_CMD_SEC_CFM_05, 
	};
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Cold_Formed___CC), _LS(IDS_WG_CMD__ADDD__Cold_Formed___2CCI), _LS(IDS_WG_CMD__ADDD__Cold_Formed___2CCB),
		_LS(IDS_WG_CMD__ADDD__Cold_Formed___3CC), _LS(IDS_WG_CMD__ADDD__Cold_Formed___4CC),
	};

	int nCount = sizeof(aBitmapID)/sizeof(UINT);

	// build image list
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList = NULL; }

	m_pImageList = new CImageList;
	m_pImageList->Create(25, 25, ILC_COLOR, nCount, 1);

	// load image and add it to image list
	for (int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(aBitmapID[i]);
		m_pImageList->Add(&bitmap, COLORREF(RGB(0,0,0)));
	}

	// Attach image list to ComboBox
	m_cboType.SetImageList(m_pImageList);

	// Make items
	COMBOBOXEXITEM cbi;
	CString str;

	for (int i = 0; i < nCount; i++)
	{
		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_TEXT
							| CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE;
		cbi.iItem = i;
		str = aItemName[i];
		cbi.pszText = (LPTSTR)(LPCTSTR)str;
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		cbi.iOverlay = i;
		cbi.iIndent = 0;
		m_cboType.InsertItem(&cbi);
	}
}

void CCMSectItemCfm::SetDBNameList()
{
#ifdef _MVIL
	m_wndDB.AddString(D_SECT_DB_KS); // KS만 넣는다.
#else
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);

	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(DBNameList[i]);
#endif
		m_wndDB.AddString(DBNameList.GetAt(i));
	}
#endif
}

void CCMSectItemCfm::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemCfm::ShowData2Dlg()
{
	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameCfm(m_Data.SectBefore.Shape);
	m_cboType.SetCurSel(nShapeIndex);

	// DB
	SetDB(m_Data.SectBefore.SectI.DBName);

	// name(DB보다 나중에 설정 : For AutoChangeName)
	m_wndName.SetWindowText(m_Data.SName);

	// first, second
	SetFirstNameCombo();
	SetBuiltUp();
	SetFirstSectData();   // Title, Value 설정, Show/Hide 수행

	// offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_Data.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_Data.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_Data.SectBefore.bConsiderWarpingEffect);
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemCfm::SetFirstNameCombo()
{
	// Name List를 삭제
	m_wndFirstName.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	csDBName = pSect->DBName;
	csShape = m_Data.SectBefore.Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndFirstName.EnableWindow(FALSE);
		return;
	}
	m_wndFirstName.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndFirstName.AddString(aSectNameList.GetAt(i));
	}

	CString SectName = pSect->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndFirstName.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndFirstName.SelectString(-1, SectName);
	else m_wndFirstName.SetCurSel(nSel);
}

void CCMSectItemCfm::SetFirstSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameCfm(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_cold_formed_secti));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexCfm(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = (pSect->DBName == _T("")) ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0;i < nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
		m_wndFirstSize[i].EnableWindow(bEnable);
	}
	for (; i < 5; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);
	}
}

void CCMSectItemCfm::ChangeBitmap()
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameCfm(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_cold_formed_secti));
		return;
	}

	UINT aBitmapID[] = {
		IDB_CMD_SP_ICFM_BMP01, IDB_CMD_SP_ICFM_BMP02, IDB_CMD_SP_ICFM_BMP03,
		IDB_CMD_SP_ICFM_BMP04, IDB_CMD_SP_ICFM_BMP05,
	};
	// set appropriate bitmap
	if (m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
	m_pBitmap = (CBitmap*)SecImage;
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nShapeIndex]);
	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCMSectItemCfm::SetBuiltUp()
{
	// Built-Up Setting
	if (m_Data.SectBefore.SectI.DBName == _T("")) 
		m_wndBuiltUp.EnableWindow(TRUE);
	else m_wndBuiltUp.EnableWindow(FALSE);
	if (m_Data.SectBefore.SectI.BuiltUpFlag == 2) m_wndBuiltUp.SetCheck(0);
	else m_wndBuiltUp.SetCheck(1);
}

void CCMSectItemCfm::DoActionWhenDBChanged()
{
	CString csDB;

	// *^^* DB와 None이 라디오로 분리.
	csDB = GetDB();

	if (csDB == m_Data.SectBefore.SectI.DBName) return;

	T_SECT_SECTBASE_D* pSectF = &m_Data.SectBefore.SectI;

	// Built-Up 초기화
	if (csDB == _T("")) m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // Built-Up
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // Rolled

	// fisrt 초기화
	if (csDB != _T("")) 
	{
		pSectF->Initialize();
		// change new DB name
	  pSectF->DBName = csDB;
	}
	else // DB가 None으로 변경되면 Size는 유지
	{
		pSectF->DBName = _T("");
		pSectF->SName = _T("");
	}
 
	// Dialog에 반영한다.
	SetFirstNameCombo();
	SetBuiltUp();
	SetFirstSectData();

	// Update preview
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

CString CCMSectItemCfm::GetDB()
{
	CString csDB;
	m_wndDB.GetWindowText(csDB);
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif
	return csDB;
}

void CCMSectItemCfm::SetDB(CString &csDBName)
{
	if (csDBName == _T("")) 
	{
		ASSERT(0);
		return;
	}
	else  // use DB
	{

    CString csDB = csDBName;
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(csDB);
#endif
		m_wndDB.EnableWindow(TRUE);
		int nSel = m_wndDB.FindStringExact(-1, csDB);
		if (nSel == CB_ERR) m_wndDB.SelectString(-1, csDB);
		else m_wndDB.SetCurSel(nSel);
	}
}

BEGIN_MESSAGE_MAP(CCMSectItemCfm, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemCfm)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CODE, OnChangeDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME, OnChangeFirstName)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE1, OnChangeFirstSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BUILTUP, OnChangeBuiltUp)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_EN_UPDATE(IDC_CMD_SP_ID_NAME, OnUpdateCmdSpIdName)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE2, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE3, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE4, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE5, OnChangeFirstSize)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCfm message handlers
BOOL CCMSectItemCfm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetRedraw(FALSE);

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_COLDFORMED;
		m_Data.SectBefore.Shape = D_SECT_CFM_DEF_SHAPE;
		T_SECT_SECTBASE_D* pSectF = &m_Data.SectBefore.SectI;

		// init db value
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
		pSectF->DBName = pref.Property.SectionDBName;
		if (pSectF->DBName == _T("")) pSectF->DBName = D_SECT_DB_KS;

#ifdef _MVIL
		pSectF->DBName = D_SECT_DB_KS;
#endif
	}
	m_wndSecView.SetDataSource(&m_Data);

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemCfm::OnChangeShape() 
{
	// TODO: Add your control notification handler code here
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameCfm(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexCfm(nShapeIndex);

	T_SECT_SECTBASE_D *pSectF;
	pSectF = &m_Data.SectBefore.SectI;

	// Section Name, DB Name은 보존
	m_wndName.GetWindowText(m_Data.SName);
	CString csDBNameF = pSectF->DBName;

	// First 초기화
	pSectF->Initialize();
	pSectF->DBName = csDBNameF;

	// Dialog에 반영
	ShowData2Dlg();	
}

void CCMSectItemCfm::OnChangeDB() 
{
	// TODO: Add your control notification handler code here
	DoActionWhenDBChanged();
}

void CCMSectItemCfm::OnChangeFirstName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndFirstName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif
	if (csName == pSect->SName) return;
	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// Size Data
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 5; i++)
		pSect->Size[i] = SectData.Size[i];
	// Built Up
	m_Data.SectBefore.SectI.BuiltUpFlag = SectData.BuiltUpFlag;

	// Dialog에 반영한다.
	SetBuiltUp();
	SetFirstSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemCfm::OnChangeFirstSize() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE1 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE2 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE3 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE4 : nIx = 3; break;
	case IDC_CMD_SP_ID_VALUE5 : nIx = 4; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndFirstSize[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemCfm::OnChangeBuiltUp() 
{
	// TODO: Add your control notification handler code here
	if (m_wndBuiltUp.GetCheck() == 1)
	  m_Data.SectBefore.SectI.BuiltUpFlag = 1;
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2;
}

void CCMSectItemCfm::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);	
}

void CCMSectItemCfm::OnUpdateCmdSpIdName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CString csDB, csName;
	csDB = GetDB();
	m_wndName.GetWindowText(csName);
	if (csName == _T("")) m_bAutoChangeName = TRUE;
	else
	{
		T_SECT_SECTBASE_D SectData;

		if (csDB == _T("")) m_bAutoChangeName = FALSE;  // change by key in
		else if (m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData))
			m_bAutoChangeName = TRUE; // exist in db
		else m_bAutoChangeName = FALSE; // not exist in db(change by key in)
	}	
}

void CCMSectItemCfm::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	//m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();	
	
}
void CCMSectItemCfm::OnCmdChangeOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D OldData;
	OldData = m_Data.SectBefore;

	CCMSectOffsetDlg dlg(&m_Data.SectBefore, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		CStringArray arOffsetNameList;
		CSectUtil::GetOffsetNameList(arOffsetNameList);
		if (m_Data.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
			m_txtOffset.SetWindowText(csOffset);
		}
	}
	else m_Data.SectBefore = OldData;	
		
	m_wndSecView.Invalidate();	
}
