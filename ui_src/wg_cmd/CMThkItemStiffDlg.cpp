// CMThkItemStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkItemStiffDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "CMThkPageItemDlg.h"
#include "CMThkItemStiffValue.h"
#include "CMThkItemStiffUser.h"
#include "CMThkItemStlWallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	constexpr int THIK_STIFFENED_USER_SIZE_COUNT = 6;
	enum STIFF_TYPE : unsigned int
	{
		VALUE = 0,
		USER,
		DB,
	};
}

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffDlg dialog

CCMThkItemStiffDlg::CCMThkItemStiffDlg(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMThkItemStiffDlg::IDD, pParent)
{
	m_pParent = (CCMThkPageItemDlg*)pParent;
	m_bInit = TRUE;
	m_bModify = FALSE;
	
	m_Key = 0;
	m_Data.Initialize();
	m_SectData.Initialize();

	m_Key = 0;
	m_Data.Initialize();
	m_SectData.Initialize();

	//{{AFX_DATA_INIT(CCMThkItemStiffDlg)
	m_StiffType = 0;
	m_nID = 0;
	m_RibPos = -1;
	//}}AFX_DATA_INIT

	m_aThikPlate.Add(IDC_CMD_TP_STIFF_TITLE1);
	m_aThikPlate.Add(IDC_CMD_TP_STIFF_THK);
	m_aThikPlate.Add(IDC_CMD_TP_STIFF_THK_UNIT);
	m_aThikWeight.Add(IDC_CMD_TP_STIFF_TITLE2);
	m_aThikWeight.Add(IDC_CMD_TP_STIFF_WEIGHT);
	m_aThikWeight.Add(IDC_CMD_TP_STIFF_WEIGHT_UNIT);

	m_sDlgData.Initialize();

	m_aValuePic.Add(IDC_CMD_TP_ID_BMP);
	m_aValuePic.Add(IDC_CMD_TP_ID_PREVIEW);
	m_aUserPic.Add(IDC_CMD_TP_ID_BMP2);
	m_aUserPic.Add(IDC_CMD_TP_ID_PREVIEW2);

}

CCMThkItemStiffDlg::~CCMThkItemStiffDlg()
{

}

void CCMThkItemStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkItemStiffDlg)
	DDX_Control(pDX, IDC_CMD_TP_STIFF_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_TP_STIFF_WEIGHT_UNIT, m_UnitWeight);
	DDX_Control(pDX, IDC_CMD_TP_STIFF_THK_UNIT, m_UnitThkofPlate);
	DDX_Control(pDX, IDC_CMD_TP_STIFF_WEIGHT, m_dWeight);
	DDX_Control(pDX, IDC_CMD_TP_STIFF_THK, m_dThkofPlate);
	DDX_Control(pDX, IDC_CMD_TP_STIFF_TAB, m_wndTab);
	DDX_Radio(pDX, IDC_CMD_TP_STIFF_VALUE, m_StiffType);
	DDX_Text(pDX, IDC_CMD_TP_STIFF_ID, m_nID);
	DDX_Radio(pDX, IDC_CMD_TP_STIFF_POSLOWER, m_RibPos);
	DDX_Control(pDX, IDC_CMD_TP_ID_BMP2, m_wndPictureMid);
	DDX_Control(pDX, IDC_CMD_TP_ID_PREVIEW, m_wndPictureBottom);
	DDX_Control(pDX, IDC_CMD_TP_ID_BMP3, m_wndPictureTop);
	DDX_Control(pDX, IDC_CMD_TP_ID_BMP, m_wndPictureMidValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThkItemStiffDlg, CDlgChild)
	//{{AFX_MSG_MAP(CCMThkItemStiffDlg)
	ON_BN_CLICKED(IDC_CMD_TP_STIFF_DB, OnChangeStiffType)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TP_STIFF_TAB, OnChangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CMD_TP_STIFF_TAB, OnBeforeChangeTab)
	ON_CBN_SELCHANGE(IDC_CMD_TP_STIFF_CODE, OnSelchangeCmdTpStiffCode)
	ON_BN_CLICKED(IDC_CMD_TP_STIFF_USER, OnChangeStiffType)
	ON_BN_CLICKED(IDC_CMD_TP_STIFF_VALUE, OnChangeStiffType)
	ON_BN_CLICKED(IDC_CMD_TP_STIFF_POSLOWER, OnSelChangeRibPos)
	ON_BN_CLICKED(IDC_CMD_TP_STIFF_POSUPPER, OnSelChangeRibPos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffDlg message handlers

BOOL CCMThkItemStiffDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDlgChild::OnInitDialog();

	if(!m_bModify) ImportDBData();
	InitialDBData(TRUE);
	MakeUnitSystem();

	// Dialog Page Manager에 Place Holder 설정
	CWnd* pPH = (CWnd*)GetDlgItem(IDC_CMD_STIFF_PLACEHOLDER);
	ASSERT(pPH && pPH->GetSafeHwnd() && IsWindow(pPH->GetSafeHwnd()));
	m_wndTab.SetPlaceHolder(pPH);

	SetTabDlg(m_StiffType, TRUE);

	// enable/disable //////////////////////////////////////////////
	CComboBox* pWnd = (CComboBox*)GetDlgItem(IDC_CMD_TP_STIFF_CODE);
	pWnd->EnableWindow(m_StiffType == DB);

	CDlgUtil::CtrlEnableDisable(this, m_aThikPlate, m_StiffType != VALUE);
	CDlgUtil::CtrlEnableDisable(this, m_aThikWeight, m_StiffType == VALUE);
	////////////////////////////////////////////////////////////////

	// preview control
	CWnd* pSecWnd = GetDlgItem(IDC_CMD_TP_ID_PREVIEW2);
	ASSERT(pSecWnd);
	m_wndSecView.Init(pSecWnd);
	m_wndSecView.SetDataSource(&m_SectData);

	AlignControls();
	OnChangeStiffType();
	OnSelChangeRibPos();
	if (m_wndTab.GetCurSel() == 0)
	{
		ChangeBitmap(m_sDlgData.YZSect_Shape);
		ChangeBitmap1(0);
	}
	else
	{
		ChangeBitmap(m_sDlgData.XZSect_Shape);
		ChangeBitmap1(1);
	}
	m_wndPictureMidValue.SetImage(_T("SVG\\Illustration\\Dialog\\thik01.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkItemStiffDlg::InvalidateSecView()
{
	if(m_SectData.SectBefore.Shape == D_SECT_SHAPE_REG_L ||
		 m_SectData.SectBefore.Shape == D_SECT_SHAPE_REG_T ||
		 m_SectData.SectBefore.Shape == D_SECT_SHAPE_REG_2L)
		m_wndSecView.m_bReverse = TRUE;
	else
		m_wndSecView.m_bReverse = FALSE;

	HWND hwnd = m_wndSecView.GetSafeHwnd();
	if (hwnd && IsWindow(hwnd)) m_wndSecView.Invalidate();
}

void CCMThkItemStiffDlg::MakeUnitSystem() 
{
	m_UnitThkofPlate.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.dPlateThik);
	m_UnitWeight.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.dWeightThik);
}

void CCMThkItemStiffDlg::AlignControls()
{
	CRect rRef;
	CRect rMove;
	int nDistX;

	GetDlgItem(IDC_CMD_TP_ID_PREVIEW)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TP_ID_PREVIEW2)->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aUserPic, nDistX);


	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_TP_STIFF_TAB);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rectLast.right+globalUtils.ScaleByDPI(5);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CCMThkItemStiffDlg::InitialDBData(BOOL bOnInitDialog)
{
	m_dThkofPlate.SetEditUnit(m_sDlgData.dPlateThk);
	m_dWeight.SetEditUnit(m_sDlgData.dWeightThk);

	SetDBNameList();

	if(m_bModify)
	{
		m_StiffType = m_sDlgData.nType;
		m_RibPos = m_sDlgData.nRibPos;
		m_wndDB.SelectString(-1, m_sDlgData.DBName);
		m_nID = m_sDlgData.ID;
	}
	else
	{
	  m_wndDB.SetCurSel(0);
		m_wndDB.GetWindowText(m_sDlgData.DBName);

		if (bOnInitDialog) // OnInitDialog에서 호출(최초 실행일 경우)
			m_nID = m_pDoc->m_pAttrCtrl->GetStartNumThik();
	  else // Apply에서 실행될 경우
	    m_nID = m_pDoc->m_pAttrCtrl->GetNextNumThik(m_nID); // type은 다시 설정 않는다.
	}

	UpdateData(FALSE);
}

