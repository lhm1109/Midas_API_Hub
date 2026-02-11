// CMSpecDlg.cpp : implementation file
//
// 2003. 08. 27  우발편심고려 할 수 있도록 수정 by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSpecDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
//!@#$ #include "..\wg_base\wg_base_MainFrmBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"

#include "CMSplcSubDlg.h"
#include "CMSpecDampDlg.h"
#include "CMSpecModeDlg.h"
//#include "CMDampDlg.h"  // damping ratio dialog 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDlg dialog
#define CFormView CCMDlgBarBase
IMPLEMENT_DYNCREATE(CCMSpecDlg, CFormView)


CCMSpecDlg::CCMSpecDlg()
	: CFormView(CCMSpecDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSpecDlg)
	m_bEccent = FALSE;
	m_nInterpolateType = 1;
	m_nAutoType = 0;
	m_nAngleType = 0;
	//}}AFX_DATA_INIT

	m_aDampMethod[0] = _LS(IDS_CMD_SPLC_Direct_Modal);
	m_aDampMethod[1] = _LS(IDS_CMD_SPLC_Mass_and_Stiff_Proportional);
	m_aDampMethod[2] = _LS(IDS_CMD_THIS_GROUP1);   // Strain Energy Proportional

	m_arAngle.RemoveAll();
	m_arAngle.Add(IDC_CMD_SPEC_ANGLE_STATIC);
	m_arAngle.Add(IDC_CMD_SPEC_ANGLE_EDIT);
	//m_arAngle.Add(IDC_CMD_SPEC_ANGLE_SPIN);
	m_arAngle.Add(IDC_CMD_SPEC_ANGLE_UNIT);
}

CCMSpecDlg::~CCMSpecDlg()
{

}

void CCMSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSpecDlg)
	DDX_Control(pDX, IDC_CMD_SPLC_APPLY_DAMPING_CHK, m_wndApplyDampingChk) ;
	DDX_Control(pDX, IDC_CMD_SPEC_DAMP_METHOD_BTN, m_wndDampMethodBtn);
	DDX_Control(pDX, IDC_CMD_MOD_FACT_EDIT, m_edtModFact);
	DDX_Control(pDX, IDC_CMD_SPEC_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_SPEC_LC_NAME, m_wndLCname);
	DDX_Control(pDX, IDC_CMD_SPEC_DESC, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_SPEC_ANGLE_EDIT, m_wndAngleEdit);
	DDX_Control(pDX, IDC_CMD_SPLC_CQC3_EDT, m_wndCQCRatioEdit);
	DDX_Control(pDX, IDC_CMD_SPEC_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SPEC_ANGLE_UNIT, m_wndAngleUnit);
	DDX_Control(pDX, IDC_CMD_SPEC_DIRECTION, m_wndDirection);
	DDX_Check(pDX, IDC_CMD_SPEC_ECCEN_CHK, m_bEccent);
	DDX_Control(pDX, IDC_CMD_SPEC_DAMP_METHOD_EDIT, m_wndDampingMethod);	
	DDX_Control(pDX, IDC_CMD_SPEC_CORR_DAMP_CHK, m_wndCorrDampChk) ;
	DDX_Radio(pDX, IDC_CMD_INTERPOLATION_RDO1, m_nInterpolateType) ;
	DDX_Control(pDX, IDC_CMD_SPEC_FUNC_LIST, m_wndSpecFuncList);
	DDX_Radio(pDX, IDC_CMD_MAJOR_RDO, m_nAutoType);       // MNET:2767 20070514 mylee
	DDX_Radio(pDX, IDC_CMD_SPLC_ANGLE_RDO, m_nAngleType);
	DDX_Control(pDX, IDC_CMD_AUTO_CHK, m_chkAuto);        // MNET:2767 20070514 mylee
	DDX_Control(pDX, IDC_CMD_SPEC_ND_CHK, m_chkND);
	DDX_Control(pDX, IDC_CMD_SPEC_ND_EDT, m_wndNDEdit);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDlg diagnostics

#ifdef _DEBUG
void CCMSpecDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CCMSpecDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDlg implementation
void CCMSpecDlg::EnableEccentricity(BOOL bEnable)
{
	BOOL bExistStor = (m_pDoc->m_pAttrCtrl->GetCountStor() > 0);

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_CMD_SPEC_ECCEN_CHK); ASSERT(pWnd);
	pWnd->EnableWindow(bEnable && bExistStor);
	if (!bEnable || !bExistStor)
	{
		m_bEccent = FALSE;
		UpdateData(FALSE);
	}
	
	pWnd = GetDlgItem(IDC_CMD_SPEC_ECCEN_MORE); ASSERT(pWnd);
	UpdateData(TRUE);
	pWnd->EnableWindow(bEnable && bExistStor && m_bEccent);
}

void CCMSpecDlg::EnableAutoSearch(BOOL bEnable)
{
	if (bEnable)
	{
		GetDlgItem(IDC_CMD_AUTO_CHK)->EnableWindow(TRUE);
		OnCmdAutoChk();
	}
	else
	{
		GetDlgItem(IDC_CMD_AUTO_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_MAJOR_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_MINOR_RDO)->EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arAngle, FALSE);
	}
}

