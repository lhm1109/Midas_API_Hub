// BldcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "BldcDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMLoadCaseDlg.h"
#include "ExportFunc.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBldcDlg dialog


CBldcDlg::CBldcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CBldcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBldcDlg)
	m_bStoryCenter = FALSE;
	m_nCentOpt = -1;
	m_bResponse = FALSE;
	m_nStorCenterMethod = 0;
	m_bStorDriftByMaxVertElem = FALSE;
	//}}AFX_DATA_INIT

	aAxialGroup.RemoveAll();
	aShearGroup.RemoveAll();
	
	aAxialGroup.Add(IDC_CMD_STATIC1);
	aAxialGroup.Add(IDC_CMD_STATIC2);
	aAxialGroup.Add(IDC_CMD_LC_COMBO);
	aAxialGroup.Add(IDC_CMD_LC_BUTTON);
	aAxialGroup.Add(IDC_CMD_SCALE_EDIT);
	aAxialGroup.Add(IDC_CMD_BTN_ADD);
	aAxialGroup.Add(IDC_CMD_BTN_MODIFY);
	aAxialGroup.Add(IDC_CMD_BTN_DELETE);
	aAxialGroup.Add(IDC_CMD_LIST);

	aShearGroup.Add(IDC_CMD_STATIC3);
	aShearGroup.Add(IDC_CMD_STATIC4);
	aShearGroup.Add(IDC_CMD_X_LC_COMBO);
	aShearGroup.Add(IDC_CMD_Y_LC_COMBO);
	aShearGroup.Add(IDC_CMD_X_LC_BUTTON);
	aShearGroup.Add(IDC_CMD_Y_LC_BUTTON);
	
	aStiffGroup.Add(IDC_CMD_STIF_CENTER_FRM);
	aStiffGroup.Add(IDC_CMD_STIF_X_STATIC);
	aStiffGroup.Add(IDC_CMD_STIF_Y_STATIC);
	aStiffGroup.Add(IDC_CMD_STIF_X_LC_COMBO);
	aStiffGroup.Add(IDC_CMD_STIF_Y_LC_COMBO);
	aStiffGroup.Add(IDC_CMD_STIF_X_LC_BTN);
	aStiffGroup.Add(IDC_CMD_STIF_Y_LC_BTN);  

	aJUDGroup.Add(IDC_CMD_MAIN_FRM);
	aJUDGroup.Add(IDC_CMD_STIF_CENTER_FRM);
	aJUDGroup.Add(IDC_CMD_STOR_CENT_CHECK);
	aJUDGroup.Add(IDC_CMD_STORY_FRM);
	aJUDGroup.Add(IDC_CMD_STOR_CENTER_FRM);
	aJUDGroup.Add(IDC_CMD_USE_MASS_RADIO);
	aJUDGroup.Add(IDC_CMD_USE_AXIAL_RADIO);
	aJUDGroup.Add(IDC_CMD_USE_SHEAR_RADIO);
	aJUDGroup.Add(IDC_CMD_STATIC1);
	aJUDGroup.Add(IDC_CMD_LC_COMBO);
	aJUDGroup.Add(IDC_CMD_LC_BUTTON);
	aJUDGroup.Add(IDC_CMD_STATIC2);
	aJUDGroup.Add(IDC_CMD_SCALE_EDIT);
	aJUDGroup.Add(IDC_CMD_LIST);
	aJUDGroup.Add(IDC_CMD_BTN_ADD);
	aJUDGroup.Add(IDC_CMD_BTN_MODIFY);
	aJUDGroup.Add(IDC_CMD_BTN_DELETE);
	aJUDGroup.Add(IDC_CMD_STIF_X_STATIC);
	aJUDGroup.Add(IDC_CMD_STIF_Y_STATIC);
	aJUDGroup.Add(IDC_CMD_STIF_X_LC_COMBO);
	aJUDGroup.Add(IDC_CMD_STIF_Y_LC_COMBO);
	aJUDGroup.Add(IDC_CMD_STIF_X_LC_BTN);
	aJUDGroup.Add(IDC_CMD_STIF_Y_LC_BTN);
	aJUDGroup.Add(IDC_CMD_STATIC3);
	aJUDGroup.Add(IDC_CMD_STATIC4);
	aJUDGroup.Add(IDC_CMD_X_LC_COMBO);
	aJUDGroup.Add(IDC_CMD_Y_LC_COMBO);
	aJUDGroup.Add(IDC_CMD_X_LC_BUTTON);
	aJUDGroup.Add(IDC_CMD_Y_LC_BUTTON);
}


void CBldcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBldcDlg)  
	DDX_Control(pDX, IDC_CMD_STIF_X_LC_COMBO, m_wndXStifCombo);
	DDX_Control(pDX, IDC_CMD_STIF_Y_LC_COMBO, m_wndYStifCombo);
	DDX_Control(pDX, IDC_CMD_Y_LC_COMBO, m_wndYLcCombo);
	DDX_Control(pDX, IDC_CMD_X_LC_COMBO, m_wndXLcCombo);
	DDX_Control(pDX, IDC_ETC_BLDC_SHEAR_FLAG, m_wndShearFlag);
	DDX_Control(pDX, IDC_ETC_BLDC_WIND_SEIS_ON_FLEXIBLE_CHK, m_chkWindSeisOnFlexible);
	DDX_Control(pDX, IDC_CMD_SCALE_EDIT, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_LC_COMBO, m_wndLcCombo);
	DDX_Control(pDX, IDC_ETC_BLDC_USE_FLAG, m_wndUseFlag);
	DDX_Control(pDX, IDC_ETC_BLDC_USE_UNDER_SEIS_LOADS_CHK, m_wndUseBedRockFlag);
	DDX_Control(pDX, IDC_ETC_BLDC_MASS_FLAG, m_wndMassFlag);
	DDX_Control(pDX, IDC_ETC_BLDC_BLEVEL_UNIT, m_wndBLevelUnit);
	DDX_Control(pDX, IDC_ETC_BLDC_BLEVEL_UNIT2, m_wndBLevelUnit2);
	DDX_Control(pDX, IDC_ETC_BLDC_BLEVEL_EDIT, m_wndBLevelEdit);
	DDX_Control(pDX, IDC_ETC_BLDC_UNDER_SEIS_LOADS_EDIT, m_wndBedRockLevelEdit);
	DDX_Check  (pDX, IDC_CMD_STOR_CENT_CHECK, m_bStoryCenter);
	DDX_Radio  (pDX, IDC_CMD_USE_MASS_RADIO, m_nCentOpt);
	DDX_Check  (pDX, IDC_CMD_RESPONSE_CHK, m_bResponse);
	DDX_Radio  (pDX, IDC_CMD_STOR_CENTER_RDO, m_nStorCenterMethod);
	DDX_Check  (pDX, IDC_CMD_STOR_MAX_VERT_ELEM_CHK, m_bStorDriftByMaxVertElem);
	//DDX_Control(pDX, IDC_ETC_BLDC_STOR_MDUL, m_wndConsiderStorMdul);
	//DDX_Check(pDX, IDC_ETC_BLDC_STOR_MDUL, m_bConsiderStorMdul);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CBldcDlg implementations

