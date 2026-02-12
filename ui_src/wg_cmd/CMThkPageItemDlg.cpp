// CMThkPageItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkPageItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "../wg_db/wg_db_SectUtil.h"

#include "CMThkItemValueDlg.h"
#include "CMThkItemStiffDlg.h"
#include "CMThkItemCalcDlg.h"
#include "CMThkItemStlWallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	constexpr int THIK_STIFFENED_USER_SIZE_COUNT = 6;
}

/////////////////////////////////////////////////////////////////////////////
// CCMThkPageItemDlg dialog
CCMThkPageItemDlg::CCMThkPageItemDlg(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThkPageItemDlg::IDD, pParent)
{
	m_pDoc			= pDoc;
	m_bModify		= FALSE;
	m_bWorkTab	= FALSE;
	//{{AFX_DATA_INIT(CCMThkPageItemDlg)
	//}}AFX_DATA_INIT

	m_data.Initialize();

	m_pDlgValue = NULL;
	m_pDlgStiff = NULL;
	m_pDlgStlWall = NULL;
}


void CCMThkPageItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkPageItemDlg)
	DDX_Control(pDX, IDC_CMD_TAB_CTRL, m_wndTabCtrl);
	DDX_Control(pDX, IDC_CMD_TP_ITEM_BTN_APPLY, m_wndBtnApply);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMThkPageItemDlg implementation
BEGIN_MESSAGE_MAP(CCMThkPageItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThkPageItemDlg)
	ON_BN_CLICKED(IDC_CMD_TP_ITEM_BTN_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_TP_ITEM_BTN_CALC, OnCmdTpItemBtnCalc)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL, OnSelchangeCmdTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Outside call
void CCMThkPageItemDlg::SetModifyData(T_THIK_K Key, T_THIK_D &Data)
{
	m_Key = Key;
	m_data = Data;
	m_bModify = TRUE;
}

void CCMThkPageItemDlg::SetThicknessID(UINT key)
{
	m_bWorkTab = TRUE;
	m_FixID = key;
}