void CCMSpecDlg::EnableAngleCQC()
{
	CArray<UINT, UINT> arAngle;
	arAngle.Add(IDC_CMD_SPEC_ANGLE_EDIT);
	//arAngle.Add(IDC_CMD_SPEC_ANGLE_SPIN);
	arAngle.Add(IDC_CMD_SPEC_ANGLE_UNIT);

	CArray<UINT, UINT> arCQC3;
	arCQC3.Add(IDC_CMD_SPLC_CQC3_STC);
	arCQC3.Add(IDC_CMD_SPLC_CQC3_EDT);

	CDlgUtil::CtrlEnableDisable(this, arAngle, m_nAngleType == 0);
	CDlgUtil::CtrlEnableDisable(this, arCQC3, m_nAngleType == 1);

	int nDirection = m_wndDirection.GetCurSel();
	GetDlgItem(IDC_CMD_SPLC_CQC3_RDO)->EnableWindow(nDirection == 0);

	UpdateData(FALSE);
}

void CCMSpecDlg::EnableNonDissipative()
{
	BOOL bEnableND = FALSE;
	CArray<UINT, UINT> aSpfcKey;
	GetSpecFunc(aSpfcKey);
	T_SPFC_D SpfcD;
	for(int i=0; i<aSpfcKey.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetSpfc(aSpfcKey[i], SpfcD)) continue;
		if(!m_pDoc->m_pAttrCtrl->IsEnableND(aSpfcKey[i])) continue;

		bEnableND = TRUE;
		break;
	}

	if (bEnableND)
	{
		GetDlgItem(IDC_CMD_SPEC_ND_CHK)->EnableWindow(TRUE);
		OnCmdNonDissipativeChk();
	}
	else
	{
		m_chkND.SetCheck(0);
		GetDlgItem(IDC_CMD_SPEC_ND_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SPEC_ND_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SPEC_ND_EDT)->EnableWindow(FALSE);
	}
}

void CCMSpecDlg::SetListCtrlHeader()
{
	CStringArray aTitleTemp;
	aTitleTemp.Add(_LS(IDS_CMD_SPLC_loadcase));
	aTitleTemp.Add(_LS(IDS_CMD_DIRECTION));
	aTitleTemp.Add(_LS(IDS_CMD_SPLC_scale));
	int nColWidth[] = {70, 70, 60};
	int nColNum = 3;
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
		title = aTitleTemp[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(title.GetLength());
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMSpecDlg::GetSelectedSpecCase(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;
	
	nIndex = iItem;
	return TRUE;
}

void CCMSpecDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	T_SPLC_K key;
	T_SPLC_D data;
	CArray<T_SPLC_K, T_SPLC_K> aKey;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aKey);
	int nItemCount = aKey.GetSize();
	if(nItemCount == 0) return;

	for (int i = 0; i < nItemCount; i++)
	{
		key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetSplc(key, data);
		m_List.InsertItem(i, _T(""));
		SetItem(i, key, data);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

CString CCMSpecDlg::DataToStr(int i, T_SPLC_D &data)
{
	CString value;
	T_SPFC_D spfc;
	if (i == 0) value = data.LoadCaseName;
	else if (i == 1)
	{
		if (data.nAngleType == 1)
		{
			value.Format(_T("CQC3(%g)"), data.dCQCRatio);
		}
		else
		{
			if      (data.nDirection == 1) value = _T("X-Y");
			else if (data.nDirection == 2) value = _T("Z");
			else value = _LS(IDS_WG_CMD__ADDD__Error);
		}
	}
	else if (i == 2) value.Format(_T("%g"), data.dblFactor);
	return value;
}

void CCMSpecDlg::SetItem(int nIndex, T_SPLC_K key, T_SPLC_D &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_List.SetItemData(nIndex, key);
}

BOOL CCMSpecDlg::FindItem(T_SPLC_K key, int &nIndex)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
		if (m_List.GetItemData(i) == (DWORD)key)
		{
			nIndex = i; return TRUE;
		}

	return FALSE;
}


BOOL CCMSpecDlg::ValidSpecCase(T_SPLC_D &data)
{
	if (data.LoadCaseName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Loadcase_name_is_not_inpu));
		return FALSE;
	}
	for(int i = 0; i < data.aSpfcKey.GetSize(); i++)
	{
		if (data.aSpfcKey[i] <= 0) 
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Spectrum_function_is_not_));
			return FALSE;
		}
	}
	if (data.nDirection <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Direction_is_not_selected));
		return FALSE;
	}
	if (data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Scale_factor_is_in_invali));
		return FALSE;
	}
	if(data.dPeriodModifyFactor <= 0.)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Period_Mod_factor_can_not_negative));
		return FALSE;
	}
	
	// JTG/T B02-01-2008 일 경우 Damping 이 포함되어 있기 때문에 Correction by Damping ratio 를 체크한 경우 에러 메세지 출력
//   T_SPFC_D SpfcData;
//   for(i=0; i<data.aSpfcKey.GetSize(); i++)
//   {
//     if(m_pDoc->m_pAttrCtrl->GetSpfc(data.aSpfcKey[i], SpfcData))
//     {
//       if(SpfcData.nSpecCodeType = D_SPFC_CODE_JTJXXX_2008)
//       {
//         if(data.bApplyDampingMethod && data.bCorrectDamping)
//         {
//           AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Correction_Damping_ratio));
//           return FALSE;
//         }
//       }
//     }
//   }

		
	return TRUE;
}

