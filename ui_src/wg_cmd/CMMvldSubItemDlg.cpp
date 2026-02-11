// CMMvldSubItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldSubItemDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMMvhcListDlg.h"
#include "CMMvldItemDlg.h"
#include "CMMvldItemEuroBSDlg.h"
#include "CMMvldItemEuroBSNewDlg.h"
#include "CMMvldItemRusDlg.h"
#include "CMMvldItemKRLRFD11Dlg.h"
#include "CMMvldItemSouthAfricaDlg.h"
#include "CMMvldItemPolandDlg.h"
#include "CMMvldItemAustraliaDlg.h"
#include "CMMvldItemNewZealandDlg.h"
#include "CMMvldItemBrazilDlg.h"

#include "..\wg_base\TestEnvMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemDlg dialog


CCMMvldSubItemDlg::CCMMvldSubItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldSubItemDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
	m_nLoadModel = 0;
	m_nPos = 0;
	ASSERT(pParent);
	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData)) ASSERT(0);
	if(mvcdData.nCodeType == D_MOVE_CODE_EURO_BS)
		m_pMvldItemEuroBSNewDlg = (CCMMvldItemEuroBSNewDlg*)pParent;
	else if(mvcdData.nCodeType == D_MOVE_CODE_RUSSIA)
		m_pMvldItemRusDlg = (CCMMvldItemRusDlg*)pParent;
	else if(mvcdData.nCodeType == D_MOVE_CODE_KOREA_LRFD_2011)
		m_pMvldItemKRLRFDDlg = (CCMMvldItemKRLRFD11Dlg*)pParent;
	else if(mvcdData.nCodeType == D_MOVE_CODE_SOUTH_AFRICA)
		m_pMvldItemSouthAfricaDlg = (CCMMvldItemSouthAfricaDlg*)pParent;
	else if(mvcdData.nCodeType == D_MOVE_CODE_POLAND)
		m_pMvldItemPolandDlg = (CCMMvldItemPolandDlg*)pParent;
	else if(mvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA)
		m_pMvldItemAustraliaDlg = (CCMMvldItemAustraliaDlg*)pParent;
	else if (mvcdData.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		m_pMvldItemDlg = (CCMMvldItemDlg*)pParent;
	else if (mvcdData.nCodeType == D_MOVE_CODE_NEWZEALAND)
		m_pMvldItemNewZealandDlg = (CCMMvldItemNewZealandDlg*)pParent;
	else if (mvcdData.nCodeType == D_MOVE_CODE_BRAZIL)
		m_pMvldItemBrazilDlg = (CCMMvldItemBrazilDlg*)pParent;
	else
		m_pMvldItemDlg = (CCMMvldItemDlg*)pParent;
	
	
	//{{AFX_DATA_INIT(CCMMvldSubItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvldSubItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldSubItemDlg)
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_wndSelList);
	DDX_Control(pDX, IDC_CMD_MAX_NUM, m_wndMaxNum);
	DDX_Control(pDX, IDC_CMD_MIN_NUM, m_wndMinNum);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_VCLASS, m_wndVClass);
	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldSubItemDlg::Data2Dlg()
{
	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if (m_Data.nVehicleType == 1)
	{
		m_pDoc->m_pAttrCtrl->GetMvhc(m_Data.VehicleKey, mvhc);
		m_wndVClass.SelectString(-1, _T("VC:")+mvhc.VehicleClassName);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(m_Data.VehicleKey, mvhl);
		m_wndVClass.SelectString(-1, _T("VL:")+mvhl.VehicleLoadName);
	}

	CString csVal;
	csVal.Format(_T("%g"), m_Data.dVehicleScaleFactor);
	m_wndScaleFactor.SetWindowText(csVal);

	csVal.Format(_T("%d"), m_Data.nMinLoadedLanes);
	m_wndMinNum.SetWindowText(csVal);

	csVal.Format(_T("%d"), m_Data.nMaxLoadedLanes);
	m_wndMaxNum.SetWindowText(csVal);

	// Select 된 것, 아닌 것 구분
	int nAllSize = m_aLaneList.GetSize();
	int nSelSize;
	int nMaxCount = m_Data.aSelectedLanes.GetSize();

	for (nSelSize = 0; nSelSize < nMaxCount; nSelSize++)
		if (m_Data.aSelectedLanes[nSelSize] == 0) break;

	m_aSelFlag.SetSize(nAllSize);
	for (int i = 0; i < nAllSize; i++)
	{
		int j = 0;
		for (j = 0; j < nSelSize; j++)
			if (m_aLaneList[i] == m_Data.aSelectedLanes[j]) break;
		if (j == nSelSize) m_aSelFlag[i] = FALSE;
		else m_aSelFlag[i] = TRUE;
	}

	int nIndex;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

BOOL CCMMvldSubItemDlg::Dlg2Data()
{
	m_Data.Initialize();
	int nCurSel = m_wndVClass.GetCurSel();
	if (nCurSel < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_Class_is_not_sele));
		return FALSE;
	}
	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_wndVClass.GetItemData(nCurSel);
	m_Data.nVehicleType = TypeKey.typekey.type + 1;
	m_Data.VehicleKey = TypeKey.typekey.key;
	CFormulaEdit::GetEditValue(&m_wndScaleFactor, m_Data.dVehicleScaleFactor);
	CFormulaEdit::GetEditValue(&m_wndMinNum, m_Data.nMinLoadedLanes);
	CFormulaEdit::GetEditValue(&m_wndMaxNum, m_Data.nMaxLoadedLanes);
	
	if(!ErrorCheck()) return FALSE;

	return TRUE;
}

