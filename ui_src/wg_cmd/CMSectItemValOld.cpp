// CMSectItemVal.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemValOld.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "CMSecPageStiffDlg.h"

// Default Shape
#define D_SECT_VALUE_DEF_SHAPE D_SECT_SHAPE_REG_H
#include "..\wg_base\wg_base_MsgDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemValOld dialog
#define   CDialog   CCMSectItemBase

CCMSectItemValOld::CCMSectItemValOld(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemValOld::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemValOld)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectItemValOld::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemValOld)
	DDX_Control(pDX, IDC_CMD_SP_ID_OFFSET, m_wndOffset);
	DDX_Control(pDX, IDC_CMD_SP_ID_BUILTUP, m_wndBuiltUp);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	//}}AFX_DATA_MAP
	UINT aFirstID[][6] = {
		{ IDC_CMD_SP_ID_TITLE1, IDC_CMD_SP_ID_TITLE2, IDC_CMD_SP_ID_TITLE3,
			IDC_CMD_SP_ID_TITLE4, IDC_CMD_SP_ID_TITLE5, IDC_CMD_SP_ID_TITLE6 },
		{ IDC_CMD_SP_ID_VALUE1, IDC_CMD_SP_ID_VALUE2, IDC_CMD_SP_ID_VALUE3,
			IDC_CMD_SP_ID_VALUE4, IDC_CMD_SP_ID_VALUE5, IDC_CMD_SP_ID_VALUE6 },
		{ IDC_CMD_SP_ID_UNIT1,  IDC_CMD_SP_ID_UNIT2,  IDC_CMD_SP_ID_UNIT3,
			IDC_CMD_SP_ID_UNIT4,  IDC_CMD_SP_ID_UNIT5,  IDC_CMD_SP_ID_UNIT6 },
	};
	for (int i = 0; i < 6; i++)
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
BOOL CCMSectItemValOld::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemValOld::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		CString strID;
		strID.Format("%d", m_Key);
		m_wndID.SetWindowText(strID);
		//m_Data.SName = "";
		//m_wndName.SetWindowText(m_Data.SName);
		m_wndName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemValOld::ShowCalcResult()
{
	return;
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemValOld::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == "")
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemValOld::Dlg2Data()
{
	// Save Peri and Stiffness
	double dblPeriIn = m_Data.SectBefore.SectI.PeriIn;
	double dblPeriOut = m_Data.SectBefore.SectI.PeriOut;
	T_SECT_STIFFNESS stiff;
	stiff = m_Data.SectBefore.SectI.Stiffness;

	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_USER;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)atol(csID);

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
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	// Built-Up
	if (m_wndBuiltUp.GetCheck() == 1)
		m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // built-up
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // rolled

	// Offset
	m_Data.SectBefore.nOffsetPoint = m_wndOffset.GetCurSel()+1;

	// First
	T_SECT_SECTBASE_D* pSectF = &(m_Data.SectBefore.SectI);
	{
		// DB : 없음
		pSectF->DBName = "";
		// Shape : 없음
		pSectF->Shape = "";
		// SName : 없음
		pSectF->SName = "";
		// Size
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_Data.SectBefore.Shape);
		for (int i = 0; i < nSizeNum; i++)
			pSectF->Size[i] = m_wndFirstSize[i].GetEditValue();
	}
	// Peri and stiffness
	m_Data.SectBefore.SectI.PeriIn = dblPeriIn;
	m_Data.SectBefore.SectI.PeriOut = dblPeriOut;
	stiff.WArea = stiff.Area; // WArea는 Area와 동일
	m_Data.SectBefore.SectI.Stiffness = stiff;

	return TRUE;
}

void CCMSectItemValOld::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	// Init Offset combo
	m_wndOffset.ResetContent();
	CStringArray aOffsetNameList;
	CSectUtil::GetOffsetNameList(aOffsetNameList);
	for (int i = 0; i < aOffsetNameList.GetSize(); i++)
		m_wndOffset.AddString(aOffsetNameList.GetAt(i));
	m_wndOffset.SetCurSel(4); // default value : center, center

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// Unit control
	for (i = 0; i < 6; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
}

void CCMSectItemValOld::SetSectionShapeCombo()
{
	UINT aBitmapID[] = { 
		IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
		IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
		IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
		IDB_CMD_SEC_REG_10, 
	};
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section), _LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe), 
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle), _LS(IDS_WG_CMD__ADDD__Solid_Round),
	};

	int nCount = sizeof(aBitmapID)/sizeof(UINT);

	// build image list
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

	for (i = 0; i < nCount; i++)
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