void CBldcDlg::Data2DlgVar()
{
	CString csBLevel;
	m_wndUseFlag.SetCheck(m_Data.bUseBaseLevel ? 1 : 0);
	m_wndUseBedRockFlag.SetCheck(m_Data.bUseBedrockLevel ? 1 : 0);
	csBLevel.Format(_T("%g"), m_Data.dBaseLevel);
	m_wndBLevelEdit.SetWindowText(csBLevel);
	m_wndBLevelEdit.EnableWindow(m_Data.bUseBaseLevel);
	csBLevel.Format(_T("%g"), m_Data.dBedrockLevel);
	m_wndBedRockLevelEdit.SetWindowText(csBLevel);
	m_wndBedRockLevelEdit.EnableWindow(m_Data.bUseBedrockLevel);
	m_wndMassFlag.SetCheck(m_Data.bBaseMass?1:0);
	m_wndShearFlag.SetCheck(m_Data.bStorShearRatio?1:0);
	//m_wndConsiderStorMdul.SetCheck(m_Data.bConsiderStorMdul?1:0);
	m_chkWindSeisOnFlexible.SetCheck(m_Data.bConsiderWindSeisForFlex?1:0);
	
	m_bStoryCenter = m_Data.bStoryCenter;
	m_nCentOpt     = m_Data.nUseOption;
	if(m_Data.nUseOption == 1) // (0) Use Mass (1) Use Static Load Case 
	{
		this -> m_wndLcCombo.ChangeSelect(D_LOADCASE_STATIC,m_Data.BldcCase[m_Data.nNumBldcCase].LoadCaseKey);
	}
	else if(m_Data.nUseOption == 2)
	{
		this->m_wndXLcCombo.ChangeSelect(D_LOADCASE_STATIC, m_Data.LoadCaseKeyX);
		this->m_wndYLcCombo.ChangeSelect(D_LOADCASE_STATIC, m_Data.LoadCaseKeyY);
	}
	this->m_wndXStifCombo.ChangeSelect(D_LOADCASE_STATIC, m_Data.StsfStldKeyX);
	this->m_wndYStifCombo.ChangeSelect(D_LOADCASE_STATIC, m_Data.StsfStldKeyY);

	m_bResponse = m_Data.bStorRespThrs;
	m_nStorCenterMethod = m_Data.nStorCenterMethod;

	m_bStorDriftByMaxVertElem = FALSE;
	if (m_nStorCenterMethod == 2)
	{
		m_nStorCenterMethod = 1;
		m_bStorDriftByMaxVertElem = TRUE;
	}
}