BOOL CCMMvldSubItemDlg::ErrorCheck()
{
	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData))ASSERT(0);

	int nSize = m_aLaneList.GetSize();
	int nCount = 0;
	// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
	int nMaxLane = 20;
	for (int i=0; i<nSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			if (nCount >= nMaxLane)
			{
				CString csMsg;
				csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
				AfxMessageBox(csMsg);
				return FALSE;
			}
			m_Data.aSelectedLanes.Add(m_aLaneList[i]);
			nCount++;
		}
	}

	return TRUE;
}

BOOL CCMMvldSubItemDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData))ASSERT(0);
	if(mvcdData.nCodeType == D_MOVE_CODE_EURO_BS)
		return m_pMvldItemEuroBSNewDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if(mvcdData.nCodeType == D_MOVE_CODE_RUSSIA)
		return m_pMvldItemRusDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if(mvcdData.nCodeType == D_MOVE_CODE_KOREA_LRFD_2011)
		return m_pMvldItemKRLRFDDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if(mvcdData.nCodeType == D_MOVE_CODE_SOUTH_AFRICA)
		return m_pMvldItemSouthAfricaDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if(mvcdData.nCodeType == D_MOVE_CODE_POLAND)
		return m_pMvldItemPolandDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if(mvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA)
		return m_pMvldItemAustraliaDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if (mvcdData.nCodeType == D_MOVE_CODE_NEWZEALAND)
		return m_pMvldItemNewZealandDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else if (mvcdData.nCodeType == D_MOVE_CODE_BRAZIL)
		return m_pMvldItemBrazilDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	else
		return m_pMvldItemDlg->OnApplySubDlg(m_bModify, m_Data, m_nPos);
	
}