void CCMThkItemStiffDlg::SetDBNameList()
{
#ifdef _MVIL
	m_wndDB.AddString(D_SECT_DB_KS);
#else
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);

	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
		m_wndDB.AddString(DBNameList.GetAt(i));
	}
#endif
}

void CCMThkItemStiffDlg::ImportDBData()
{
//	T_THIK_D data;
//	data = m_pParent->m_Data;

	m_Data.Initialize();

	m_StiffType =	m_Data.nType;
	m_RibPos = m_Data.Stiffened.nRibPosition-1;

	m_sDlgData.ID = m_pDoc->m_pAttrCtrl->GetStartNumSect();
	m_sDlgData.nType = m_Data.nType;
	m_sDlgData.dPlateThk = m_Data.Stiffened.dPlateThik;
	m_sDlgData.dWeightThk = m_Data.Stiffened.dWeightThik;
	m_sDlgData.nRibPos = m_Data.Stiffened.nRibPosition-1;

	m_sDlgData.YZValue_Shape = m_Data.Stiffened.yzValue.Shape;
	if(m_sDlgData.YZValue_Shape.IsEmpty()) m_sDlgData.YZValue_Shape = D_SECT_SHAPE_REG_URIB;
	m_sDlgData.YZValue_ThickIn = m_Data.Stiffened.yzValue.ThickIn;
	m_sDlgData.YZValue_ThickOut = m_Data.Stiffened.yzValue.ThickOut;
	m_sDlgData.YZValue_Hu = m_Data.Stiffened.yzValue.Hu;
	m_sDlgData.YZValue_Hl = m_Data.Stiffened.yzValue.Hl;

	m_sDlgData.XZValue_Shape = m_Data.Stiffened.xzValue.Shape;
	if(m_sDlgData.XZValue_Shape.IsEmpty()) m_sDlgData.XZValue_Shape = D_SECT_SHAPE_REG_URIB;
	m_sDlgData.XZValue_ThickIn = m_Data.Stiffened.xzValue.ThickIn;
	m_sDlgData.XZValue_ThickOut = m_Data.Stiffened.xzValue.ThickOut;
	m_sDlgData.XZValue_Hu = m_Data.Stiffened.xzValue.Hu;
	m_sDlgData.XZValue_Hl = m_Data.Stiffened.xzValue.Hl;



	m_sDlgData.YZSect_Exist = m_Data.Stiffened.yzSect.bRibAttached;
	m_sDlgData.YZSect_Shape = m_Data.Stiffened.yzSect.Shape;
	if(m_sDlgData.YZSect_Shape.IsEmpty()) m_sDlgData.YZSect_Shape = D_SECT_SHAPE_REG_URIB;
	m_sDlgData.YZSect_Dist = m_Data.Stiffened.yzSect.Dist;
	m_sDlgData.YZSect_SName = m_Data.Stiffened.yzSect.SName;

	for (int i = 0; i < 10; i++)
		m_sDlgData.YZSect_Size[i] = m_Data.Stiffened.yzSect.Size[i];

	m_sDlgData.XZSect_Exist = m_Data.Stiffened.xzSect.bRibAttached;
	m_sDlgData.XZSect_Shape = m_Data.Stiffened.xzSect.Shape;
	if(m_sDlgData.XZSect_Shape.IsEmpty()) m_sDlgData.XZSect_Shape = D_SECT_SHAPE_REG_URIB;
	m_sDlgData.XZSect_Dist = m_Data.Stiffened.xzSect.Dist;
	m_sDlgData.XZSect_SName = m_Data.Stiffened.xzSect.SName;

	for (int i = 0; i < 10; i++)
		m_sDlgData.XZSect_Size[i] = m_Data.Stiffened.xzSect.Size[i];
}