BOOL CCMSpecDlg::Dlg2SpecCase(T_SPLC_D &data)
{
	data.Initialize();  
	m_wndLCname.GetWindowText(data.LoadCaseName);
	
	GetSpecFunc(data.aSpfcKey);
	if(data.aSpfcKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Spectrum_function_is_not_));
		return FALSE;
	}

	data.nDirection = m_wndDirection.GetCurSel()+1;
	// MNET:2767 20070514 mylee
	UpdateData(TRUE);
	data.bAutoSearchAngle = (m_chkAuto.GetCheck()==1);
	if (data.bAutoSearchAngle) data.nAutoType = m_nAutoType;
	else data.nAutoType = 0;

	if (data.nDirection == 1 && !data.bAutoSearchAngle) // if X-Y direction
		data.dblAngle = m_wndAngleEdit.GetEditValue();
	if (m_nAngleType == 1) //CQC3
		data.dblAngle = 0.0;

	data.nAngleType = m_nAngleType;
	data.dCQCRatio = m_wndCQCRatioEdit.GetEditValue();
	m_wndScaleFactor.GetEditValue(data.dblFactor);
	m_edtModFact.GetEditValue(data.dPeriodModifyFactor);
	m_wndNDEdit.GetEditValue(data.dNonDissipative);
	m_wndDesc.GetWindowText(data.Description);

	// damping method
	data.bApplyDampingMethod = m_wndApplyDampingChk.GetCheck() == 1 ? TRUE : FALSE;  
	if (data.bApplyDampingMethod)
	{
		data.nDampingType = m_SubData.nDampingType;
		data.dDampAll = m_SubData.dDampAll;
		data.aDampData.Copy(m_SubData.aDampData);
		data.PropDamping = m_SubData.PropDamping;
		data.bCorrectDamping = m_wndCorrDampChk.GetCheck()==1 ? TRUE : FALSE;
	}

	// eccentricity
	UpdateData(TRUE);
	data.bConsiderEccen = m_bEccent;
	if (data.bConsiderEccen)
	{
		data.nEccenMethod = m_SubData.nEccenMethod;
		data.nAutomatic = m_SubData.nAutomatic;
		data.dEccenPercent = m_SubData.dEccenPercent;
		data.aSplcEccen.Copy(m_SubData.aSplcEccen);
		data.aSplcEccen_AL.Copy(m_SubData.aSplcEccen_AL);
		data.bUseGL = m_SubData.bUseGL;
		data.bConsEccMnt = m_SubData.bConsEccMnt;
	}
	data.bNonDissipative = m_chkND.GetCheck() == 1 ? TRUE : FALSE;  
	data.nInterpolateType = m_nInterpolateType;

	if (data.nAngleType == 1)
	{
		m_SubData.bResultSign = FALSE;
		m_SubData.nSignType = 0;
	}
	data.nSpecCombType = m_SubData.nSpecCombType;
	data.bResultSign = m_SubData.bResultSign;
	data.bSelectUse = m_SubData.bSelectUse;
	data.nSignType = m_SubData.nSignType;
	data.aUseMode.Copy(m_SubData.aUseMode);
	data.aMSFactor.Copy(m_SubData.aMSFactor);

	return TRUE;
}

BOOL CCMSpecDlg::SpecCase2Dlg(T_SPLC_D &data)
{
	m_wndLCname.SetWindowText(data.LoadCaseName);	

	SetSpecFunc(data.aSpfcKey);

	m_wndDirection.SetCurSel(data.nDirection-1);
	OnChangeDirection();
	//EnableAngle(data.nDirection == 1);  // X-Y direction
	// MNET:2767 20070514 mylee
	if (data.nDirection == 1)
	{
		m_chkAuto.SetCheck(data.bAutoSearchAngle);
		OnCmdAutoChk();
		if (data.bAutoSearchAngle) m_nAutoType = data.nAutoType;
		else m_nAutoType = 0;
	}
	
	m_nAngleType = data.nAngleType;

	CString csVal;
	csVal.Format(_T("%g"), data.dblAngle);
	m_wndAngleEdit.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dCQCRatio);
	m_wndCQCRatioEdit.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dPeriodModifyFactor);
	m_edtModFact.SetWindowText(csVal);
	csVal.Format(_T("%g"), data.dNonDissipative);
	m_wndNDEdit.SetWindowText(csVal);
	m_wndDesc.SetWindowText(data.Description);

	// damping method
	m_wndApplyDampingChk.SetCheck((int)data.bApplyDampingMethod);  
	if (data.bApplyDampingMethod)
	{
		m_SubData.nDampingType = data.nDampingType;
		m_SubData.dDampAll = data.dDampAll;
		m_SubData.aDampData.Copy(data.aDampData);
		m_SubData.PropDamping = data.PropDamping;
		m_wndCorrDampChk.SetCheck((int)data.bCorrectDamping);
	}
	OnCmdApplyDampingChk();

	m_nInterpolateType = data.nInterpolateType;
	
	// eccentricity
	m_bEccent = data.bConsiderEccen;
	UpdateData(FALSE);
	if (m_bEccent)
	{
		m_SubData.nEccenMethod = data.nEccenMethod;
		m_SubData.nAutomatic = data.nAutomatic;
		m_SubData.dEccenPercent = data.dEccenPercent;
		m_SubData.aSplcEccen.Copy(data.aSplcEccen);
		m_SubData.aSplcEccen_AL.Copy(data.aSplcEccen_AL);
		m_SubData.bUseGL = data.bUseGL;
		m_SubData.bConsEccMnt = data.bConsEccMnt;
	}
	EnableEccentricity(data.nDirection == 1);
	//CtrlEnableDisable();

	m_chkND.SetCheck((int)data.bNonDissipative);
	EnableNonDissipative();

	m_SubData.nSpecCombType=data.nSpecCombType;
	m_SubData.bResultSign=data.bResultSign;
	m_SubData.bSelectUse=data.bSelectUse;
	m_SubData.nSignType=data.nSignType;
	m_SubData.aUseMode.Copy(data.aUseMode);
	m_SubData.aMSFactor.Copy(data.aMSFactor);

	OnCmdAngleType();

	return TRUE;
}