BOOL CBldcDlg::DlgVar2Data()
{
	m_Data.bUseBaseLevel = m_wndUseFlag.GetCheck() ? TRUE : FALSE;
	if (m_Data.bUseBaseLevel)
		m_Data.dBaseLevel = m_wndBLevelEdit.GetEditValue();
	else m_Data.dBaseLevel = 0.;

	m_Data.bUseBedrockLevel = (m_wndUseBedRockFlag.GetCheck() && m_Data.bUseBaseLevel) ? TRUE : FALSE;
	if (m_Data.bUseBedrockLevel)
	{
		double dBedrockLevel = m_wndBedRockLevelEdit.GetEditValue();

		if (m_Data.dBaseLevel <= dBedrockLevel)
		{
			AfxMessageBox(_LS(IDS_CMD_BLDC_BEDROCK_ERROR));
			return FALSE;
		}
		m_Data.dBedrockLevel = dBedrockLevel;
	}
	else
	{
		m_Data.dBedrockLevel = 0.;
	}

	m_Data.bBaseMass = m_wndMassFlag.GetCheck() ? TRUE : FALSE;
	m_Data.bStorShearRatio          = m_wndShearFlag.GetCheck()          ? TRUE : FALSE;
	//m_Data.bConsiderStorMdul        = m_wndConsiderStorMdul.GetCheck()   ? TRUE : FALSE;
	m_Data.bConsiderWindSeisForFlex = m_chkWindSeisOnFlexible.GetCheck() ? TRUE : FALSE;

	m_Data.bStoryCenter = this->m_bStoryCenter;
	m_Data.nUseOption   = this->m_nCentOpt;
	if(m_bStoryCenter)
	{
		unsigned int nType;
		if(m_nCentOpt == 2)
		{      
			m_wndXLcCombo.GetSelectedLoad(nType, m_Data.LoadCaseKeyX);
			m_wndYLcCombo.GetSelectedLoad(nType, m_Data.LoadCaseKeyY);
		}
		m_wndXStifCombo.GetSelectedLoad(nType, m_Data.StsfStldKeyX);
		m_wndYStifCombo.GetSelectedLoad(nType, m_Data.StsfStldKeyY);
	}

	m_Data.bStorRespThrs = m_bResponse;

	m_Data.nStorCenterMethod = m_nStorCenterMethod;
	if (m_nStorCenterMethod == 1 && m_bStorDriftByMaxVertElem && !CDBDoc::EnableJUD())
	{
		m_Data.nStorCenterMethod = 2;
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CBldcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CBldcDlg)
	ON_BN_CLICKED(IDC_ETC_BLDC_USE_FLAG, OnEtcBldcUseFlag)
	ON_BN_CLICKED(IDC_ETC_BLDC_USE_UNDER_SEIS_LOADS_CHK, OnEtcBldcUnderSeisLoad)
	ON_BN_CLICKED(IDC_CMD_STOR_CENT_CHECK, OnCmdStorCentCheck)
	ON_BN_CLICKED(IDC_CMD_X_LC_BUTTON, OnCmdLcButton)
	ON_BN_CLICKED(IDC_CMD_USE_AXIAL_RADIO, OnCmdOptionRadio)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_RESPONSE_CHK,  OnCmdResponseChk)
	ON_BN_CLICKED(IDC_CMD_USE_SHEAR_RADIO, OnCmdOptionRadio)
	ON_BN_CLICKED(IDC_CMD_USE_MASS_RADIO, OnCmdOptionRadio)
	ON_BN_CLICKED(IDC_CMD_Y_LC_BUTTON, OnCmdLcButton)
	ON_BN_CLICKED(IDC_CMD_LC_BUTTON, OnCmdLcButton)
	ON_BN_CLICKED(IDC_CMD_STIF_X_LC_BTN, OnCmdLcButton)
	ON_BN_CLICKED(IDC_CMD_STIF_Y_LC_BTN, OnCmdLcButton)
	ON_BN_CLICKED(IDC_ETC_BLDC_SHEAR_FLAG, OnEtcBldcShearFlag)
	ON_BN_CLICKED(IDC_CMD_STOR_CENTER_RDO, OnCmdStorTHDriftRadio)
	ON_BN_CLICKED(IDC_CMD_STOR_AVERAGE_RDO, OnCmdStorTHDriftRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CBldcDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CBldcDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	int i;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				// do nothing
				break;
		case(UR_STLD_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = m_Data.nNumBldcCase-1; i >= 0; i--)
				{
					if (m_Data.BldcCase[i].LoadCaseKey == Key)
					{
						m_List.DeleteItem(i);
						for (int j = i; j < m_Data.nNumBldcCase-1; j++)
							m_Data.BldcCase[j] = m_Data.BldcCase[j+1];
						m_Data.nNumBldcCase--;
					}
				}
				break;
		case(UR_STLD_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetStld(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_STLD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = 0; i < m_Data.nNumBldcCase; i++)
				{
					if (m_Data.BldcCase[i].LoadCaseKey == Key)
					{
						m_Data.BldcCase[i].LoadCaseKey = KeyBak;
						SetItem(i, m_Data.BldcCase[i]);
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
/*
	BOOL bUpdate = FALSE;
	
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_BLDC_ADD):
			pViewBuff->GetBldc(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_BLDC_DEL):
			break;  // do nothing
		case(UR_BLDC_MFD):
			pViewBuff->GetBldc(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_BLDC_MFS):
			break;  // do nothing
		}
	}
	if (bUpdate)  MakeItemEx();
*/
}

//////////////////////////////////////////////////////////////////////////////
//List control... by KJH
void CBldcDlg::AlignControls()
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_CMD_STATIC1); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_CMD_STATIC3); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, aShearGroup, nDistY);

	this->GetWindowRect(&FromRect);
	pWnd = GetDlgItem(IDOK); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	FromRect.bottom = ToRect.bottom + globalUtils.ScaleByDPI(8);
	this->MoveWindow(&FromRect);
}

