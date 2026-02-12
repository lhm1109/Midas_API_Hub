// CMThgcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThgcDlg.h"
#include "CMThgcIncrStepDlg.h"
#include "CMThgcIterParamDlg.h"
#include "CMThgcWallOptionDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_METHOD_GENERAL 0
#define D_METHOD_SPECIAL 1

/////////////////////////////////////////////////////////////////////////////
// CCMThgcDlg dialog
CCMThgcDlg::CCMThgcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThgcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThgcDlg)
	m_bEnergyResult=FALSE;
	m_bSdvi        =FALSE;
	m_bSdve        =FALSE;
	m_bSdst        =FALSE;
	m_bSdhy        =FALSE;
	m_bSdis        =FALSE;
	m_bMSSStatus   =FALSE;
	m_nGeomNonlinearType = 0;
	m_nInitLoadMethod = D_METHOD_GENERAL;
	m_nHingeOutput = D_THGC_OUTPUT_OPT_ALL;
	m_bFiberSameHinge = FALSE;
	m_nFiberOutput = D_THGC_OUTPUT_OPT_ALL;

#if defined(_CIVIL)
	m_bCivil = TRUE;
#else
	m_bCivil = FALSE;
#endif

	m_aCtrl_InitLoadMethod.Add(IDC_INIT_LOAD_CONDITION_TYPE1_RDO);
	m_aCtrl_InitLoadMethod.Add(IDC_INIT_LOAD_CONDITION_TYPE2_RDO);
	m_aCtrl_GeomNonlinearType.Add(IDC_GEOM_NONLINEAR_TYPE1_RDO);
	m_aCtrl_GeomNonlinearType.Add(IDC_GEOM_NONLINEAR_TYPE2_RDO);
	if(!m_bCivil)
	{
		m_aCtrl_GeomNonlinearType.Add(IDC_GEOM_NONLINEAR_TYPE3_RDO);
	}
	m_aCtrl_SeqLoadingType.Add(IDC_SEQ_LOADING_TYPE1_RDO);
	m_aCtrl_SeqLoadingType.Add(IDC_SEQ_LOADING_TYPE2_RDO);
	//}}AFX_DATA_INIT
}

void CCMThgcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThgcDlg)
	DDX_Check(pDX, IDC_THGC_ENRG_CHK, m_bEnergyResult);
	DDX_Check(pDX, IDC_THGC_SDVI_CHK, m_bSdvi        );
	DDX_Check(pDX, IDC_THGC_SDVE_CHK, m_bSdve        );
	DDX_Check(pDX, IDC_THGC_SDST_CHK, m_bSdst        );
	DDX_Check(pDX, IDC_THGC_SDHY_CHK, m_bSdhy        );
	DDX_Check(pDX, IDC_THGC_SDIS_CHK, m_bSdis        );
	DDX_Check(pDX, IDC_THGC_MSS_CHK, m_bMSSStatus   );
	DDX_Control(pDX, IDC_PREVLOAD_STLD_CMB    , m_cmbStld);
	DDX_Control(pDX, IDC_PREVLOAD_LCASELIST   , m_lstLCase);
	DDX_Control(pDX, IDC_SCALE_FACTOR_EDT, m_edtScale);
	DDX_Control(pDX, IDC_CONSIDER_IGN_ELEM_CHK , m_chkConsiderIgnoreElem);
	DDX_Radio(pDX, IDC_THGC_HINGE_OPT1_RDO, m_nHingeOutput);
	DDX_Radio(pDX, IDC_THGC_FIBER_OPT1_RDO, m_nFiberOutput);
	DDX_Check(pDX, IDC_THGC_FIBER_SAME_CHK, m_bFiberSameHinge);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMThgcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThgcDlg)
	ON_BN_CLICKED(IDC_GEOM_NONLINEAR_TYPE1_RDO  , OnGeomNonlinearType)
	ON_BN_CLICKED(IDC_GEOM_NONLINEAR_TYPE2_RDO  , OnGeomNonlinearType)
	ON_BN_CLICKED(IDC_GEOM_NONLINEAR_TYPE3_RDO, OnGeomNonlinearType)
	ON_BN_CLICKED(IDC_INIT_LOAD_CONDITION_TYPE1_RDO  , OnRdoInitLoadMethod)
	ON_BN_CLICKED(IDC_INIT_LOAD_CONDITION_TYPE2_RDO  , OnRdoInitLoadMethod)
	ON_BN_CLICKED(IDC_PREVLOAD_ADD      , OnPushoverInitialLoadAdd)
	ON_BN_CLICKED(IDC_PREVLOAD_DELETE   , OnPushoverInitialLoadDelete)
	ON_BN_CLICKED(IDC_PREVLOAD_MODIFY   , OnPushoverInitialLoadModify)
	ON_BN_CLICKED(IDC_INCRE_STEP_BTN, OnBtnIncrStep)
	ON_BN_CLICKED(IDC_ITERA_PARAM_BTN,OnBtnIterParam)
	ON_NOTIFY(NM_CLICK , IDC_PREVLOAD_LCASELIST, OnChangeSelList)
	ON_NOTIFY(NM_DBLCLK, IDC_PREVLOAD_LCASELIST, OnChangeSelList)
	ON_BN_CLICKED(IDC_THGC_FIBER_SAME_CHK, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_HINGE_OPT1_RDO, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_HINGE_OPT2_RDO, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_HINGE_OPT3_RDO, UpdateFiberOutputCtrl)
	ON_BN_CLICKED(IDC_THGC_GLOBAL_WALL_NODE_BTN, OnBtnWall)
	ON_BN_CLICKED(IDC_THGC_GLOBAL_REMOVE_BTN, OnThgcRemove)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThgcDlg message handlers