void CCMThkItemStiffDlg::SetTabDlg(int Type, BOOL flg)
{
	UINT dlgID;
	if(!flg) DelAllTabs();

	if(Type == 0) // value
	{
		m_pCurYZDlg = new CCMThkItemStiffValue(0, this);
		m_pCurXZDlg = new CCMThkItemStiffValue(1, this);
		dlgID = CCMThkItemStiffValue::IDD;
		m_pParent->EnableDisableCalcBtn(FALSE);
	}
	else // user, DB
	{
		m_pCurYZDlg	= new CCMThkItemStiffUser(0, this);
		m_pCurXZDlg	= new CCMThkItemStiffUser(1, this);
		dlgID = CCMThkItemStiffUser::IDD;
		m_pParent->EnableDisableCalcBtn(TRUE);
	}

	m_wndTab.AddTab(m_pCurYZDlg, _LS(IDS_WG_CMD__ADDD__yz_section), dlgID, TRUE);
	m_wndTab.AddTab(m_pCurXZDlg, _LS(IDS_WG_CMD__ADDD__xz_section), dlgID, TRUE);

	m_wndTab.ShowTab(0);
}

void CCMThkItemStiffDlg::DelAllTabs()
{
	int nNum = m_wndTab.GetTabCount();
	for(int i=0; i < nNum; i++)
		m_wndTab.DelTab(0);
}

void CCMThkItemStiffDlg::OnChangeStiffType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_pCurYZDlg && ::IsWindow(m_pCurYZDlg->GetSafeHwnd())) m_pCurYZDlg->SaveData();	// added by KYM
	if (m_pCurXZDlg && ::IsWindow(m_pCurXZDlg->GetSafeHwnd())) m_pCurXZDlg->SaveData();	// added by KYM

	SetTabDlg(m_StiffType);

	CComboBox* pWnd = (CComboBox*)GetDlgItem(IDC_CMD_TP_STIFF_CODE);
	pWnd->EnableWindow(m_StiffType == DB);

	CDlgUtil::CtrlEnableDisable(this, m_aThikPlate, m_StiffType != VALUE);
	CDlgUtil::CtrlEnableDisable(this, m_aThikWeight, m_StiffType == VALUE);

	if(m_StiffType == VALUE)
	{
		CDlgUtil::CtrlShowHide(this, m_aValuePic, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aUserPic, FALSE);
	}
	else
	{
		if (m_StiffType == USER)
		{
			const auto xzSectSize = min(CSectUtil::GetSizeCountByRegShape(m_sDlgData.XZSect_Shape), THIK_STIFFENED_USER_SIZE_COUNT);
			for (int idx = xzSectSize; idx < 10; ++idx)
				m_sDlgData.XZSect_Size[idx] = 0.0;

			const auto yzSectSize = min(CSectUtil::GetSizeCountByRegShape(m_sDlgData.YZSect_Shape), THIK_STIFFENED_USER_SIZE_COUNT);
			for (int idx = yzSectSize; idx < 10; ++idx)
				m_sDlgData.YZSect_Size[idx] = 0.0;
		}

		CDlgUtil::CtrlShowHide(this, m_aValuePic, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUserPic, TRUE);
		ChangeBitmap(m_sDlgData.YZSect_Shape);
	}


}

void CCMThkItemStiffDlg::ChangeBitmap1(int nNum)
{
	CString aBitmapPath[] = 
	{
		_T("SVG\\Illustration\\Dialog\\thik02.svg"),
		_T("SVG\\Illustration\\Dialog\\thik03.svg")
	};
	if (nNum == 0) m_wndPictureBottom.SetImage(aBitmapPath[0]);
	else if (nNum == 1) m_wndPictureBottom.SetImage(aBitmapPath[1]);
}

void CCMThkItemStiffDlg::ChangeBitmap(CString shape)
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	CString aBitmapPath[] = {
		_T("SVG\\Illustration\\Dialog\\thikreg01.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec02.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec03.svg"),
		_T("SVG\\Illustration\\Dialog\\thikreg04.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec05.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec06.svg"),
		_T("SVG\\Illustration\\Dialog\\thikreg07.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec08.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec09.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec10.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec11.svg"),
		_T("SVG\\Illustration\\Dialog\\Sec12.svg")
	};
	m_wndPictureMid.SetImage(aBitmapPath[nShapeIndex]);
}