void CCMSpecDlg::SetDefault(T_SPLC_D &data)
{
	data.Initialize();
	data.dblFactor = 1.0;
	data.nDirection = 1;
	data.dPeriodModifyFactor = 1.;
	 int nEigvCount=m_pDoc->m_pAttrCtrl->GetNumEigv();
	 data.aUseMode.SetSize(nEigvCount);
	 data.aMSFactor.SetSize(nEigvCount);
	 for(int i=0;i<nEigvCount;i++)
	 {
		 data.aUseMode[i]=TRUE;
		 data.aMSFactor[i]=1.0;
	 }
	
	m_SubData.Initialize();
	 m_SubData.aUseMode.SetSize(nEigvCount);
	m_SubData.aMSFactor.SetSize(nEigvCount);
	for(int i=0;i<nEigvCount;i++)
	{
		m_SubData.aUseMode[i]=TRUE;
		m_SubData.aMSFactor[i]=1.0;
	}

} 
BEGIN_MESSAGE_MAP(CCMSpecDlg, CFormView)
	//{{AFX_MSG_MAP(CCMSpecDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnDlgClose)
	ON_BN_CLICKED(IDC_CMD_SPEC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SPEC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_CMD_SPEC_BTN_MODIFY, OnBtnModify)
	ON_BN_CLICKED(IDC_CMD_SPEC_BTN_COPY, OnBtnCopy)
	ON_CBN_SELCHANGE(IDC_CMD_SPEC_DIRECTION, OnChangeDirection)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SPEC_ANGLE_SPIN, OnDeltaposCmdSpecAngleSpin)
	ON_BN_CLICKED(IDC_CMD_SPEC_EIGV_DLG, OnEigvDlg)
	ON_BN_CLICKED(IDC_CMD_SPEC_SPFC_DLG, OnSpfcDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SPEC_LIST, OnChangedCurSpecCase)
	//ON_BN_CLICKED(IDC_CMD_SPEC_SPCT_DLG, OnCmdSpecSpctDlg)
	ON_BN_CLICKED(IDC_CMD_SPEC_ECCEN_CHK, OnCmdSpecEccenChk)
	ON_BN_CLICKED(IDC_CMD_SPEC_ECCEN_MORE, OnCmdSpecEccenMore)
	ON_BN_CLICKED(IDC_CMD_SPEC_DAMP_METHOD_BTN, OnCmdSpecDampMethodBtn)
	ON_BN_CLICKED(IDC_CMD_SPLC_APPLY_DAMPING_CHK, OnCmdApplyDampingChk)
	ON_CLBN_CHKCHANGE(IDC_CMD_SPEC_FUNC_LIST, OnClickCmdSpecFuncList)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SPEC_LIST, OnClickCmdSpecList)
	ON_BN_CLICKED(IDC_CMD_SPEC_MODE_SHAPES, OnCmdSpecModeShapes)
	ON_BN_CLICKED(IDC_CMD_AUTO_CHK, OnCmdAutoChk)
	ON_BN_CLICKED(IDC_CMD_SPEC_ND_CHK, OnCmdNonDissipativeChk)
	ON_BN_CLICKED(IDC_CMD_SPLC_ANGLE_RDO, OnCmdAngleType)
	ON_BN_CLICKED(IDC_CMD_SPLC_CQC3_RDO , OnCmdAngleType)

	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDlg message handlers

void CCMSpecDlg::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CCMDlgBarBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	AlignControls();

#if !defined(_MGEN) //CIVIL
	GetDlgItem(IDC_CMD_SPEC_ANGLE_STATIC)->ShowWindow(FALSE);

	GetDlgItem(IDC_CMD_AUTO_CHK)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_MAJOR_RDO)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_MINOR_RDO)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SPEC_ND_CHK)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SPEC_ND_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SPEC_ND_EDT)->ShowWindow(FALSE);

// 	if (CTestEnvMgr::GetTestEnvST(_LSX(Test_CQC3)) != _LSX(yes))
// 	{
// 		GetDlgItem(IDC_CMD_SPLC_CQC3_RDO)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CMD_SPLC_CQC3_STC)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CMD_SPLC_CQC3_EDT)->ShowWindow(SW_HIDE);
// 	}
#else
	GetDlgItem(IDC_CMD_SPLC_ANGLE_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SPLC_CQC3_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SPLC_CQC3_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_SPLC_CQC3_EDT)->ShowWindow(SW_HIDE);
#endif

	T_SPLC_D data;
	SetDefault(data);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();

	//m_wndAngleEdit.SetUnitType(CUnitCtrl::m_SPLC_UNIT.dblAngle);
	m_wndAngleEdit.SetRange(-360, 360);
	m_wndAngleUnit.SetUnitType(CUnitCtrl::m_SPLC_UNIT.dblAngle);
	m_wndCQCRatioEdit.SetUnitType(D_UNITSYS_NONE);
	m_wndDirection.AddString(_T("X-Y"));
	m_wndDirection.AddString(_T("Z"));
	m_wndDirection.SetCurSel(0);

	SetInitSpecFunc();
	SpecCase2Dlg(data);
	
	// ListCtrl의 값을 설정
	MakeItemEx();

	IsExistEigvComplex();
	OnCmdAngleType();

	UpdateData(FALSE);
}

void CCMSpecDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCMSpecDlg::OnBtnAdd()
{
	// TODO: Add your control notification handler code here
	T_SPLC_D data;
	
	if (!Dlg2SpecCase(data)) return;
	if (!ValidSpecCase(data)) return;

	int nCount = m_List.GetItemCount();
	if (m_pDoc->m_pDataCtrl->AddSplc(data))
		m_List.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMSpecDlg::OnBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedSpecCase(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_response_spec));
		return;
	}

	T_SPLC_D data;
	if (!Dlg2SpecCase(data)) return;
	if (!ValidSpecCase(data)) return;
	
	T_SPLC_K KeyOld = (T_SPLC_K)m_List.GetItemData(nIndex);
	T_SPLC_D DataOld;
	m_pDoc->m_pAttrCtrl->GetSplc(KeyOld, DataOld);
	data.LoadCaseId = DataOld.LoadCaseId;

	if (m_pDoc->m_pDataCtrl->ModifySplc(DataOld.LoadCaseName, data))
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMSpecDlg::OnBtnCopy()
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedSpecCase(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_response_spec));
		return;
	}

	T_SPLC_D data;
	data.Initialize();
	T_SPLC_K KeyOld = (T_SPLC_K)m_List.GetItemData(nIndex);
	T_SPLC_D DataOld;
	m_pDoc->m_pAttrCtrl->GetSplc(KeyOld, DataOld);
	data = DataOld;
	data.LoadCaseName = DataOld.LoadCaseName + _T("-1");
	

	int nCount = m_List.GetItemCount();
	if (m_pDoc->m_pDataCtrl->AddSplc(data))
		m_List.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMSpecDlg::OnBtnDelete() 
{
	int nIndex;
	if (!GetSelectedSpecCase(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_response_spec));
		return;
	}
	int nCount = m_List.GetItemCount();
	T_SPLC_K key = (T_SPLC_K)m_List.GetItemData(nIndex);
	T_SPLC_D data;
	m_pDoc->m_pAttrCtrl->GetSplc(key, data);
	if (m_pDoc->m_pDataCtrl->DelSplc(data.LoadCaseName))
	{
		if (nIndex >= nCount-1) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMSpecDlg::OnChangeDirection() 
{
	// TODO: Add your control notification handler code here
	int nDirection = m_wndDirection.GetCurSel();
	//EnableAngle(nDirection == 0); // enable when X-Y direction
	EnableAutoSearch(nDirection == 0);
	EnableEccentricity(nDirection == 0);

	if (nDirection == 1) // Z
	{
		m_nAngleType = 0;
	}
	EnableAngleCQC();
}

void CCMSpecDlg::OnDeltaposCmdSpecAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString csVal;
	
	CFormulaEdit* pEdit = (CFormulaEdit*)GetDlgItem(IDC_CMD_SPEC_ANGLE_EDIT);
	double dblAngle;
	pEdit->GetEditValue(dblAngle);
	dblAngle -= (double)pNMUpDown->iDelta;
	if (dblAngle > 360.0) dblAngle = 360.0;
	else if (dblAngle < -360.0) dblAngle = -360.0;
	
	csVal.Format(_T("%g"),dblAngle);
	pEdit->SetWindowText(csVal);
	
	*pResult = 0;
}

void CCMSpecDlg::OnEigvDlg() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_EIGENVALUE, 0));
}

void CCMSpecDlg::OnCmdSpecSpctDlg() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_RESP_CONTROL, 0));	
}

void CCMSpecDlg::OnSpfcDlg() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_SPEC_DEFINE, 0));	
}

void CCMSpecDlg::OnChangedCurSpecCase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	T_SPLC_K key;
	T_SPLC_D data;
	int nIndex;

	if (!GetSelectedSpecCase(nIndex))
		SetDefault(data);
	else 
	{
		key = m_List.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetSplc(key, data);
	}

	// 값을 설정한다.
	SpecCase2Dlg(data);

	*pResult = 0;
}

void CCMSpecDlg::OnClickCmdSpecFuncList() 
{
	EnableNonDissipative();
}

void CCMSpecDlg::OnClickCmdSpecList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_SPLC_K key;
	T_SPLC_D data;
	int nIndex;

	if (!GetSelectedSpecCase(nIndex))
		SetDefault(data);
	else 
	{
		key = m_List.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetSplc(key, data);
	}

	// 값을 설정한다.
	SpecCase2Dlg(data);
	
	*pResult = 0;
}

void CCMSpecDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateSpecFunc();
		MakeItemEx();    
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
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

void CCMSpecDlg::UpdateUnit() // 유닛 변경시 편심창 데이터 업데이트 m_SubData
{
	int nUnitTypeOffset = D_UNITSYS_BASE_LENGTH;  
	
	for (int i=0 ; i<m_SubData.aSplcEccen.GetSize() ; ++i)
	{
		m_SubData.aSplcEccen[i].dEccentricity = 
			m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, m_SubData.aSplcEccen[i].dEccentricity);
	}

	for (int i=0 ; i<m_SubData.aSplcEccen_AL.GetSize() ; ++i)
	{
		m_SubData.aSplcEccen_AL[i].dEccentricity = 
			m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, m_SubData.aSplcEccen_AL[i].dEccentricity);
	}
}

void CCMSpecDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	BOOL bEigvChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_SPFC_ADD): case(UR_SPFC_DEL): case(UR_SPFC_MFD): case(UR_SPFC_MFS):
			bSpfcChanged = TRUE;
			break;
		case(UR_STOR_ADD): case(UR_STOR_DEL):
			bStorChanged = TRUE;
			break;
		case(UR_EIGV_ADD): case(UR_EIGV_DEL): 
		 bEigvChanged=TRUE;
			break;
			
		}
	}

	pos = pViewBuff->GetStartBuffer();

	T_SPLC_K Key, KeyBak;
	T_SPLC_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SPLC_ADD):
			{
				pViewBuff->GetSplc(nKey, Key, Data);
				int nCount = m_List.GetItemCount();
				m_List.InsertItem(nCount, _T(""));
				SetItem(nCount, Key, Data);
			}
			break;
		case(UR_SPLC_DEL):
			{
				pViewBuff->GetSplc(nKey, Key, Data);
				int nIndex;
				if (FindItem(Key, nIndex))
					m_List.DeleteItem(nIndex);
			}
			break;
		case(UR_SPLC_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetSplc(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
			}
			break;
		case(UR_SPLC_MFS):
			{
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetSplc(nKey, Key, Data);
				int nIndex;
				if (FindItem(Key, nIndex)) 
					SetItem(nIndex, KeyBak, DataBak);
				else ASSERT(0);
				// 플래그 수정
				bMFD = FALSE;
			}
			break;
		default:
			break;
		}
	} // end of while

	if(bEigvChanged)
	{
		int nEigvSize=m_pDoc->m_pAttrCtrl->GetNumEigv();
		int nModeNum=m_SubData.aUseMode.GetSize();
		int nDiffNum=nEigvSize-nModeNum;
		m_SubData.aUseMode.SetSize(nEigvSize);
		m_SubData.aMSFactor.SetSize(nEigvSize);
		if(nDiffNum>0)
		{
			for(int j=0;j<nDiffNum;j++)
			{
				m_SubData.aUseMode[nModeNum+j]=TRUE;
				m_SubData.aMSFactor[nModeNum+j]=1.0;
			}
		}
	}

	if (bSpfcChanged) UpdateSpecFunc();  // UpdateSpfcName(); // Spectrum function name을 Update한다.
	if (bStorChanged) 
	{
		int nDir = m_wndDirection.GetCurSel();
		EnableEccentricity(nDir==0);    
	}
}

BOOL CCMSpecDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_SPLC_D data;

	m_pDoc->m_pAttrCtrl->GetSplc(key, data);
	SpecCase2Dlg(data);

	return TRUE;
}

///////////////////////////////// 추가부분 

// Gen인 경우과 Civil인 경우가 다르다.
void CCMSpecDlg::AlignControls()
{
#ifdef _MGEN
	int nDir = m_wndDirection.GetCurSel();
	EnableEccentricity(nDir==0);
	//CtrlEnableDisable();
#else
	CArray<UINT, UINT> aCtrlHide;
	aCtrlHide.Add(IDC_CMD_SPEC_ECCEN_CHK);
	aCtrlHide.Add(IDC_CMD_SPEC_ECCEN_MORE);
	CDlgUtil::CtrlShowHide(this, aCtrlHide, FALSE);

	CRect r1, r2;
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_CMD_SPEC_MAIN); 
	if (!pWnd) { ASSERT(0); return; }
	pWnd->GetWindowRect(r1);
	m_wndDesc.GetWindowRect(r2);
	r1.bottom = r2.bottom + 10;
	pWnd->SetWindowPos(NULL, 0, 0, r1.Width(), r1.Height(), 
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	CArray<UINT, UINT> aCtrlMove;
	aCtrlMove.Add(IDC_CMD_SPEC_LIST);
	aCtrlMove.Add(IDC_CMD_SPEC_TITLE);
	aCtrlMove.Add(IDC_CMD_SPEC_BTN_ADD);
	aCtrlMove.Add(IDC_CMD_SPEC_BTN_MODIFY);
	aCtrlMove.Add((IDC_CMD_SPEC_BTN_COPY));
	aCtrlMove.Add(IDC_CMD_SPEC_BTN_DELETE);
	aCtrlMove.Add(IDC_CMD_SPEC_EIGV_DLG);
	//aCtrlMove.Add(IDC_CMD_SPEC_SPCT_DLG);
	aCtrlMove.Add(IDC_CMD_SPEC_SPFC_DLG);
	aCtrlMove.Add(IDC_CMD_CLOSE);

	m_List.GetWindowRect(r2);
	int nDistY = r1.bottom - r2.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, aCtrlMove, nDistY);
#endif

}

void CCMSpecDlg::OnCmdSpecEccenChk() 
{
	// TODO: Add your control notification handler code here
	int nDir = m_wndDirection.GetCurSel();
	EnableEccentricity(nDir==0);
}

void CCMSpecDlg::OnCmdSpecEccenMore() 
{
	// TODO: Add your control notification handler code here  
	m_SubData.nDirection = m_wndDirection.GetCurSel()+1;
	if (m_SubData.nDirection == 1) // if X-Y direction
	{
		m_SubData.dblAngle = double(m_wndAngleEdit.GetEditValue());
		m_SubData.bAutoSearchAngle = (m_chkAuto.GetCheck()==1);
	}

	CCMSplcSubDlg dlg;
	dlg.SetEccenData(&m_SubData);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetEccenData(m_SubData);
	}
}

void CCMSpecDlg::OnCmdSpecDampMethodBtn() 
{
	// TODO: Add your control notification handler code here

	T_SPLC_D backupData = m_SubData;

	CCMSpecDampDlg dlg(&m_SubData);
//  dlg.SetInitPos(D_INIT_POS_RT);
	if (dlg.DoModal() == IDOK) 
	{
		m_wndDampingMethod.SetWindowText(m_aDampMethod[m_SubData.nDampingType-1]);
	}
	else
	{
		m_SubData = backupData;
	}

	//AfxMessageBox(_T("Button Clicked"));
	//CCMDampDlg dampDlg ;
	//dampDlg.DoModal() ;	
}