void CCMSectItemValOld::ShowKey2Dlg()
{
	CString csKey;
	csKey.Format("%ld", m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemValOld::ShowData2Dlg()
{
	// name
	m_wndName.SetWindowText(m_Data.SName);
	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	m_cboType.SetCurSel(nShapeIndex);

	// BuiltUp
	SetBuiltUp();

	// first
	SetFirstSectData();   // Title, Value 설정, Show/Hide 수행

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	m_wndSecView.Invalidate();
}

void CCMSectItemValOld::SetFirstSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_value_section_sha));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	for (int i = 0;i < nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format("%g", pSect->Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
	}
	for (; i < 6; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);
	}
}

void CCMSectItemValOld::ChangeBitmap()
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	UINT aBitmapID[] = {
		IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP03,
		IDB_CMD_SP_ISGL_BMP04, IDB_CMD_SP_ISGL_BMP05, IDB_CMD_SP_ISGL_BMP06,
		IDB_CMD_SP_ISGL_BMP07, IDB_CMD_SP_ISGL_BMP08, IDB_CMD_SP_ISGL_BMP09,
		IDB_CMD_SP_ISGL_BMP10,
	};
	// set appropriate bitmap
	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
	m_pBitmap = (CBitmap*)SecImage;
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nShapeIndex]);
	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCMSectItemValOld::SetBuiltUp()
{
	// Built-Up Setting
	if (m_Data.SectBefore.SectI.DBName == "") 
		m_wndBuiltUp.EnableWindow(TRUE);
	else m_wndBuiltUp.EnableWindow(FALSE);
	if (m_Data.SectBefore.SectI.BuiltUpFlag == 2) m_wndBuiltUp.SetCheck(0);
	else m_wndBuiltUp.SetCheck(1);
}

BEGIN_MESSAGE_MAP(CCMSectItemValOld, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemValOld)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE1, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_ID, OnChangeID)
	ON_EN_CHANGE(IDC_CMD_SP_ID_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BUILTUP, OnChangeBuiltUp)
	ON_BN_CLICKED(IDC_CMP_SP_ID_BTN_STIFF, OnClickBtnStiffness)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE2, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE3, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE4, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE5, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE6, OnChangeFirstSize)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemValOld message handlers

BOOL CCMSectItemValOld::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();
		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_USER;
		m_Data.SectBefore.Shape = D_SECT_VALUE_DEF_SHAPE;
	}
	m_wndSecView.SetDataSource(&m_Data);

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemValOld::OnChangeShape() 
{
	// TODO: Add your control notification handler code here
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	T_SECT_SECTBASE_D *pSectF;
	pSectF = &m_Data.SectBefore.SectI;

	// Section Name은 보존
	m_wndName.GetWindowText(m_Data.SName);

	// Peri 초기화
	m_Data.SectBefore.SectI.PeriIn = 0.0;
	m_Data.SectBefore.SectI.PeriOut = 0.0;

	// First 초기화
	pSectF->Initialize();

	// Dialog에 반영
	ShowData2Dlg();		
}

void CCMSectItemValOld::OnChangeFirstSize() 
{
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
	case IDC_CMD_SP_ID_VALUE6 : nIx = 5; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndFirstSize[nIx].GetEditValue();
	m_wndSecView.Invalidate();	
}

void CCMSectItemValOld::OnChangeID() 
{
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = atol(csID);
}

void CCMSectItemValOld::OnChangeName() 
{
	m_wndName.GetWindowText(m_Data.SName);
}

void CCMSectItemValOld::OnChangeBuiltUp() 
{
	// TODO: Add your control notification handler code here
	if (m_wndBuiltUp.GetCheck()) m_Data.SectBefore.SectI.BuiltUpFlag = 1;
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2;
}

void CCMSectItemValOld::OnClickBtnStiffness() 
{
	// TODO: Add your control notification handler code here
	T_SECT_D Data;
	Data = m_Data;
	Data.nStype = D_SECT_TYPE_REGULAR;
	m_pDoc->m_pSectDB->CalcSectData(Data);

	/*
	double dblPeriIn = m_Data.SectBefore.SectI.PeriIn;
	double dblPeriOut = m_Data.SectBefore.SectI.PeriOut;
	T_SECT_STIFFNESS Stiffness = m_Data.SectBefore.SectI.Stiffness;
	CCMSecPageStiffDlg dlg(m_pDoc, &Data, &Stiffness, &dblPeriIn, &dblPeriOut, this);
	if (dlg.DoModal() == IDOK)
	{
		m_Data.SectBefore.SectI.PeriIn = dblPeriIn;
		m_Data.SectBefore.SectI.PeriOut = dblPeriOut;
		m_Data.SectBefore.SectI.Stiffness = Stiffness;
	}
	*/
}

void CCMSectItemValOld::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);
}