BOOL CCMThkItemStiffDlg::OnApplyPublic()
{
	UpdateData();
	if(m_StiffType == VALUE)
	{
		if(m_wndTab.GetCurSel() == 0)
			((CCMThkItemStiffValue*)m_pCurYZDlg)->OnApplyPublic();
		else
			((CCMThkItemStiffValue*)m_pCurXZDlg)->OnApplyPublic();
	}
	else
	{
		if(m_wndTab.GetCurSel() == 0)
			((CCMThkItemStiffUser*)m_pCurYZDlg)->OnApplyPublic();
		else
			((CCMThkItemStiffUser*)m_pCurXZDlg)->OnApplyPublic();
	}

//	m_dThkofPlate.SetEditUnit(m_sDlgData.dPlateThk);
//	m_dWeight.SetEditUnit(m_sDlgData.dWeightThk);

	m_sDlgData.ID = m_nID;
	m_sDlgData.nType = m_StiffType;
//	m_sDlgData.DBName
	m_sDlgData.dPlateThk = m_dThkofPlate.GetEditValue();
	m_sDlgData.dWeightThk = m_dWeight.GetEditValue();

	return TRUE;
}

void CCMThkItemStiffDlg::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CCMThkItemStiffDlg::OnBeforeChangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();
	if(m_StiffType == VALUE)
	{
		if(m_wndTab.GetCurSel() == 0)
		{
			((CCMThkItemStiffValue*)m_pCurYZDlg)->OnApplyPublic();
			ChangeBitmap1(1);
		}
		else
		{
			((CCMThkItemStiffValue*)m_pCurXZDlg)->OnApplyPublic();
			ChangeBitmap1(0);
		}
	}
	else
	{
		if(m_wndTab.GetCurSel() == 0)
		{
			((CCMThkItemStiffUser*)m_pCurYZDlg)->OnApplyPublic();
			ChangeBitmap(m_sDlgData.XZSect_Shape);
		}
		else
		{
			((CCMThkItemStiffUser*)m_pCurXZDlg)->OnApplyPublic();
			ChangeBitmap(m_sDlgData.YZSect_Shape);
		}
	}
	
	*pResult = 0;
}

void CCMThkItemStiffDlg::OnSelchangeCmdTpStiffCode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_wndDB.GetWindowText(m_sDlgData.DBName);

	if(m_StiffType != 0)
	{
		if(m_wndTab.GetCurSel() == 0)
			((CCMThkItemStiffUser*)m_pCurYZDlg)->UpdateSectNameCobx();
		else
			((CCMThkItemStiffUser*)m_pCurXZDlg)->UpdateSectNameCobx();
	}
}

int CCMThkItemStiffDlg::GetCurStiffType()
{
	UpdateData();
	return m_StiffType;
}

int CCMThkItemStiffDlg::GetCurStiffSectionType()
{
	UpdateData();
	return m_wndTab.GetCurSel();
}

BOOL CCMThkItemStiffDlg::GetCurCalcData(T_THIK_D& thik)
{
	BOOL ret;
	if(m_wndTab.GetCurSel() == 0)
		ret = ((CCMThkItemStiffUser*)m_pCurYZDlg)->GetCurCalcData(thik);
	else
		ret = ((CCMThkItemStiffUser*)m_pCurXZDlg)->GetCurCalcData(thik);

	return ret;
}

void CCMThkItemStiffDlg::OnSelChangeRibPos()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CString aBitmapPath[] =
	{
		_T("SVG\\Illustration\\Dialog\\thik04.svg"),
		_T("SVG\\Illustration\\Dialog\\thik05.svg")
	};
	if(m_RibPos == 0) m_wndPictureTop.SetImage(aBitmapPath[0]);
	else if(m_RibPos == 1) m_wndPictureTop.SetImage(aBitmapPath[1]);

	m_sDlgData.nRibPos = m_RibPos;
}