void CCMSpecDlg::OnCmdApplyDampingChk()
{
	int nCheck = m_wndApplyDampingChk.GetCheck();
	m_wndDampMethodBtn.EnableWindow(nCheck==1);
	m_wndCorrDampChk.EnableWindow(nCheck==1);
	if (nCheck == 1) 
		m_wndDampingMethod.SetWindowText(m_aDampMethod[m_SubData.nDampingType-1]);
	else
	{
		m_SubData.dDampAll = 0.05;
		int nMovingType = CProduct::GetMovingType();
		if(nMovingType == D_PRODUCT_MOVING_JP) m_SubData.nDampingType = 3;
		else m_SubData.nDampingType = 1;
		m_SubData.aDampData.RemoveAll();
		m_SubData.PropDamping.Initialize();
		m_wndDampingMethod.SetWindowText(_T(""));
	}
}

void CCMSpecDlg::OnCmdAutoChk()
{
	int nCheck = m_chkAuto.GetCheck();
	GetDlgItem(IDC_CMD_MAJOR_RDO)->EnableWindow(nCheck==1);
	GetDlgItem(IDC_CMD_MINOR_RDO)->EnableWindow(nCheck==1);
	CDlgUtil::CtrlEnableDisable(this, m_arAngle, nCheck==0);
	
	// later - 포스트 있더라도 체크 없다가 있어지면 초기화되게 나중에 구현해보자
	if (m_pDoc->m_pPostCtrl->IsPostEnable())
	{
		CDlgUtil::CtrlEnableDisable(this, m_arAngle, TRUE);
		//GetDlgItem(IDC_CMD_SPEC_ANGLE_SPIN)->EnableWindow(nCheck==0);
		m_wndAngleEdit.SetReadOnly(nCheck==1);
	}

	/*
	int nDirection = m_wndDirection.GetCurSel();
	EnableEccentricity(nDirection == 0); // Direction 에 의해 정해지는 초기상태로 돌림
	if (nCheck==1) // 체크가 되고 해석이 없을때만 Disable
	{
		if (!m_pDoc->m_pPostCtrl->IsPostEnable())
		{
			GetDlgItem(IDC_CMD_SPEC_ECCEN_CHK)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_SPEC_ECCEN_MORE)->EnableWindow(FALSE);
		}
	}
	if (!GetSelectedSpecCase(nIndex))
		SetDefault(data);
	else 
	{
		key = m_List.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetSplc(key, data);
	}
	*/
}

/*void CCMSpecDlg::OnCmdSpecCorrDampChk() 
{	
	int nCheck = m_wndCorrDampChk.GetCheck();
	m_wndDampMethodBtn.EnableWindow(nCheck==1);		
	if (nCheck == 1) 
		m_wndDampingMethod.SetWindowText(m_aDampMethod[m_SubData.nDampingType-1]);
	else
	{
		m_SubData.dDampAll = 0.05;
		int nMovingType = CProduct::GetMovingType();
		if(nMovingType == D_PRODUCT_MOVING_JP) m_SubData.nDampingType = 3;
		else m_SubData.nDampingType = 1;
		m_SubData.aDampData.RemoveAll();
		m_SubData.PropDamping.Initialize();
		m_wndDampingMethod.SetWindowText(_T(""));
	}
}
*/

void CCMSpecDlg::OnCmdNonDissipativeChk()
{
	int nCheck = m_chkND.GetCheck();
	GetDlgItem(IDC_CMD_SPEC_ND_STC)->EnableWindow(nCheck==1);
	GetDlgItem(IDC_CMD_SPEC_ND_EDT)->EnableWindow(nCheck==1);
}

void CCMSpecDlg::OnCmdAngleType()
{
	UpdateData(TRUE);

	EnableAngleCQC();
}

void CCMSpecDlg::SetInitSpecFunc()
{
	m_aFuncName.RemoveAll(); 
	m_wndSpecFuncList.ResetContent();

	T_FUNC_NAME FuncName;

	T_SPFC_K KeySpfc;
	T_SPFC_D DataSpfc;
	CArray<T_SPFC_K, T_SPFC_K> aSpfcKeyList;
	m_pDoc->m_pAttrCtrl->GetSpfcKeyList(aSpfcKeyList);

	CString strName, strDamping;  
	for(int i = 0; i < aSpfcKeyList.GetSize(); i++)
	{
		KeySpfc = aSpfcKeyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetSpfc(KeySpfc, DataSpfc)) continue;
		strDamping.Format(_T("%g"), DataSpfc.dDampingRatio);
		FuncName.strName = DataSpfc.SpecFuncName + _T(" (") + strDamping + _T(")");
		FuncName.Key     = KeySpfc;
		m_aFuncName.Add(FuncName);    
	}
	int nCount, ix=0;
	nCount = m_aFuncName.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ix = m_wndSpecFuncList.AddString(m_aFuncName[i].strName);
		m_wndSpecFuncList.SetItemData(ix, m_aFuncName[i].Key);
		m_wndSpecFuncList.SetCheck(ix, 0);
	}
}

void CCMSpecDlg::SetSpecFunc(CArray<UINT, UINT>& aSpfcKey)
{     
	CMap<UINT, UINT, int, int> mapCheckedSpfc;
	mapCheckedSpfc.InitHashTable(11);
	mapCheckedSpfc.RemoveAll();
	int nTemp;
	for(int i = 0; i < aSpfcKey.GetSize(); i++)
	{
		if(!mapCheckedSpfc.Lookup(aSpfcKey[i], nTemp))
			mapCheckedSpfc.SetAt(aSpfcKey[i], 1);
	}

	int nCount = m_aFuncName.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		m_aFuncName[i].Key = m_wndSpecFuncList.GetItemData(i);
		if(mapCheckedSpfc.Lookup(m_aFuncName[i].Key, nTemp))
			m_wndSpecFuncList.SetCheck(i, 1);
		else
			m_wndSpecFuncList.SetCheck(i, 0);
	}  
}