BOOL CCMThgcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetThgc(m_ThgcD))
		m_ThgcD.Initialize();	

	m_nGeomNonlinearType = m_ThgcD.nGeomNonlinearType;
	m_nInitLoadMethod = m_ThgcD.nInitLoadMethod;

	m_edtScale.SetEditUnit(1.0);  
	SetListCtrlHeader();
	MakeItemEx();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_InitLoadMethod, m_ThgcD.nInitLoadMethod);
	if(m_ThgcD.nInitLoadMethod==1 && (m_ThgcD.nGeomNonlinearType== D_THIS_GEOMNONLIN_LARGE_DISP || m_ThgcD.nGeomNonlinearType == D_THIS_GEOMNONLIN_P_DELTA))
	{
		ASSERT(0);
		m_ThgcD.nGeomNonlinearType = 0;
	}

	GetDlgItem(IDC_GEOM_NONLINEAR_TYPE3_RDO)->ShowWindow(!m_bCivil);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_GeomNonlinearType, m_ThgcD.nGeomNonlinearType);

	CWnd* pWnd = GetDlgItem(IDC_THGC_GLOBAL_WALL_NODE_BTN);
	if (m_bCivil)
	{
		pWnd->ShowWindow(FALSE); // set control as invisible

		CRect cRefRect;
		GetDlgItem(IDC_INIT_LOAD_GRP)->GetClientRect(&cRefRect);
		CRect cRect;
		GetDlgItem(IDC_THGC_GLOBAL_REMOVE_BTN)->GetClientRect(&cRect);
		GetDlgItem(IDC_THGC_GLOBAL_REMOVE_BTN)->SetWindowPos(NULL, 0, 0, cRefRect.Width(), cRect.Height(), SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		if (CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) || CProduct::IsMovingType(D_PRODUCT_MOVING_US) || CProduct::IsMovingType(D_PRODUCT_MOVING_JP) || CProduct::IsMovingType(D_PRODUCT_MOVING_RUS))
		{
			pWnd->EnableWindow(TRUE); // set control as visible
		}
		else
		{
			pWnd->EnableWindow(FALSE); // set control as invisible
		}
	}

	SetLoadCaseCmb();

	Data2Dlg();
	OnGeomNonlinearType();
	UpdateFiberOutputCtrl();
	EnableConsiderIgnoreElem();

	CWnd* pWnd2 = GetDlgItem(IDC_THGC_MSS_CHK);
	if (pWnd2 != NULL) pWnd2->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThgcDlg::Data2Dlg()
{
	m_bEnergyResult=m_ThgcD.bEnergyResult;
	m_bSdvi        =m_ThgcD.bSdvi        ;
	m_bSdve        =m_ThgcD.bSdve        ;
	m_bSdst        =m_ThgcD.bSdst        ;
	m_bSdhy        =m_ThgcD.bSdhy        ;
	m_bSdis        =m_ThgcD.bSdis        ;
	m_bMSSStatus   =m_ThgcD.bMSSStatus   ;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_InitLoadMethod, m_ThgcD.nInitLoadMethod);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_GeomNonlinearType, m_ThgcD.nGeomNonlinearType);
	m_chkConsiderIgnoreElem.SetCheck(m_ThgcD.bConsiderIgnoreElem);
	m_nHingeOutput    = m_ThgcD.nHingeOutput;
	m_nFiberOutput    = m_ThgcD.nFiberOutput;
	m_bFiberSameHinge = m_ThgcD.bFiberSameHinge;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_SeqLoadingType, m_ThgcD.nSeqLoadingDisplType);

	UpdateData(FALSE);
}

void CCMThgcDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_ThgcD.bEnergyResult=m_bEnergyResult;
	m_ThgcD.bSdvi        =m_bSdvi        ;
	m_ThgcD.bSdve        =m_bSdve        ;
	m_ThgcD.bSdst        =m_bSdst        ;
	m_ThgcD.bSdhy        =m_bSdhy        ;
	m_ThgcD.bSdis        =m_bSdis        ;
	m_ThgcD.bMSSStatus   =m_bMSSStatus   ;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_InitLoadMethod, m_ThgcD.nInitLoadMethod);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_GeomNonlinearType, m_ThgcD.nGeomNonlinearType);
	m_ThgcD.bConsiderIgnoreElem = m_chkConsiderIgnoreElem.GetCheck();
	m_ThgcD.nHingeOutput    = m_nHingeOutput    ;
	m_ThgcD.nFiberOutput    = m_nFiberOutput    ;
	m_ThgcD.bFiberSameHinge = m_bFiberSameHinge ;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_SeqLoadingType, m_ThgcD.nSeqLoadingDisplType);
}

void CCMThgcDlg::OnOK()
{
	Dlg2Data();
	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddThgc(m_ThgcD)) return;

	CDialogMove::OnOK();
}

void CCMThgcDlg::OnGeomNonlinearType()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_GeomNonlinearType, m_nGeomNonlinearType);

	if( (m_nGeomNonlinearType== D_THIS_GEOMNONLIN_LARGE_DISP || m_nGeomNonlinearType == D_THIS_GEOMNONLIN_P_DELTA) && m_nInitLoadMethod==1)
	{
		m_nInitLoadMethod = 0;

		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_InitLoadMethod, m_nInitLoadMethod);

		SetLoadCaseCmb();
	}

	BOOL bR2Enable = m_nGeomNonlinearType==0;
	GetDlgItem(IDC_INIT_LOAD_CONDITION_TYPE2_RDO)->EnableWindow(bR2Enable);
	GetDlgItem(IDC_INIT_LOAD_CONDITION_TYPE2_DESC1_STC)->EnableWindow(bR2Enable);
	GetDlgItem(IDC_INIT_LOAD_CONDITION_TYPE2_DESC2_STC)->EnableWindow(bR2Enable);
	GetDlgItem(IDC_INIT_LOAD_CONDITION_TYPE2_DESC3_STC)->EnableWindow(bR2Enable);
	GetDlgItem(IDC_INIT_LOAD_CONDITION_TYPE2_DESC4_STC)->EnableWindow(bR2Enable);
}