/////////////////////////////////////////////////////////////////////////////
// CCMThkPageItemDlg message handlers
BOOL CCMThkPageItemDlg::OnInitDialog() 
{
	SetRedraw(FALSE);

	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// Add TabDlg _LS(IDS_WG_CMD__ADDD__Value)
	m_pDlgValue = new CCMThkItemValueDlg(m_pDoc);
	m_wndTabCtrl.AddTab(m_pDlgValue, _LS(IDS_WG_CMD__ADDD__Value), CCMThkItemValueDlg::IDD, TRUE);

	// Add TabDlg _LS(IDS_WG_CMD__ADDD__Stiffened)
	m_pDlgStiff = new CCMThkItemStiffDlg(this);
	m_wndTabCtrl.AddTab(m_pDlgStiff, _LS(IDS_WG_CMD__ADDD__Stiffened), CCMThkItemStiffDlg::IDD, TRUE);

#if defined(_MGEN)
	if(CProduct::GetMovingType() == D_PRODUCT_MOVING_CH)
	{
	  // Add TabDlg _LS(IDS_WG_CMD__ADDD__StlWall)
	  m_pDlgStlWall = new CCMThkItemStlWallDlg(m_pDoc);
	  m_wndTabCtrl.AddTab(m_pDlgStlWall, _LS(IDS_WG_CMD__ADDD__StlWall), CCMThkItemStlWallDlg::IDD, TRUE);
	}
#endif

	// Dialog Page Manager¿¡ Place Holder ¼³Á¤
	BOOL bSet;
	CWnd* pPH = (CWnd*)GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPH && pPH->GetSafeHwnd() && IsWindow(pPH->GetSafeHwnd()));
	bSet = m_wndTabCtrl.SetPlaceHolder(pPH);
	ASSERT(bSet);

	int actTab;
	if(m_bModify)
	{
		m_pDlgValue->m_bModify = TRUE;
		m_pDlgStiff->m_bModify = TRUE;
		if(m_pDlgStlWall) m_pDlgStlWall->m_bModify = TRUE;
		SetData2Dlg();
		actTab = m_data.nType;

#if defined(_MGEN)
		if(CProduct::GetMovingType() != D_PRODUCT_MOVING_CH)
		{
			if (actTab != 0 && actTab != 1) actTab = 0;
		}
#endif
	}
	else
		actTab = 0;

	m_wndTabCtrl.ShowTab(actTab);

	if(actTab == 0) EnableDisableCalcBtn(FALSE);
	if(actTab == 2) EnableDisableCalcBtn(FALSE);
	if(m_bModify) m_wndBtnApply.EnableWindow(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkPageItemDlg::OnOK() 
{
	if(!SaveData()) return;

	CDialogMove::OnOK();
}

BOOL CCMThkPageItemDlg::SaveData()
{
	int nActTab = m_wndTabCtrl.GetCurSel();
	if(nActTab == 0)
	{
		if(!m_pDlgValue->OnApplyPublic()) return FALSE;
	}
	else  if (nActTab == 1)
	{
		m_pDlgStiff->OnApplyPublic();
	}
	else
	{
		m_pDlgStlWall->OnApplyPublic();
	}

	T_THIK_K Key;
	T_THIK_D Data;
	Data.Initialize();

	SetDlg2Data(Key, Data);

	if (m_bModify)
	{
		T_THIK_K KeyOld = m_Key;
		m_Key = Key;
		if(!m_pDoc->m_pDataCtrl->ModifyThik(KeyOld, Key, Data)) return FALSE;
	}
	else
		if(!m_pDoc->m_pDataCtrl->AddThik(Key, Data)) return FALSE;

	return TRUE;
}

void CCMThkPageItemDlg::OnApply() 
{
	if(!SaveData()) return;

	if(!m_bModify)
	{
		// new ID
		int nActTab = m_wndTabCtrl.GetCurSel();
		if(nActTab == 0)
		{
			T_THIK_K Key = m_pDlgValue->m_sDlgData.ID;
			m_pDlgValue->m_nID = m_pDoc->m_pAttrCtrl->GetNextNumThik(Key);
			m_pDlgValue->UpdateData(FALSE);
		}
		else if(nActTab == 1)
		{
			T_THIK_K Key = m_pDlgStiff->m_sDlgData.ID;
			m_pDlgStiff->m_nID = m_pDoc->m_pAttrCtrl->GetNextNumThik(Key);
			m_pDlgStiff->UpdateData(FALSE);
		}
		else
		{
			T_THIK_K Key = m_pDlgStlWall->m_sDlgData.ID;
			m_pDlgStlWall->m_nID = m_pDoc->m_pAttrCtrl->GetNextNumThik(Key);
			m_pDlgStlWall->UpdateData(FALSE);
		}
	}

	// initialize dlg data
//	m_pDlgStiff->m_sDlgData.Initialize();
//	m_pDlgValue->m_sDlgData.Initialize();
}

void CCMThkPageItemDlg::SetDlg2Data(T_THIK_K &Key, T_THIK_D &data)
{
	int nActTab = m_wndTabCtrl.GetCurSel();
	data.nType = nActTab; // 0:Value, 1:Stiffened

	// value
	if(nActTab == 0)
	{
		Key = m_pDlgValue->m_sDlgData.ID;
		data.Value.strName = m_pDlgValue->m_sDlgData.strName;
		data.Value.nThickType = m_pDlgValue->m_sDlgData.nThickType;
		data.Value.ThickIn = m_pDlgValue->m_sDlgData.ThickIn;
		data.Value.ThickOut = m_pDlgValue->m_sDlgData.ThickOut;
		data.Value.bOffset = m_pDlgValue->m_sDlgData.bOffset;
		data.Value.nOffsetType = m_pDlgValue->m_sDlgData.nOffsetType;
		data.Value.dRatio = m_pDlgValue->m_sDlgData.dRatio;
		data.Value.dValue = m_pDlgValue->m_sDlgData.dValue;
	}
	else  if(nActTab == 1)// stiffened
	{
		Key = m_pDlgStiff->m_sDlgData.ID;
		data.Stiffened.nType = m_pDlgStiff->m_sDlgData.nType;
		data.Stiffened.DBName = m_pDlgStiff->m_sDlgData.DBName;
		data.Stiffened.dPlateThik = m_pDlgStiff->m_sDlgData.dPlateThk;
		data.Stiffened.dWeightThik = m_pDlgStiff->m_sDlgData.dWeightThk;
		data.Stiffened.nRibPosition = m_pDlgStiff->m_sDlgData.nRibPos+1;

		data.Stiffened.yzValue.Shape = m_pDlgStiff->m_sDlgData.YZValue_Shape;
		data.Stiffened.yzValue.ThickIn = m_pDlgStiff->m_sDlgData.YZValue_ThickIn;
		data.Stiffened.yzValue.ThickOut = m_pDlgStiff->m_sDlgData.YZValue_ThickOut;
		data.Stiffened.yzValue.Hu = m_pDlgStiff->m_sDlgData.YZValue_Hu;
		data.Stiffened.yzValue.Hl = m_pDlgStiff->m_sDlgData.YZValue_Hl;
		data.Stiffened.xzValue.Shape = m_pDlgStiff->m_sDlgData.XZValue_Shape;
		data.Stiffened.xzValue.ThickIn = m_pDlgStiff->m_sDlgData.XZValue_ThickIn;
		data.Stiffened.xzValue.ThickOut = m_pDlgStiff->m_sDlgData.XZValue_ThickOut;
		data.Stiffened.xzValue.Hu = m_pDlgStiff->m_sDlgData.XZValue_Hu;
		data.Stiffened.xzValue.Hl = m_pDlgStiff->m_sDlgData.XZValue_Hl;

		data.Stiffened.yzSect.bRibAttached = m_pDlgStiff->m_sDlgData.YZSect_Exist; 
		data.Stiffened.yzSect.Shape = m_pDlgStiff->m_sDlgData.YZSect_Shape;
		data.Stiffened.yzSect.Dist = m_pDlgStiff->m_sDlgData.YZSect_Dist;
		data.Stiffened.yzSect.SName = m_pDlgStiff->m_sDlgData.YZSect_SName;

		const auto yzSectSize = min(CSectUtil::GetSizeCountByRegShape(data.Stiffened.yzSect.Shape), THIK_STIFFENED_USER_SIZE_COUNT);
		for (int i = 0; i < yzSectSize; i++)
			data.Stiffened.yzSect.Size[i] = m_pDlgStiff->m_sDlgData.YZSect_Size[i];

		data.Stiffened.xzSect.bRibAttached = m_pDlgStiff->m_sDlgData.XZSect_Exist; 
		data.Stiffened.xzSect.Shape = m_pDlgStiff->m_sDlgData.XZSect_Shape;
		data.Stiffened.xzSect.Dist = m_pDlgStiff->m_sDlgData.XZSect_Dist;
		data.Stiffened.xzSect.SName = m_pDlgStiff->m_sDlgData.XZSect_SName;

		const auto xzSectSize = min(CSectUtil::GetSizeCountByRegShape(data.Stiffened.xzSect.Shape), THIK_STIFFENED_USER_SIZE_COUNT);
		for (int i = 0; i < xzSectSize; i++)
			data.Stiffened.xzSect.Size[i] = m_pDlgStiff->m_sDlgData.XZSect_Size[i];
	}
	else // steel Wall
	{
		Key = m_pDlgStlWall->m_sDlgData.ID;
		data.StlWall.ThickCon = m_pDlgStlWall->m_sDlgData.ThickCon;
		data.StlWall.ThickSteel = m_pDlgStlWall->m_sDlgData.ThickSteel;
	}
}

void CCMThkPageItemDlg::SetData2Dlg()
{
	// value
	m_pDlgValue->m_sDlgData.ID = m_Key;
	m_pDlgValue->m_sDlgData.strName = m_data.Value.strName;
	m_pDlgValue->m_sDlgData.nThickType = m_data.Value.nThickType;
	m_pDlgValue->m_sDlgData.ThickIn = m_data.Value.ThickIn;
	m_pDlgValue->m_sDlgData.ThickOut = m_data.Value.ThickOut;
	m_pDlgValue->m_sDlgData.bOffset = m_data.Value.bOffset;
	m_pDlgValue->m_sDlgData.nOffsetType = m_data.Value.nOffsetType;
	m_pDlgValue->m_sDlgData.dRatio = m_data.Value.dRatio;
	m_pDlgValue->m_sDlgData.dValue = m_data.Value.dValue;

	// stiffened
	m_pDlgStiff->m_sDlgData.ID = m_Key;
	m_pDlgStiff->m_sDlgData.nType = m_data.Stiffened.nType;
	m_pDlgStiff->m_sDlgData.DBName = m_data.Stiffened.DBName;
	m_pDlgStiff->m_sDlgData.dPlateThk = m_data.Stiffened.dPlateThik;
	m_pDlgStiff->m_sDlgData.dWeightThk = m_data.Stiffened.dWeightThik;
	m_pDlgStiff->m_sDlgData.nRibPos = m_data.Stiffened.nRibPosition-1;

	if(m_data.Stiffened.yzValue.Shape.IsEmpty()) m_pDlgStiff->m_sDlgData.YZValue_Shape = D_SECT_SHAPE_REG_URIB;
	else m_pDlgStiff->m_sDlgData.YZValue_Shape = m_data.Stiffened.yzValue.Shape;
	m_pDlgStiff->m_sDlgData.YZValue_ThickIn = m_data.Stiffened.yzValue.ThickIn;
	m_pDlgStiff->m_sDlgData.YZValue_ThickOut = m_data.Stiffened.yzValue.ThickOut;
	m_pDlgStiff->m_sDlgData.YZValue_Hu = m_data.Stiffened.yzValue.Hu;
	m_pDlgStiff->m_sDlgData.YZValue_Hl = m_data.Stiffened.yzValue.Hl;
	if(m_data.Stiffened.xzValue.Shape.IsEmpty()) m_pDlgStiff->m_sDlgData.XZValue_Shape = D_SECT_SHAPE_REG_URIB;
	else m_pDlgStiff->m_sDlgData.XZValue_Shape = m_data.Stiffened.xzValue.Shape;
	m_pDlgStiff->m_sDlgData.XZValue_ThickIn = m_data.Stiffened.xzValue.ThickIn;
	m_pDlgStiff->m_sDlgData.XZValue_ThickOut = m_data.Stiffened.xzValue.ThickOut;
	m_pDlgStiff->m_sDlgData.XZValue_Hu = m_data.Stiffened.xzValue.Hu;
	m_pDlgStiff->m_sDlgData.XZValue_Hl = m_data.Stiffened.xzValue.Hl;

	m_pDlgStiff->m_sDlgData.YZSect_Exist = m_data.Stiffened.yzSect.bRibAttached;
	if(m_data.Stiffened.yzSect.Shape.IsEmpty()) m_pDlgStiff->m_sDlgData.YZSect_Shape = D_SECT_SHAPE_REG_URIB;
	else m_pDlgStiff->m_sDlgData.YZSect_Shape = m_data.Stiffened.yzSect.Shape;
	m_pDlgStiff->m_sDlgData.YZSect_Dist = m_data.Stiffened.yzSect.Dist;
	m_pDlgStiff->m_sDlgData.YZSect_SName = m_data.Stiffened.yzSect.SName;

	const auto yzSectSize = CSectUtil::GetSizeCountByRegShape(m_data.Stiffened.yzSect.Shape);
	for (int i = 0; i < yzSectSize; i++)
		m_pDlgStiff->m_sDlgData.YZSect_Size[i] = m_data.Stiffened.yzSect.Size[i];

	m_pDlgStiff->m_sDlgData.XZSect_Exist = m_data.Stiffened.xzSect.bRibAttached;
	if(m_data.Stiffened.xzSect.Shape.IsEmpty()) m_pDlgStiff->m_sDlgData.XZSect_Shape = D_SECT_SHAPE_REG_URIB;
	else m_pDlgStiff->m_sDlgData.XZSect_Shape = m_data.Stiffened.xzSect.Shape;
	m_pDlgStiff->m_sDlgData.XZSect_Dist = m_data.Stiffened.xzSect.Dist;
	m_pDlgStiff->m_sDlgData.XZSect_SName = m_data.Stiffened.xzSect.SName;

	const auto xzSectSize = CSectUtil::GetSizeCountByRegShape(m_data.Stiffened.xzSect.Shape);
	for (int i = 0; i < xzSectSize; i++)
		m_pDlgStiff->m_sDlgData.XZSect_Size[i] = m_data.Stiffened.xzSect.Size[i];

	//Steel Wall
	if(m_pDlgStlWall) m_pDlgStlWall->m_sDlgData.ID = m_Key;
	if(m_pDlgStlWall) m_pDlgStlWall->m_sDlgData.ThickCon = m_data.StlWall.ThickCon;
	if(m_pDlgStlWall) m_pDlgStlWall->m_sDlgData.ThickSteel = m_data.StlWall.ThickSteel;
}

void CCMThkPageItemDlg::EnableDisableCalcBtn(BOOL flg)
{
	CButton* bt = (CButton*)GetDlgItem(IDC_CMD_TP_ITEM_BTN_CALC);
	bt->EnableWindow(flg);
}

void CCMThkPageItemDlg::OnCmdTpItemBtnCalc() 
{
	// TODO: Add your control notification handler code here
	T_THIK_D thik;
	thik.Initialize();

	if(!m_pDlgStiff->GetCurCalcData(thik))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Input_Data___));
		return;
	}

	CCMThkItemCalcDlg dlg(m_pDoc, &thik, this);
	dlg.DoModal();
}

void CCMThkPageItemDlg::OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	int nActTab = m_wndTabCtrl.GetCurSel();
	if(nActTab == 0) EnableDisableCalcBtn(FALSE);
	if(nActTab == 2) EnableDisableCalcBtn(FALSE);
	*pResult = 0;
}
