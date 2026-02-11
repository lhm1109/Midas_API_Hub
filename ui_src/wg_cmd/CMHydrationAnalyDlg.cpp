// CMHydrationAnalyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydrationAnalyDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_StrParser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg dialog


CCMHydrationAnalyDlg::CCMHydrationAnalyDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMHydrationAnalyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydrationAnalyDlg)
	m_evaluation = -1;
	m_type = 0;
	m_bSelfWeight = FALSE;
	m_nCreepType = 0;
	m_bUseEquivalent = FALSE;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();//
	ASSERT(m_pDoc);//

	m_data.Initialize();

	m_aCreep.Add(IDC_HHCT_CREEP_CHK);
	m_aCreep.Add(IDC_HHCT_SHRINK_CHK);
	m_aCreep.Add(IDC_HHCT_CANDS_CHK);
	m_aCreep.Add(IDC_CMD_STATIC_NUMBER ); //IDC_STATIC1
	m_aCreep.Add(IDC_CMD_STATIC_TOLERANCE );//IDC_STATIC2
	m_aCreep.Add(IDC_HHCT_GENERAL_CHK);//
	m_aCreep.Add(IDC_HHCT_EM_CHK);//
		
	m_aCreep.Add(IDC_HHCT_NUM);//
	//m_aCreep.Add(IDC_HHCT_NUM_SPIN);
	m_aCreep.Add(IDC_HHCT_TOL);
	m_aCreep.Add(IDC_HHCT_P1EDT ); //
	m_aCreep.Add(IDC_HHCT_T1EDT);//
	m_aCreep.Add(IDC_HHCT_P2EDT );//
	m_aCreep.Add(IDC_HHCT_T2EDT);//

	//m_aShrinkage.Add(IDC_STATIC1);
	//m_aShrinkage.Add(IDC_STATIC2);
	m_aShrinkage.Add(IDC_HHCT_NUM);
	///m_aShrinkage.Add(IDC_HHCT_NUM_SPIN);
	m_aShrinkage.Add(IDC_HHCT_TOL);
	
	m_aFinalStage.Add(IDC_HHCT_FINAL_STAGE_LAST);
	m_aFinalStage.Add(IDC_HHCT_FINAL_STAGE_OTHER);
}


void CCMHydrationAnalyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydrationAnalyDlg)
	DDX_Control(pDX, IDC_HHCT_T2EDT, m_day2);
	DDX_Control(pDX, IDC_HHCT_P2EDT, m_phi2);
	DDX_Control(pDX, IDC_HHCT_T1EDT, m_Day1);
	DDX_Control(pDX, IDC_HHCT_P1EDT, m_phi1);
	DDX_Control(pDX, IDC_HHCT_GENERAL_CHK, m_General);
	DDX_Control(pDX, IDC_HHCT_COBX_HSTG, m_cobxHstg);
	DDX_Control(pDX, IDC_HHCT_TOL, m_tol);
	DDX_Control(pDX, IDC_HHCT_CS_CHK, m_CreepShrinkChk);
	DDX_Control(pDX, IDC_HHCT_INIT_TEMPER, m_temper);
	DDX_Control(pDX, IDC_HHCT_INIT_TEMPER_UNIT, m_wndHhctInitThrUnit);
	DDX_Control(pDX, IDC_HHCT_CN_FACTOR, m_factor);
	DDX_Control(pDX, IDC_HHCT_SELFEDT, m_selfWeightFactor);
	DDX_Control(pDX, IDC_HHCT_NUM, m_num_spin);
	DDX_Radio(pDX, IDC_HHCT_EVALU_CENTER, m_evaluation);
	DDX_Radio(pDX, IDC_HHCT_CREEP_CHK, m_type);
	DDX_Check(pDX, IDC_HHCT_SELFWEIGHT_CHK, m_bSelfWeight);
	DDX_Radio(pDX, IDC_HHCT_GENERAL_CHK, m_nCreepType);
	DDX_Check(pDX, IDC_HHCT_UEA_CHK, m_bUseEquivalent);
	//}}AFX_DATA_MAP
}