void CCMThgcDlg::OnPushoverInitialLoadAdd() 
{
	T_TGLC_BASE data;
	
	if (!Dlg2TglcBase(data)) return;
	int nSize = m_ThgcD.aInitLoad.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (m_ThgcD.aInitLoad[i].LoadCaseType  == data.LoadCaseType &&
				m_ThgcD.aInitLoad[i].LoadCaseKey   == data.LoadCaseKey   )
		{
			CString msg;
			msg.Format(_LS(IDS_PUSHOVER_Error___The_data_already_exist_));
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_ThgcD.aInitLoad.Add(data);
	m_lstLCase.InsertItem(nSize, _T(""));
	SetItem(nSize, data);
	m_lstLCase.SetItemState(nSize, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	CWnd* wnd = GetDlgItem(IDC_SCALE_FACTOR_EDT);
	GotoDlgCtrl(wnd);	
}

void CCMThgcDlg::OnPushoverInitialLoadModify() 
{
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		AfxMessageBox(_LS(IDS_PUSHOVER_Error___No_selected_Data__nChoose_one_));
		return;
	}

	T_TGLC_BASE data;
	if (!Dlg2TglcBase(data)) return;

	int nSize = m_ThgcD.aInitLoad.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_ThgcD.aInitLoad[i].LoadCaseType == data.LoadCaseType &&
				m_ThgcD.aInitLoad[i].LoadCaseKey == data.LoadCaseKey     )
		{
			CString msg;
			msg.Format(_LS(IDS_PUSHOVER_The_data_already_exist_));
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_ThgcD.aInitLoad[nIndex] = data;
	SetItem(nIndex, data);
	m_lstLCase.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMThgcDlg::OnPushoverInitialLoadDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		AfxMessageBox(_LS(IDS_PUSHOVER_Error___No_selected_Data__nChoose_one_));
		return;
	}

	m_lstLCase.DeleteItem(nIndex);
	m_ThgcD.aInitLoad.RemoveAt(nIndex);
	int nSize = m_ThgcD.aInitLoad.GetSize();
	if (nSize > 0)
	{
		if (nIndex >= nSize) nIndex--;
		m_lstLCase.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CCMThgcDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_PUSHOVER_Static_Load_Case), _LS(IDS_PUSHOVER_Scale)};
	int nColWidth[] = {190, 80};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstLCase.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstLCase.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstLCase.InsertColumn(i,&lvcolumn);
		m_lstLCase.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMThgcDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_lstLCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMThgcDlg::MakeItemEx()
{
	m_lstLCase.DeleteAllItems();

	int nItemCount = m_ThgcD.aInitLoad.GetSize();
	if(nItemCount == 0) return;

	BOOL bPostMode = CDBDoc::GetDocPoint()->IsPostMode();
	BOOL bSkip = TRUE;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_lstLCase.InsertItem(nCount, _T(""));
		SetItem(nCount, m_ThgcD.aInitLoad[nCount]);
	}
	m_lstLCase.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCMThgcDlg::DataToStr(int i, T_TGLC_BASE &data)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CString strValue = _T("");
	T_STLD_D stld_d; stld_d.Initialize();
	if (i == 0)
	{
		CString PreNm = _T("");
		if(m_nInitLoadMethod==0)
		{
			ASSERT(data.LoadCaseType == D_LOADCASE_STATIC);
			if (pDoc->IsPostMode())       
				pDoc->m_pPostCtrl->GetStld(data.LoadCaseKey,stld_d);
			else
				pDoc->m_pAttrCtrl->GetStld(data.LoadCaseKey,stld_d);
		}
		else
		{
			if(pDoc->IsPostMode())
			{
				if(data.LoadCaseType == D_LOADCASE_STATIC)
				{
					pDoc->m_pPostCtrl->GetStld(data.LoadCaseKey,stld_d);
					PreNm = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STATIC,LOAD_NORMAL);
				}
				else if(data.LoadCaseType == D_LOADCASE_STAGE)
				{
					pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(data.LoadCaseKey, stld_d);
					PreNm = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STAGE,LOAD_NORMAL);
				}
				else {ASSERT(0);}
			}
			else
			{
				if(data.LoadCaseType == D_LOADCASE_STATIC)
				{
					pDoc->m_pAttrCtrl->GetStld(data.LoadCaseKey,stld_d);
					PreNm = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STATIC,LOAD_NORMAL);
				}
				else if(data.LoadCaseType == D_LOADCASE_STAGE)
				{
					pDoc->m_pAttrCtrl->GetSgld(data.LoadCaseKey, stld_d);
					PreNm = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STAGE,LOAD_NORMAL);
				}
				else {ASSERT(0);}
			}
		}
		strValue.Format(_T("%s%s"), PreNm, stld_d.LoadCaseName);
	}
	else if (i == 1) strValue.Format(_T("%g"), data.dScaleFactor);
	else strValue = _LS(IDS_PUSHOVER_Error);
	return strValue;
}