void CCMMvldSubItemDlg::FillVClassList()
{
	int nIndex;
	T_MVLD_TYPE_KEY TypeKey;
	T_MVHC_D mvhc;
	T_MVHL_D mvhl;

	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData))mvcdData.Initialize();
	
	CArray<T_MVHC_K, T_MVHC_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetMvhcKeyList(aKeyList);
	int nSize = aKeyList.GetSize();
	m_wndVClass.ResetContent();
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhc(aKeyList[i], mvhc);

		if(mvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA)
		{
			if(mvhc.arVehicleLoad.GetSize() == 0) continue;
			if(!m_pDoc->m_pAttrCtrl->GetMvhl(mvhc.arVehicleLoad[0], mvhl)) continue; // Vehicle Class 에서 Check 했으니까 한개만 Check

			if(m_nLoadModel == 0)
			{
				if(mvhl.bFatigue) continue;

				if(mvhl.bStandard)
				{
					if(mvhl.VehicleTypeName==_T("150LA")  || mvhl.VehicleTypeName==_T("300LA")) continue;
					if(mvhl.VehicleTypeName==_T("HLP320") || mvhl.VehicleTypeName==_T("HLP400")) continue;
				}
				else
				{
					if(mvhl.nLoadType==3) continue;  //User - 150LA / 300LA
				}
			}
			else if(m_nLoadModel == 1)
			{
				if(!mvhl.bFatigue) continue;
			}
			else if(m_nLoadModel == 2) // Heavy Load
			{
				ASSERT(0); continue; // subload case에 안들어온다.
			}
			else if(m_nLoadModel == 3) // Rail
			{
				if(mvhl.bStandard)
				{
					if(mvhl.VehicleTypeName!=_T("150LA") && mvhl.VehicleTypeName!=_T("300LA")) continue;
				}
				else
				{
					if(mvhl.nLoadType!=3) continue;  //User - 150LA / 300LA
				}
			}
			else
			{
				ASSERT(0); continue; 
			}

			nIndex = m_wndVClass.AddString(_T("VC:")+mvhc.VehicleClassName);
			TypeKey.typekey.type = 0;
			TypeKey.typekey.key = aKeyList[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
		else
		{
			nIndex = m_wndVClass.AddString(_T("VC:")+mvhc.VehicleClassName);
			TypeKey.typekey.type = 0;
			TypeKey.typekey.key = aKeyList[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
	}

	CArray<T_MVHL_K, T_MVHL_K> aKeyListMvhl;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aKeyListMvhl);
	nSize = aKeyListMvhl.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(aKeyListMvhl[i], mvhl);
		if(mvcdData.nCodeType == D_MOVE_CODE_EURO_BS)
		{
			if(mvhl.bStandard)
			{
				if(mvhl.nStandardCode == D_MVHL_EURO_EN1991_2_2003_ROAD_BRG)
				{
					if(m_nLoadModel==4) continue;

					if(mvhl.VehicleTypeName == _T("Load Model 1"))
						continue;
				}
				else if(mvhl.nStandardCode == D_MVHL_EURO_EN1991_2_2003_FOOTWAY)
				{
					if(m_nLoadModel==4) continue;
				}
				else if(mvhl.nStandardCode == D_MVHL_EURO_EN1991_2_2003_ROAD_BRG_FTG)
				{
					if(m_nLoadModel==4) continue;

					if(mvhl.VehicleTypeName==_T("Fatigue Load Model 1"))
						continue;
				}
				else if(mvhl.nStandardCode == D_MVHL_EURO_EN1991_2_2003_RAIL)
				{
					if(m_nLoadModel!=4)
					{
						if(mvhl.VehicleTypeName==_T("Load Model 71") || 
							 mvhl.VehicleTypeName==_T("Load Model SW/0") || 
							 mvhl.VehicleTypeName==_T("Load Model SW/2") || 
							 mvhl.VehicleTypeName==_T("HSLM A1 ~ HSLM A10") || 
							 mvhl.VehicleTypeName==_T("HSLM B") || 
							 mvhl.VehicleTypeName==_T("Unloaded Train"))
							continue;
					}
				}
			}
			else
			{
				if(m_nLoadModel==4)
				{
					if(mvhl.nSubType < 5 || mvhl.nSubType > 11)
					  continue;
				}
				else
				{
					if(mvhl.nSubType ==0 || mvhl.nSubType > 5)
						continue;
				}
			}
			nIndex = m_wndVClass.AddString(_T("VL:")+mvhl.VehicleLoadName);
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aKeyListMvhl[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
		else if(mvcdData.nCodeType == D_MOVE_CODE_POLAND)
		{
			if(mvhl.bStandard)
			{
				if( mvhl.VehicleTypeName == _T("Vehicle K")       ||
					  mvhl.VehicleTypeName == _T("Tracked Vehicle") ||
					  mvhl.VehicleTypeName == _T("Wheeled Vehicle")) continue;
			}
			else
			{
				if(mvhl.nLoadType == 0 && mvhl.nSubType == 0) continue;
				if(mvhl.nLoadType == 1) continue;
			}

			nIndex = m_wndVClass.AddString(_T("VL:")+mvhl.VehicleLoadName);
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aKeyListMvhl[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
		else if(mvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA)
		{
			if (m_pDoc->m_pAttrCtrl->IsBDoubleVehicle(mvhl)) continue;

			if(m_nLoadModel == 0) // General
			{
				if(mvhl.bFatigue) continue;
				if(mvhl.bStandard)
				{
					if(mvhl.VehicleTypeName==_T("150LA")  || mvhl.VehicleTypeName==_T("300LA")) continue;
					if(mvhl.VehicleTypeName==_T("HLP320") || mvhl.VehicleTypeName==_T("HLP400")) continue;

					if (mvhl.VehicleTypeName == _T("WA-Group2-Vehicle1 (3.01m O/A)") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle1 (3.70m O/A)") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle2 (3.01m O/A)") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle2 (3.70m O/A)") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle4") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle4 (Non-Supervised)") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle5") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle5 (Non-Supervised)") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle7") ||
							mvhl.VehicleTypeName == _T("WA-Group2-Vehicle8")) continue;
				}
				else
				{
					if(mvhl.nLoadType==3) continue;  //User - 150LA / 300LA
				}
			}
			else if(m_nLoadModel == 1) // Fatigue 
			{
				if(!mvhl.bFatigue) continue;
			}
			else if(m_nLoadModel == 2) // Heavy Load
			{
				ASSERT(0); continue; // subload case에 안들어온다.
			}
			else if(m_nLoadModel == 3) // Rail
			{
				if(mvhl.bStandard)
				{
					if(mvhl.VehicleTypeName!=_T("150LA") && mvhl.VehicleTypeName!=_T("300LA")) continue;
				}
				else
				{
					if(mvhl.nLoadType!=3) continue;  //User - 150LA / 300LA
				}
			}
			else
			{
				ASSERT(0); continue; 
			}


			nIndex = m_wndVClass.AddString(_T("VL:")+mvhl.VehicleLoadName);
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aKeyListMvhl[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
		else if (mvcdData.nCodeType == D_MOVE_CODE_NEWZEALAND)
		{
			if (mvhl.bStandard)
			{
				if ((mvhl.VehicleTypeName == _T("HO (overload) loading (a)") || mvhl.VehicleTypeName == _T("HO (overload) loading (b)")) && (m_nLoadModel != 0 && m_nLoadModel != 2)) continue;
				if (m_nLoadModel == 1 && mvhl.nStandardCode != D_MVHL_TRAFFIC_LOAD_NZ) continue;
				else if (m_nLoadModel == 2)
				{
					if (mvhl.VehicleTypeName != _T("HO (overload) loading (a)") && mvhl.VehicleTypeName != _T("HO (overload) loading (b)") &&
						mvhl.nStandardCode != D_MVHL_GENERAL_ACCESS_NZ && mvhl.nStandardCode != D_MVHL_50MAX_NZ && 
						mvhl.nStandardCode != D_MVHL_HPMV_NZ && mvhl.nStandardCode != D_MVHL_DECK_EVALUATION)
					{
						continue;
					}
				}
				else if (m_nLoadModel == 3 && mvhl.nStandardCode != D_MVHL_FATIGUE_NZ) continue;
			}

			nIndex = m_wndVClass.AddString(_T("VL:") + mvhl.VehicleLoadName);
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aKeyListMvhl[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
		else if (mvcdData.nCodeType == D_MOVE_CODE_BRAZIL)
		{
			if (mvhl.nStandardCode == D_MVHL_NBR7189_RAILWAY)
			{
				nIndex = m_wndVClass.AddString(_T("VL:") + mvhl.VehicleLoadName);
				TypeKey.typekey.type = 1;
				TypeKey.typekey.key = aKeyListMvhl[i];
				m_wndVClass.SetItemData(nIndex, TypeKey.value);
			}
		}
		else
		{
			//if (mvhl.nStandardCode == 9 && mvhl.nLoadType == 2) continue; // AASHTO Permit Load는 제외
			nIndex = m_wndVClass.AddString(_T("VL:")+mvhl.VehicleLoadName);
			TypeKey.typekey.type = 1;
			TypeKey.typekey.key = aKeyListMvhl[i];
			m_wndVClass.SetItemData(nIndex, TypeKey.value);
		}
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndVClass);
	m_wndVClass.SetCurSel(0);
}

CString CCMMvldSubItemDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_MVCD_D mvcdData;
		if (!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData))mvcdData.Initialize();
		if (mvcdData.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
		{
			T_LLANjp_D llan;
			m_pDoc->m_pAttrCtrl->GetLlanjp(nLaneKey, llan);
			return llan.LineLaneName;
		}
		else
		{
			T_LLAN_D llan;
			m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, llan);
			return llan.LineLaneName;
		}
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}



BEGIN_MESSAGE_MAP(CCMMvldSubItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldSubItemDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_LBN_DBLCLK(IDC_CMD_SEL_LIST, OnDblclkCmdSelList)
	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdUnselList)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS, OnCmdBtnVclass)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemDlg message handlers

BOOL CCMMvldSubItemDlg::OnInitDialog() 
{
	
	CDialogMove::OnInitDialog();
	
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0) m_bSurface = TRUE;
	else m_bSurface = FALSE;

	// TODO: Add extra initialization here
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.dVehicleScaleFactor = 1.0;
	}
	
	CArray<UINT,UINT> arScale,arMinMax;
	arScale.Add(IDC_CMD_SCALE_FACTOR_TXT);
	arScale.Add(IDC_CMD_SCALE_FACTOR);

	arMinMax.Add(IDC_CMD_MIN_NUM_TXT);
	arMinMax.Add(IDC_CMD_MIN_NUM);
	arMinMax.Add(IDC_CMD_MAX_NUM_TXT);
	arMinMax.Add(IDC_CMD_MAX_NUM);
	
 
	T_MVCD_D mvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData)) ASSERT(0);
	if(mvcdData.nCodeType == D_MOVE_CODE_EURO_BS)
	{
		if(m_pMvldItemEuroBSNewDlg->m_Data.nLoadModel == 4)
		{
			CDlgUtil::CtrlShowHide(this,arScale, TRUE);
		}
		else
		{
			CRect rRef;
			CRect rToMove;
			int nDistY;
			GetDlgItem(IDC_CMD_SCALE_FACTOR_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MIN_NUM_TXT)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, arMinMax,  nDistY);
			CDlgUtil::CtrlMoveDistY(this, arScale, -nDistY);
			CDlgUtil::CtrlShowHide(this,arScale, FALSE);
		}
	}
	if (m_bSurface) m_pDoc->m_pAttrCtrl->GetSlanKeyList(m_aLaneList);
	else
	{
		if (mvcdData.nCodeType == D_MOVE_CODE_JAPAN_RAIL)
			m_pDoc->m_pAttrCtrl->GetLlanjpKeyList(m_aLaneList);
		else
			m_pDoc->m_pAttrCtrl->GetLlanKeyList(m_aLaneList);
	}	
	FillVClassList();

	Data2Dlg();
	EnableCtrl();
	// EuroCode 일 경우 Text 이름 변경
	if(mvcdData.nCodeType == D_MOVE_CODE_EURO_BS)
	{
		GetDlgItem(IDC_CMD_VCLASS_STC)->SetWindowText(_LS(IDS_CMD_MVLD_BS_VEHICLE));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldSubItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldSubItemDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}
	
	CString csLaneName;
	m_wndSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}



	T_MVCD_D mvcdData;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData)) ASSERT(0);
	if (mvcdData.nCodeType == D_MOVE_CODE_NEWZEALAND && m_nLoadModel == 3)
	{
		m_Data.nMaxLoadedLanes = min(m_wndSelList.GetCount(),2);
		CString csVal;
		csVal.Format(_T("%d"), m_Data.nMaxLoadedLanes);
		m_wndMaxNum.SetWindowText(csVal);
	}
}

void CCMMvldSubItemDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}

	T_MVCD_D mvcdData;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData)) ASSERT(0);
	if (mvcdData.nCodeType == D_MOVE_CODE_NEWZEALAND && m_nLoadModel == 3)
	{
		m_Data.nMaxLoadedLanes = min(m_wndSelList.GetCount(), 2);
		CString csVal;
		csVal.Format(_T("%d"), m_Data.nMaxLoadedLanes);
		m_wndMaxNum.SetWindowText(csVal);
	}
}

void CCMMvldSubItemDlg::EnableCtrl()
{
	T_MVCD_D mvcdData;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdData)) ASSERT(0);
	if (mvcdData.nCodeType != D_MOVE_CODE_NEWZEALAND || m_nLoadModel != 3) return;

	m_Data.nMaxLoadedLanes = min(m_wndSelList.GetCount(), 2);
	CString csVal;
	csVal.Format(_T("%d"), m_Data.nMaxLoadedLanes);
	m_wndMaxNum.SetWindowText(csVal);
	GetDlgItem(IDC_CMD_MAX_NUM)->EnableWindow(FALSE);

}

void CCMMvldSubItemDlg::OnDblclkCmdSelList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete();
}

void CCMMvldSubItemDlg::OnDblclkCmdUnselList() 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();
}