void CCMHydrationAnalyDlg::SetHstgCombo()
{
	CArray<T_HSTG_K, T_HSTG_K> aHstgKey;
	T_HSTG_D HstgData;
	m_pDoc->m_pAttrCtrl->GetHstgKeyList(aHstgKey);
	int nHstg = aHstgKey.GetSize();
	m_cobxHstg.ResetContent();
	for(int i = 0; i < nHstg ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetHstg(aHstgKey[i],HstgData))
			CDlgUtil::CobxAddItem(m_cobxHstg, HstgData.StageName, aHstgKey[i]);
	}
	m_cobxHstg.SetCurSel(0);
}

BEGIN_MESSAGE_MAP(CCMHydrationAnalyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMHydrationAnalyDlg)
	ON_BN_CLICKED(IDC_HHCT_OK, OnHhctOk)
	ON_BN_CLICKED(IDC_HHCT_CANCEL, OnHhctCancel)
	ON_BN_CLICKED(IDC_HHCT_REMOVE, OnHhctRemove)
	ON_BN_CLICKED(IDC_HHCT_CS_CHK, OnHhctCsChk)
	ON_BN_CLICKED(IDC_HHCT_CREEP_CHK, OnSelChangeType)
	ON_BN_CLICKED(IDC_HHCT_FINAL_STAGE_LAST, OnChangeFinalOption)
	ON_BN_CLICKED(IDC_HHCT_SELFWEIGHT_CHK, OnSelfWeightChk)
	ON_BN_CLICKED(IDC_HHCT_GENERAL_CHK, OnHhctType)
	ON_BN_CLICKED(IDC_HHCT_SHRINK_CHK, OnSelChangeType)
	ON_BN_CLICKED(IDC_HHCT_CANDS_CHK, OnSelChangeType)
	ON_BN_CLICKED(IDC_HHCT_FINAL_STAGE_OTHER, OnChangeFinalOption)
	ON_BN_CLICKED(IDC_HHCT_EM_CHK, OnHhctType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg message handlers

BOOL CCMHydrationAnalyDlg::OnInitDialog() 
{  
	CDialogMove::OnInitDialog();
	
	/////////////////////////////////////////////////
	CDlgUtil::CtrlShowHide(this, m_Convergence, TRUE);
	CDlgUtil::CtrlShowHide(this, m_Eeff, FALSE);

	CRect nRect;
	CRect mRect,gRect;
	GetDlgItem(IDC_STATIC_CONVERGENCE_FRM)->GetWindowRect(&mRect);
	GetDlgItem(IDC_STATIC_EEFF_FRM)->GetWindowRect(&gRect);
	this->GetWindowRect(nRect);
	//nRect.bottom = nRect.bottom + (mRect.bottom-gRect.bottom) + 135;  
#ifdef _RUS  
	nRect.bottom = nRect.bottom + (mRect.bottom-gRect.bottom) + 200;  
#else
	nRect.bottom = nRect.bottom + (mRect.bottom-gRect.bottom) + 160;  
#endif
	MoveWindow(nRect);

	this->m_Convergence.Add(IDC_STATIC_CONVERGENCE_FRM);
	m_Convergence.Add(IDC_CMD_STATIC_NUMBER);
	m_Convergence.Add(IDC_HHCT_NUM);
	//m_Convergence.Add(IDC_HHCT_NUM_SPIN);
	m_Convergence.Add(IDC_CMD_STATIC_TOLERANCE);
	m_Convergence.Add(IDC_HHCT_TOL);
	
	this->m_Eeff.Add(IDC_STATIC_EEFF_FRM);
	m_Eeff.Add(IDC_STATIC_EEFF_T);
	m_Eeff.Add(IDC_STATIC_PHI1);
	m_Eeff.Add(IDC_HHCT_P1EDT);
	m_Eeff.Add(IDC_STATIC_TR);
	m_Eeff.Add(IDC_HHCT_T1EDT);
	m_Eeff.Add(IDC_STATIC_DAY1);
	m_Eeff.Add(IDC_STATIC_PHI2);
	m_Eeff.Add(IDC_HHCT_P2EDT);
	m_Eeff.Add(IDC_STATIC_TL);
	m_Eeff.Add(IDC_HHCT_T2EDT);
	m_Eeff.Add(IDC_STATIC_DAY2);
	//////////////////////////////////////
	
	/*
	void Initialize()
	{
		nFinalStage = 0;
		FinalHstgKey = 0;
		dTheta = 0.5; //
		dInitTemp = 20.0; //
		nEvaluation = 2;
		bCreepShrinkage = FALSE;
		nType = 1;
		nIter = 5; ///
		dTol = 0.001; ///
		bIncludeSelfWeight = FALSE;
		dSelfWeightFactor = -1.0;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
	}
	*/
	if(m_pDoc->m_pAttrCtrl->ExistHhct()) m_pDoc->m_pAttrCtrl->GetHhct(m_data);
	else
		m_data.Initialize();
	  
	m_type = m_data.nType - 1;
 // m_evaluation = m_data.nEvaluation; // 보완 
	m_CreepShrinkChk.SetCheck(m_data.bCreepShrinkage);
	m_nCreepType = m_data.nCreepCalcMethod; ////

	m_num_spin.SetRange(1, 10000);
	m_num_spin.SetInteger(TRUE);
	m_num_spin.SetValue(m_data.nIter);
	m_tol.SetEditUnit(m_data.dTol);
	m_phi1.SetEditUnit(m_data.dPhi1);
	m_phi2.SetEditUnit(m_data.dPhi2);
	m_Day1.SetEditUnit(m_data.nDay1);
	m_day2.SetEditUnit(m_data.nDay2); 

	m_wndHhctInitThrUnit.SetUnitType(D_UNITSYS_BASE_TEMPER);

	// final stage
	SetHstgCombo();
	CDlgUtil::CtrlRadioSetCheck(this, m_aFinalStage, m_data.nFinalStage);
	m_cobxHstg.EnableWindow(m_data.nFinalStage == 1);
	if (m_data.nFinalStage == 1)
		CDlgUtil::CobxSetCurSelItemData(m_cobxHstg, m_data.FinalHstgKey);
	
	//m_TimeList.Copy(m_data.aTime);
	CString str;
	str.Format(_T("%g"), m_data.dTheta);
	m_factor.SetWindowText(str);
	str.Format(_T("%g"), m_data.dInitTemp);
	m_temper.SetWindowText(str);

	m_evaluation = m_data.nEvaluation-1;// - 1;
	
	m_bSelfWeight = m_data.bIncludeSelfWeight;
	str.Format(_T("%g"), m_data.dSelfWeightFactor);
	m_selfWeightFactor.SetWindowText(str);

	m_bUseEquivalent = m_data.bUseEquivalentAge;

	SetListCtrlHeader();

	MakeItemEx();

	// 초기 데이타가 없으면 Remove Button Disable ... ///
	CButton* bt = (CButton*)GetDlgItem(IDC_HHCT_REMOVE);
	bt->EnableWindow( m_pDoc->m_pAttrCtrl->ExistHhct() );
	/////////////////////////////////////////////////////

	UpdateData(FALSE);

	OnHhctType();
	OnHhctCsChk();
	OnSelChangeType();
	OnSelfWeightChk();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CCMHydrationAnalyDlg::Item2Dlg() // from Database to Dialog  
{
	 return TRUE;
}

void CCMHydrationAnalyDlg::ResizeDialog()
{
	UINT nLastCtrlID = IDC_HHCT_CANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CCMHydrationAnalyDlg::OnHhctCsChk()
{
	UpdateData();
	CDlgUtil::CtrlEnableDisable(this, m_aCreep, m_CreepShrinkChk.GetCheck());
}

void CCMHydrationAnalyDlg::OnSelChangeType() 
{
	UpdateData();
	CDlgUtil::CtrlEnableDisable(this, m_aShrinkage, m_CreepShrinkChk.GetCheck());
}

void CCMHydrationAnalyDlg::OnHhctOk() 
{
	if(!Dlg2Item()) return;
	if(!m_pDoc->m_pDataCtrl->AddHhct(m_data)) return;;

	CDialogMove::OnOK();
}

void CCMHydrationAnalyDlg::OnHhctCancel() 
{
	CDialogMove::OnCancel();
}

void CCMHydrationAnalyDlg::OnHhctRemove() 
{
	if(m_pDoc->m_pDataCtrl->DelHhct())
	{
		CDialogMove::OnOK();
	}
}

int CCMHydrationAnalyDlg::AddData2List(double sum)
{
	/*
	CArray<double, double> temp;
	temp.RemoveAll();
	BOOL bflg = FALSE;
	int  num = 0;

	int cnt = m_TimeList.GetSize();
	for(int i=0; i < cnt; i++)
	{
		if(m_TimeList[i] == sum) return -1;
		if(m_TimeList[i] > sum && !bflg)
		{
			temp.Add(sum);
			bflg = TRUE;
			i--;
			num = i;
		}
		else
			temp.Add(m_TimeList[i]);
	}

	if(!bflg) temp.Add(sum);

	m_TimeList.RemoveAll();
	m_TimeList.Copy(temp);

	return num+1;
	*/
	return 0;
}

void CCMHydrationAnalyDlg::AddList2Data()
{
	/*
	m_TimeList.RemoveAll();
	for(int i=0; i < m_list.GetItemCount(); i++)
	{
		CString str = m_list.GetItemText(i, 1);
		double data = _tstof(str);
		m_TimeList.Add(data);
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg List Controls
void CCMHydrationAnalyDlg::SetListCtrlHeader()
{
	/*
	CString aTitle[] = { _LS(IDS_CMD_HYDRANALY_step), _LS(IDS_CMD_HYDRANALY_time) };
//18japan  CString aTitle[] = {_T("Step"), _T("Time(hr)")};
	int nColWidth[] = {40, 80};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_list.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_list.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_list.InsertColumn(i,&lvcolumn);
		m_list.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
	*/
}

BOOL CCMHydrationAnalyDlg::GetSelectedItem(int &nIndex)
{
	/*
	int iItem = m_list.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	*/
	return TRUE;
}

void CCMHydrationAnalyDlg::MakeItemEx()
{
	/*
	m_list.DeleteAllItems();

	int nItemCount = m_TimeList.GetSize();
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_list.InsertItem(nCount, _T(""));
		SetItem(nCount);
	}
	*/
//  m_list.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMHydrationAnalyDlg::SetItem(int nIndex)
{
	/*
	LVITEM lvitem;
	CString str;

	lvitem.iItem=nIndex;
	lvitem.iSubItem=0;
	str.Format(_T("%d"), nIndex+1);
	lvitem.pszText=str.GetBuffer(0);
	lvitem.mask=LVIF_TEXT ;

	m_list.SetItem(&lvitem);
	str.ReleaseBuffer();


	lvitem.iItem=nIndex;
	lvitem.iSubItem=1;
	str.Format(_T("%g"), m_TimeList[nIndex]);
	lvitem.pszText=str.GetBuffer(0);
	lvitem.mask=LVIF_TEXT ;

	m_list.SetItem(&lvitem);
	str.ReleaseBuffer();
	*/
}

BOOL CCMHydrationAnalyDlg::Dlg2Item() // from Dialog to DataBase
{
	UpdateData();
//  m_data.aTime.RemoveAll();
//  m_data.aTime.Copy(m_TimeList);

	CDlgUtil::CtrlRadioGetCheck(this, m_aFinalStage, m_data.nFinalStage);
	if (m_data.nFinalStage == 1)
	{
		if (!CDlgUtil::CobxGetItemDataByCurSel(m_cobxHstg, (DWORD&)m_data.FinalHstgKey))
		{
			AfxMessageBox(_LS(IDS_CMD0417__Error___Final_stage_is_not_selected_));
			return FALSE;
		}
	}
	m_data.dInitTemp = m_temper.GetEditValue();
	m_data.dTheta = m_factor.GetEditValue();

	m_data.nEvaluation = m_evaluation+1;// + 1;

	m_data.bCreepShrinkage = m_CreepShrinkChk.GetCheck();
	m_data.nType = m_type+1;
	m_data.dTol = m_tol.GetEditValue();
	CString csIter;
	m_num_spin.GetWindowText(csIter);
	if ( !CStrParser::GetINumber(csIter, m_data.nIter) ) return FALSE;
 
	m_data.nCreepCalcMethod = m_nCreepType;//
	
	m_data.dPhi1 = this->m_phi1.GetEditValue(); //
	m_data.dPhi2 = this->m_phi2.GetEditValue(); //
	m_data.nDay1 = this->m_Day1.GetEditValue(); //
	m_data.nDay2 = this->m_day2.GetEditValue(); //
	
	m_data.bIncludeSelfWeight = m_bSelfWeight;
	
	if (m_bSelfWeight)
		m_data.dSelfWeightFactor = m_selfWeightFactor.GetEditValue();

	m_data.bUseEquivalentAge = m_bUseEquivalent;

	UpdateData();

	//if( !(m_pDoc->m_pDataCtrl->AddHhct(m_data)) ) 
	 // return FALSE;
	
	return TRUE;
}
/*
void CCMGilcItemDlg::Data2Dlg()
{  
	// SetWindowText
	//m_pDoc->m_pAttrCtrl->GetGilc(m_Data.LoadCaseName,m_Data);
	
	m_ctrlName.SetWindowText(m_Data.LoadCaseName);

	int nCount = m_ctrlType.GetCount();
	for (int i=0; i<nCount; i++)
	{
		if (m_Data.nLoadCaseType == m_ctrlType.GetItemData(i)) break;
	}
	ASSERT(i != nCount);
	m_ctrlType.SetCurSel(i);

	//m_ctrlType.SetItemData(IDC_CMD_TYPE_COBX,m_Data.nLoadCaseType);
	// CComboBox : SetItemData(Init)/ GetItemData
}
*/

void CCMHydrationAnalyDlg::OnChangeFinalOption() 
{
	// TODO: Add your control notification handler code here
	CDlgUtil::CtrlRadioGetCheck(this, m_aFinalStage, m_data.nFinalStage);
	m_cobxHstg.EnableWindow(m_data.nFinalStage == 1);
}

void CCMHydrationAnalyDlg::OnSelfWeightChk()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_HHCT_SELFEDT)->EnableWindow(m_bSelfWeight);
}

void CCMHydrationAnalyDlg::OnHhctType() 
{	
	UpdateData(TRUE);

	CRect mRect,dRect;

	CDlgUtil::CtrlShowHide(this, m_Convergence, m_nCreepType == 0);
	CDlgUtil::CtrlShowHide(this, m_Eeff,   m_nCreepType == 1);
	GetDlgItem(IDC_STATIC_CONVERGENCE_FRM)->GetWindowRect(&mRect);
	GetDlgItem(IDC_STATIC_EEFF_FRM)->GetWindowRect(&dRect);

	if(m_nCreepType == 1)
		CDlgUtil::CtrlMoveDistY(this, m_Eeff, mRect.top-dRect.top);

	UpdateData(TRUE);

	ResizeDialog();
}