void CCMThgcDlg::SetItem(int nIndex, T_TGLC_BASE &data)
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

		m_lstLCase.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CCMThgcDlg::Dlg2TglcBase(T_TGLC_BASE &data)
{
	UpdateData(TRUE);
	UINT nLoadCaseType,nLoadCaseKey;
	CString val;
	
	if(m_edtScale.GetEditValue() == 0. )
	{
		AfxMessageBox(_LS(IDS_PUSHOVER_the_Scale_Factor_can_not_be_zero));
		return FALSE;
	}
	m_cmbStld.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseType = nLoadCaseType;
	data.LoadCaseKey  = nLoadCaseKey;
	data.dScaleFactor = m_edtScale.GetEditValue();

	return TRUE;
}

BOOL CCMThgcDlg::TglcBase2Dlg(T_TGLC_BASE &data)
{
	CString csVal;
	m_cmbStld.ChangeSelect(data.LoadCaseType, data.LoadCaseKey);	
	m_edtScale.SetEditUnit(data.dScaleFactor);
	UpdateData(FALSE);

	return TRUE;
}

void CCMThgcDlg::OnChangeSelList(NMHDR* pNMHDR, LRESULT* pResult)
{
	/**
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if ((pNMListView->uNewState & LVIS_SELECTED) == 0)
	{
		*pResult = 0;
		return;
	}**/

	int iItem = m_lstLCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	CString strSelLoadCase = m_lstLCase.GetItemText(/*pNMListView->*/iItem, 0);
	CString strScaleFct    = m_lstLCase.GetItemText(/*pNMListView->*/iItem, 1); 
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	UINT LoadCaseType = 0;
	UINT LoadCaseKey = 0;
	if (pDoc->IsPostMode())
	{
		CString strLoadCaseKey=strSelLoadCase;
		CString PreNmST = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STATIC, LOAD_NORMAL);
		CString PreNmCS = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STAGE, LOAD_NORMAL);
		if(strSelLoadCase.Find(PreNmST)==0)
		{
			LoadCaseType = D_LOADCASE_STATIC;
			strLoadCaseKey = strSelLoadCase.Right(2);
			LoadCaseKey = pDoc->m_pPostCtrl->GetStldKey(strSelLoadCase);
		}
		else if(strSelLoadCase.Find(PreNmCS)==0)
		{
			LoadCaseType = D_LOADCASE_STAGE;
			strLoadCaseKey = strSelLoadCase.Right(2);
			T_STLD_D StldD; StldD.Initialize();
			LoadCaseKey = pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(strLoadCaseKey, StldD);
		}
		else
		{
			LoadCaseType = D_LOADCASE_STATIC;
			LoadCaseKey = pDoc->m_pPostCtrl->GetStldKey(strLoadCaseKey);
		}
	}
	else
	{
		CString strLoadCaseKey=strSelLoadCase;
		CString PreNmST = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STATIC, LOAD_NORMAL);
		CString PreNmCS = CSelectLC::GetLoadNamePrefix(D_LOADCASE_STAGE, LOAD_NORMAL);
		if(strSelLoadCase.Find(PreNmST)==0)
		{
			LoadCaseType = D_LOADCASE_STATIC;
			strLoadCaseKey = strSelLoadCase.Right(2);
			LoadCaseKey = pDoc->m_pAttrCtrl->GetStldKey(strSelLoadCase);
		}
		else if(strSelLoadCase.Find(PreNmCS)==0)
		{
			LoadCaseType = D_LOADCASE_STAGE;
			strLoadCaseKey = strSelLoadCase.Right(2);
			T_STLD_D StldD; StldD.Initialize();
			LoadCaseKey = pDoc->m_pAttrCtrl->GetSgldKey(strLoadCaseKey);
		}
		else
		{
			LoadCaseType = D_LOADCASE_STATIC;
			LoadCaseKey = pDoc->m_pAttrCtrl->GetStldKey(strLoadCaseKey);
		}
	}
	
	m_cmbStld.ChangeSelect(LoadCaseType,LoadCaseKey);
	m_edtScale.SetEditUnit(_tstof(strScaleFct));

	*pResult = 1;
	return;
 
}    