void CBldcDlg::StoryCentMan()
{  
	// 일단 Show/Hide 부터 
	CRect ToRect, FromRect;
	CDlgUtil::CtrlShowHide(this, aAxialGroup, m_nCentOpt == 1);
	CDlgUtil::CtrlShowHide(this, aShearGroup, m_nCentOpt == 2);

	// 다음으로 Enable/Disable
	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_CMD_STOR_CENTER_FRM);
	Ctrls.Add(IDC_CMD_USE_MASS_RADIO);
	Ctrls.Add(IDC_CMD_USE_AXIAL_RADIO);
	Ctrls.Add(IDC_CMD_USE_SHEAR_RADIO);
	
	CDlgUtil::CtrlEnableDisable(this, Ctrls, m_bStoryCenter);    
	CDlgUtil::CtrlEnableDisable(this, aStiffGroup, m_bStoryCenter);
	CDlgUtil::CtrlEnableDisable(this, aAxialGroup, m_bStoryCenter && m_nCentOpt == 1);
	CDlgUtil::CtrlEnableDisable(this, aShearGroup, m_bStoryCenter && m_nCentOpt == 2);
}

void CBldcDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__ScaleFactor) };
//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__LoadCase), _LS(IDS_WG_CMD__ADDD__ScaleFactor)};
	int nColWidth[] = {134, 80};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CBldcDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CBldcDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.nNumBldcCase;
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.BldcCase[nCount]);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CBldcDlg::DataToStr(int i, T_BLDC_BASE &data)
{
	CString value;
	if (i == 0) 
	{
		T_STLD_K key = data.LoadCaseKey;
		T_STLD_D data;
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		value = data.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dblFactor);
	return value;
}