void CCMSpecDlg::GetSpecFunc(CArray<UINT, UINT>& aSpfcKey)
{   
	aSpfcKey.RemoveAll();
	for(int i = 0; i < m_wndSpecFuncList.GetCount(); i++)
	{    
		if(m_wndSpecFuncList.GetCheck(i) != 0)
			aSpfcKey.Add(m_aFuncName[i].Key);
	}  
}

void CCMSpecDlg::UpdateSpecFunc()
{  
	T_FUNC_NAME FuncName;

	T_SPFC_K KeySpfc;
	T_SPFC_D DataSpfc;
	CArray<T_SPFC_K, T_SPFC_K> aSpfcKeyList;
	m_pDoc->m_pAttrCtrl->GetSpfcKeyList(aSpfcKeyList);

	// mapSpfcKey에 변경된 DB에 있는 Spfc의 Key를 채운다.
	CMap<UINT, UINT, int, int> mapSpfcKey;
	mapSpfcKey.InitHashTable(11);
	mapSpfcKey.RemoveAll();
	int nTemp;
	for(int i = 0; i < aSpfcKeyList.GetSize(); i++)
	{
		if(!mapSpfcKey.Lookup(aSpfcKeyList[i], nTemp))
			mapSpfcKey.SetAt(aSpfcKeyList[i], 1);
	}
	
	// 현재 대화상자의 Spfc Check List에 있는 Spfc중에서 변경된 DB에 없는 Spfc를 지운다.
	for(int i = m_wndSpecFuncList.GetCount()-1; i >=0; i--)
	{
		KeySpfc = m_wndSpecFuncList.GetItemData(i);
		if(!mapSpfcKey.Lookup(KeySpfc, nTemp))
		{      
			m_wndSpecFuncList.DeleteString(i);
			for(int j = 0; j < m_aFuncName.GetSize(); j++)
			{
				if(m_aFuncName[i].Key == KeySpfc) 
				{
					m_aFuncName.RemoveAt(i);
					break;
				}
			}
		}
	}

	// 변경된 Spfc DB를 반영한 m_aFuncName로부터 mapFuncName에 Spfc의 Key를 채운다.
	CMap<UINT, UINT, int, int> mapFuncName;
	mapFuncName.InitHashTable(11);
	mapFuncName.RemoveAll();
	for(int i = 0; i < m_aFuncName.GetSize(); i++)
	{
		if(!mapFuncName.Lookup(m_aFuncName[i].Key, nTemp))
			mapFuncName.SetAt(m_aFuncName[i].Key, 1);
	}

	
	// mapFuncName에 없는 새로 추가된 Spfc를 Spfc Check List에 넣는다. 이 때 m_aFuncName도 채워준다.
	CString strName, strDamping;  
	int ix;
	for(int i = 0; i < aSpfcKeyList.GetSize(); i++)
	{
		KeySpfc = aSpfcKeyList.GetAt(i);
		// 추가된 Spfc에 대한 작업
		if(!mapFuncName.Lookup(KeySpfc, nTemp))
		{
			if(!m_pDoc->m_pAttrCtrl->GetSpfc(KeySpfc, DataSpfc)) continue;
			strDamping.Format(_T("%g"), DataSpfc.dDampingRatio);
			FuncName.strName = DataSpfc.SpecFuncName + _T(" (") + strDamping + _T(")");
			FuncName.Key     = KeySpfc;
			m_aFuncName.Add(FuncName);    
			ix = m_wndSpecFuncList.AddString(FuncName.strName);
			m_wndSpecFuncList.SetItemData(ix, FuncName.Key);
			m_wndSpecFuncList.SetCheck(ix, 0);
		}    
	}
	
	// 데이터의 변경(이름, 감쇠값)을 반영한다.
	CArray<T_SPFC_K, T_SPFC_K> aSpfcKey;
	GetSpecFunc(aSpfcKey);
	SetInitSpecFunc();
	SetSpecFunc(aSpfcKey);  
}
// MNET:1952-BJLEE(BBONG)-20060322 

void CCMSpecDlg::OnCmdSpecModeShapes() 
{
	m_SubData.nAngleType = m_nAngleType;
	m_SubData.dCQCRatio = m_wndCQCRatioEdit.GetEditValue();
	if (m_SubData.nAngleType == 1)
	{
		m_SubData.bResultSign = FALSE;
		m_SubData.nSignType = 0;
	}

	CCMSpecModeDlg m_GridDlg(this);
	T_SPLC_D* data=&m_SubData;

	T_SPLC_D tempData;
	tempData=m_SubData;
	m_GridDlg.SetDataSource(data);
	if(m_GridDlg.DoModal()!=IDOK)
	{
		m_SubData=tempData;
	}	
	
}


void CCMSpecDlg::IsExistEigvComplex()
{
	T_EIGV_D EigvD;
	EigvD.Initialize();
	BOOL bExistEigv;
	if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetEigv(EigvD) && EigvD.nType > 2)
		bExistEigv = TRUE;
	else
		bExistEigv = FALSE;

	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SPLC_APPLY_DAMPING_CHK, !bExistEigv);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_SPEC_DAMP_METHOD_BTN, !bExistEigv);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_AUTO_CHK, !bExistEigv);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_MAJOR_RDO, !bExistEigv);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_MINOR_RDO, !bExistEigv);
	if (bExistEigv)
	{
		m_wndDampingMethod.SetWindowText(m_aDampMethod[1]);
		m_wndCorrDampChk.SetCheck(1);
	}

	UpdateData(FALSE);
}