void CCMThgcDlg::OnRdoInitLoadMethod() 
{	
	m_lstLCase.DeleteAllItems();
	m_ThgcD.aInitLoad.RemoveAll();

	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_InitLoadMethod, nCheck);
	m_nInitLoadMethod = nCheck;

	SetLoadCaseCmb();

	EnableConsiderIgnoreElem();
	if(nCheck==1)
		m_chkConsiderIgnoreElem.SetCheck(0);
}

void CCMThgcDlg::EnableConsiderIgnoreElem()
{
	int nInitLoadMethod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_InitLoadMethod, nInitLoadMethod);
	GetDlgItem(IDC_CONSIDER_IGN_ELEM_CHK)->EnableWindow(nInitLoadMethod==0);
}

void CCMThgcDlg::OnBtnIncrStep()
{
	CCMThgcIncrStepDlg dlg;
	dlg.SetThgc(m_ThgcD);
	if(dlg.DoModal()==IDOK)
	{
		m_ThgcD = dlg.GetThgc();
	}
}

void CCMThgcDlg::OnBtnIterParam()
{
	CCMThgcIterParamDlg dlg;
	dlg.SetThgc(m_ThgcD);
	if(dlg.DoModal()==IDOK)
	{
		m_ThgcD = dlg.GetThgc();
	}
}

void CCMThgcDlg::SetLoadCaseCmb()
{
	BOOL bCSInitLoadWithPO = FALSE;
	UINT nLCType = NULL;
	if(m_nInitLoadMethod==0)
	{
		bCSInitLoadWithPO = FALSE;
		nLCType = D_LOADCASE_STATIC;
	}
	else if(m_nInitLoadMethod==1)
	{
		bCSInitLoadWithPO = TRUE;
		nLCType = D_SELECTLOAD_STATIC|D_SELECTLOAD_STAGE;
	}
	m_cmbStld.SetCSInitLoadWithPO(bCSInitLoadWithPO);
	m_cmbStld.SetIgnoreStldCSLoad(TRUE);
	m_cmbStld.SetLoadType(nLCType, TRUE);
}

void CCMThgcDlg::UpdateFiberOutputCtrl()
{
	UpdateData();

	if(m_bFiberSameHinge || m_nFiberOutput<m_nHingeOutput)
	{
		m_nFiberOutput = m_nHingeOutput;
		UpdateData(FALSE);
	}

	// Fiber는 찍는데 힌지를 안 찍는건 불가
	GetDlgItem(IDC_THGC_FIBER_OPT1_RDO)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_ALL);
	GetDlgItem(IDC_THGC_FIBER_OPT1_STC)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_ALL);
	GetDlgItem(IDC_THGC_FIBER_OPT2_RDO)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_SEL);
	GetDlgItem(IDC_THGC_FIBER_OPT2_STC)->EnableWindow(!m_bFiberSameHinge && m_nHingeOutput<=D_THGC_OUTPUT_OPT_SEL);
	GetDlgItem(IDC_THGC_FIBER_OPT3_RDO)->EnableWindow(!m_bFiberSameHinge);
	GetDlgItem(IDC_THGC_FIBER_OPT3_STC)->EnableWindow(!m_bFiberSameHinge);
}

void CCMThgcDlg::OnBtnWall()
{
	CWnd* pBtn = GetDlgItem(IDC_THGC_GLOBAL_WALL_NODE_BTN); ASSERT(pBtn);
	CRect rectBtn; rectBtn.SetRectEmpty();
	if ( pBtn != NULL )
	{
		pBtn->GetWindowRect(&rectBtn);

		CPoint   pt(0, rectBtn.Height());
		rectBtn += pt;
	}

	CMThgcWallOptionDlg WallDlg;
	WallDlg.m_rectPosition = rectBtn;
	WallDlg.SetData(m_ThgcD.bConvertWallStiffness);
	if ( WallDlg.DoModal() == IDOK )
	{
		WallDlg.GetData(m_ThgcD.bConvertWallStiffness);
	}
}

void CCMThgcDlg::OnThgcRemove()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( !pDoc->m_pDataCtrl->DelThgc() ) return;
	CDialogMove::OnOK();
}