void CBldcDlg::SetItem(int nIndex, T_BLDC_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CBldcDlg::ValidItem(T_BLDC_BASE& data)
{
	if (data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if (data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CBldcDlg::Dlg2Item(T_BLDC_BASE &data)
{
	UINT nLoadCaseType;
	if (!m_wndLcCombo.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	CString csScaleFactor;
	m_wndScaleFactor.GetWindowText(csScaleFactor);
	if(!CStrParser::GetFloatNumber(csScaleFactor, data.dblFactor)) return FALSE;
	//  data.dblFactor = _tstof(csScaleFactor);

	return TRUE;
}

BOOL CBldcDlg::Item2Dlg(T_BLDC_BASE &data)
{
	m_wndLcCombo.ChangeSelect(D_LOADCASE_STATIC, data.LoadCaseKey);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csScaleFactor);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBldcDlg message handlers

BOOL CBldcDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	AlignControls();

	m_wndBLevelEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBLevelUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBLevelUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLcCombo.SetLoadType(D_LOADCASE_STATIC,FALSE,TRUE);
	m_wndYLcCombo.SetLoadType(D_LOADCASE_STATIC,FALSE,TRUE);
	m_wndXLcCombo.SetLoadType(D_LOADCASE_STATIC,FALSE,TRUE);
	m_wndXStifCombo.SetLoadType(D_LOADCASE_STATIC,FALSE,TRUE);
	m_wndYStifCombo.SetLoadType(D_LOADCASE_STATIC,FALSE,TRUE);

	if (!m_pDoc->m_pAttrCtrl->GetBldc(m_Data)) m_Data.Initialize();  
	Data2DlgVar();
	UpdateData(FALSE);

	StoryCentMan();
	ControlStoryCenterMethod();

	m_wndScaleFactor.SetWindowText(_T("1"));

	SetListCtrlHeader();
	MakeItemEx();
	OnEtcBldcUseFlag();
	OnEtcBldcShearFlag();  
	OnEtcBldcUnderSeisLoad();
	BOOL bJUDEnable = CDBDoc::EnableJUD();  
	//if(bJUDEnable) JUDCtrlGroupDisable();
	//GetDlgItem(IDC_CMD_MAIN_FRM_JUD)->ShowWindow(SW_HIDE);

	// iGen의 경우는 모듈을 정의 할수 없기 때문에 기존 방식처럼 간다. JWKWON 2015-05-13
	if(bJUDEnable) GetDlgItem(IDC_CMD_STOR_MAX_VERT_ELEM_CHK)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBldcDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if (!DlgVar2Data())
		return;
	
	for (int i = m_Data.nNumBldcCase; i < D_BLDC_NUMBLDC; i++)
		m_Data.BldcCase[i].Initialize();
	if (m_pDoc->m_pDataCtrl->AddBldc(m_Data))
	{
		CDialogMove::OnOK();
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_add_replace_bulidin));
	}
}

void CBldcDlg::OnEtcBldcUseFlag() 
{
	// TODO: Add your control notification handler code here
	BOOL bUseGL = m_wndUseFlag.GetCheck() ? TRUE : FALSE;
	m_wndBLevelEdit.EnableWindow(bUseGL);
	m_wndUseBedRockFlag.EnableWindow(bUseGL);
	m_wndBedRockLevelEdit.EnableWindow((m_wndUseBedRockFlag.GetCheck() && bUseGL) ? TRUE : FALSE);
	m_wndMassFlag.EnableWindow(bUseGL);
}

void CBldcDlg::OnEtcBldcUnderSeisLoad()
{
	BOOL bIsChk = m_wndUseBedRockFlag.GetCheck();
	m_wndBedRockLevelEdit.EnableWindow(bIsChk);
	CString strChange;
	if (bIsChk)
		strChange = _LS(IDS_CMD_BEDROCK_FLAG_ON_MASS);
	else
		strChange = _LS(IDS_CMD_BEDROCK_FLAG_OFF_MASS);
	GetDlgItem(IDC_ETC_BLDC_MASS_FLAG)->SetWindowText(strChange);
}

void CBldcDlg::OnCmdStorCentCheck() 
{
	UpdateData();
	StoryCentMan();
	ControlStoryCenterMethod();
}

void CBldcDlg::OnCmdLcButton() 
{
	CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(),CCMLoadCaseDlg::IDD,NULL);
}

void CBldcDlg::OnCmdOptionRadio() 
{
	UpdateData();
	StoryCentMan();
}

void CBldcDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_BLDC_BASE data;
	
	if (m_Data.nNumBldcCase >= D_BLDC_NUMBLDC) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumBldcCase; i++)
	{
		if (m_Data.BldcCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_wndLcCombo.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.BldcCase[m_Data.nNumBldcCase] = data;
	m_List.InsertItem(m_Data.nNumBldcCase, _T(""));
	SetItem(m_Data.nNumBldcCase, data);
	m_List.SetItemState(m_Data.nNumBldcCase++, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CBldcDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_List.DeleteItem(nIndex);
	for (int i = nIndex; i < m_Data.nNumBldcCase-1; i++)
		m_Data.BldcCase[i] = m_Data.BldcCase[i+1];
	
	m_Data.nNumBldcCase--;
	if (m_Data.nNumBldcCase > 0)
	{
		if (nIndex >= m_Data.nNumBldcCase) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CBldcDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_BLDC_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumBldcCase; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_Data.BldcCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_wndLcCombo.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.BldcCase[nIndex] = data;
	SetItem(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED); 
}

void CBldcDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nIndex;
	T_BLDC_BASE data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else data = m_Data.BldcCase[nIndex];

	// 값을 설정한다.
	Item2Dlg(data);

	*pResult = 0;
}

void CBldcDlg::OnCmdResponseChk()
{
	UpdateData(TRUE);

	ControlStoryCenterMethod();
}

void CBldcDlg::OnCmdStorTHDriftRadio()
{
	UpdateData(TRUE);

	if (m_nStorCenterMethod==1)
	{
		// 2014-07-14. by hsshim
		// MQC 4836-49 Response of Time History Results 옵션의 중문 Default 값을, Story Average에 옵션이 체크된 것으로 반영
		// 원래 m_nStorCenterMethod를 바꿔줘야 하나, 대화상자의 체크 옵션으로 빼면서 m_bStorDriftByMaxVertElem만 설정해 두고 DB에 저장시 바꿔줌.
		m_bStorDriftByMaxVertElem = TRUE;
		UpdateData(FALSE);
	}

	ControlStoryCenterMethod();
}

void CBldcDlg::ControlStoryCenterMethod()
{
	BOOL bNeedUpdateData = FALSE;
	if(m_bResponse && !m_bStoryCenter && m_nStorCenterMethod==0)
	{
		m_nStorCenterMethod=1;

		// 2014-07-14. by hsshim
		// MQC 4836-49 Response of Time History Results 옵션의 중문 Default 값을, Story Average에 옵션이 체크된 것으로 반영
		// 원래 m_nStorCenterMethod를 바꿔줘야 하나, 대화상자의 체크 옵션으로 빼면서 m_bStorDriftByMaxVertElem만 설정해 두고 DB에 저장시 바꿔줌.
		m_bStorDriftByMaxVertElem = TRUE;

		bNeedUpdateData = TRUE;
	}

	GetDlgItem(IDC_CMD_STOR_CENTER_RDO) ->EnableWindow(m_bResponse && m_bStoryCenter);
	GetDlgItem(IDC_CMD_STOR_AVERAGE_RDO)->EnableWindow(m_bResponse);

	GetDlgItem(IDC_CMD_STOR_MAX_VERT_ELEM_CHK)->EnableWindow(m_bResponse && m_nStorCenterMethod==1);

	if (m_nStorCenterMethod!=1 && m_bStorDriftByMaxVertElem)  
	{ 
		m_bStorDriftByMaxVertElem = FALSE;
		bNeedUpdateData = TRUE;
	}

	if (bNeedUpdateData) UpdateData(FALSE);
}

void CBldcDlg::OnEtcBldcShearFlag() 
{
	// TODO: Add your control notification handler code here
	//m_wndConsiderStorMdul.EnableWindow(m_wndShearFlag.GetCheck());
	//if(!m_wndShearFlag.GetCheck())	m_wndConsiderStorMdul.SetCheck(FALSE);
}

//void CBldcDlg::JUDCtrlGroupDisable()
//{   
//	CArray<UINT, UINT> aDisableGroup;
//	aDisableGroup.Add(IDC_ETC_BLDC_SHEAR_FLAG);
//	//aDisableGroup.Add(IDC_ETC_BLDC_STOR_MDUL);
//	aDisableGroup.Add(IDC_ETC_BLDC_WIND_SEIS_ON_FLEXIBLE_CHK);
//	aDisableGroup.Add(IDC_CMD_RESPONSE_CHK);  
//	aDisableGroup.Add(IDC_CMD_STOR_CENTER_RDO);
//	aDisableGroup.Add(IDC_CMD_STOR_AVERAGE_RDO);
//	aDisableGroup.Add(IDC_CMD_STOR_MAX_VERT_ELEM_CHK);
//	CDlgUtil::CtrlEnableDisable(this, aDisableGroup, FALSE);
//
//	CDlgUtil::CtrlShowHide(this, aJUDGroup, SW_HIDE);
//
//	CRect rectFrame, rectDlg, rectOK, rectCancel;  
//	GetDlgItem(IDC_CMD_MAIN_FRM_JUD)->GetWindowRect(rectFrame);   
//	GetDlgItem(IDOK)->GetWindowRect(rectOK);
//	GetDlgItem(IDCANCEL)->GetWindowRect(rectCancel);            
//	this->GetWindowRect(rectDlg);
//	rectDlg.bottom = rectFrame.bottom + rectOK.Height() + 30;
//	this->MoveWindow(rectDlg);
//	
//	int nHeight = rectOK.Height();
//	rectOK.top    = rectCancel.top    = rectFrame.bottom + 15;
//	rectOK.bottom = rectCancel.bottom = rectOK.top + nHeight;    
//	ScreenToClient(rectOK    );
//	ScreenToClient(rectCancel);
//	GetDlgItem(IDOK)->MoveWindow(rectOK);
//	GetDlgItem(IDCANCEL)->MoveWindow(rectCancel);  
//}