void CCMMvldSubItemDlg::OnCmdBtnVclass() 
{
	// TODO: Add your control notification handler code here
	CCMMvhcListDlg dlg;
	dlg.SetModeAddOnly();
	dlg.DoModal();
	T_MVLD_TYPE_KEY TypeKey;
	int nCurSel = m_wndVClass.GetCurSel();
	if (nCurSel >= 0) 
	TypeKey.value = m_wndVClass.GetItemData(nCurSel);
	FillVClassList();
	CString csName;
	if (nCurSel >= 0) // 예전 선택 복구
	{
		if (TypeKey.typekey.type == 0)  // mvhc
		{
			T_MVHC_D mvhc;
			if (m_pDoc->m_pAttrCtrl->GetMvhc(TypeKey.typekey.key, mvhc))
				csName = _T("VC:")+mvhc.VehicleClassName;
			else csName = _T("");
		}
		else
		{
			T_MVHL_D mvhl;
			if (m_pDoc->m_pAttrCtrl->GetMvhl(TypeKey.typekey.key, mvhl))
				csName = _T("VL:")+mvhl.VehicleLoadName;
			else csName = _T("");
		}
	}
	else csName = _T("");
	if (csName == _T("")) m_wndVClass.SetCurSel(0);
	else m_wndVClass.SelectString(-1, csName);
}

void CCMMvldSubItemDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	ApplyOrOK();
}
