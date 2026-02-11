// CMNlacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNlacDlg.h"
#include "CMNlacItemDlg.h"

#include "../wg_base/wg_base_DlgUtil.h"
#include "../wg_base/ModelessDlgCtrl.h"
#include "../wg_db/wg_db_InitCtrl.h"
#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_DataCtrl.h"
#include "../wg_db/wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_TOP_MARGIN 50 

/////////////////////////////////////////////////////////////////////////////
// CCMNlacDlg dialog

#define COLCOUNT 2

CCMNlacDlg::CCMNlacDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMNlacDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMNlacDlg)
	m_nMethod = -1;
	//}}AFX_DATA_INIT  
}


void CCMNlacDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMNlacDlg)  
	//DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_MAX_SPIN, m_maximumspin);
	DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_MAX_ED, m_maximum);
	//DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_NUM_SPIN, m_numberspin);
	DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_NUM_ED, m_number);	
	//DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXITER_SPIN, m_maxiterspin);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXITER_ED, m_maxiter);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXDISP_ED, m_maxdisp);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXDISP_UNIT, m_maxdispunit);
	//DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXINCR_SPIN, m_maxincrspin);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXINCR_ED, m_maxincr);
	//DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_INIT_SPIN, m_initforcespin);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_INIT_ED, m_initforce);
	//DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXITER_SPIN, m_DispCtrlMaxIterSpin);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXITER_ED, m_DispCtrlMaxIter);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXDISP_ED, m_DispCtrlMaxDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXDISP_UNIT, m_DispCtrlMaxDispUnit);
	//DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_DISPSTEP_SPIN, m_DispCtrlDispStepSpin);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_DISPSTEP_ED, m_DispCtrlDispStep);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MASTER_NODE_ED, m_DispCtrlMasterNode);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_DIRECTION_CMB, m_DispCtrlDirectionCmb);
	
	DDX_Control(pDX, IDC_CMD_LIST_NEWTON, m_ListNewton);
	DDX_Control(pDX, IDC_CMD_LIST_ARCLENG, m_ListArclen);
	DDX_Control(pDX, IDC_CMD_LIST_DISPCTRL, m_ListDispct);
	DDX_Control(pDX, IDC_CMD_NLAC_ENGERGY_CHK, m_engergyChk);	
	DDX_Control(pDX, IDC_CMD_NLAC_FORCE_CHK, m_forceChk);
	DDX_Control(pDX, IDC_CMD_NLAC_DISP_CHK, m_dispChk);
	DDX_Control(pDX, IDC_CMD_NLAC_ENGERGY_ED, m_engergyEdit);
	DDX_Control(pDX, IDC_CMD_NLAC_FORCE_ED, m_forceEdit);
	DDX_Control(pDX, IDC_CMD_NLAC_DISP_ED, m_dispEdit);
	DDX_Control(pDX, IDC_CMD_NLAC_NONLINEAR_TYPE_CHK, m_chkGeom);
	DDX_Control(pDX, IDC_CMD_NLAC_NONLINEAR_TYPE_CHK2, m_chkMatl);
	DDX_Radio(pDX, IDC_CMD_NLAC_ITER_METHOD_RDO, m_nMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMNlacDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMNlacDlg)  
	ON_BN_CLICKED(IDC_CMD_NLAC_ITER_METHOD_RDO2, OnChangeMethod)
	ON_BN_CLICKED(IDC_CMD_NLAC_ENGERGY_CHK, OnCmdNlacEngergyChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_DISP_CHK, OnCmdNlacDispChk)
	ON_BN_CLICKED(IDC_CMD_NLAC_FORCE_CHK, OnCmdNlacForceChk)	
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)	
	ON_BN_CLICKED(IDC_CMD_NLAC_REMOVE, OnCmdNlacRemove)
	ON_BN_CLICKED(IDC_CMD_NLAC_ITER_METHOD_RDO3, OnChangeMethod)
	ON_BN_CLICKED(IDC_CMD_NLAC_ITER_METHOD_RDO, OnChangeMethod)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST_NEWTON, OnDblclkCmdList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST_ARCLENG, OnDblclkCmdList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST_DISPCTRL, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_NLAC_NONLINEAR_TYPE_CHK, OnCmdBtnGeom)
	ON_BN_CLICKED(IDC_CMD_NLAC_NONLINEAR_TYPE_CHK2, OnCmdBtnMatl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMNlacDlg message handlers

BOOL CCMNlacDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
		
	m_aCtrlNewton.RemoveAll();
	m_aCtrlArcLeng.RemoveAll();
	m_aCtrlDispCtrl.RemoveAll();  
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlNewton,   IDC_CMD_NLAC_NEWTON_FRM, FALSE);  
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlArcLeng,  IDC_CMD_NLAC_ARCLENG_FRM, FALSE);	
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDispCtrl, IDC_CMD_NLAC_DISPCTRL_FRM, FALSE);
	
	m_maximum.SetRange(1, 1000);
	m_maximum.SetInteger(TRUE);
	m_number.SetRange(1, 1000);
	m_number.SetInteger(TRUE);
	m_maxiter.SetRange(1, 1000);
	m_maxiter.SetInteger(TRUE);
	m_initforce.SetRange(1, 1000);
	m_initforce.ShowWindow(SW_HIDE);  // 실수값으로 변경되어 spin 버튼 숨긴다.
	m_maxincr.SetRange(1, 1000);
	m_maxincr.SetInteger(TRUE);
	m_DispCtrlDispStep.SetRange(1, 1000);
	m_DispCtrlDispStep.SetInteger(TRUE);
	m_DispCtrlMaxIter.SetRange(1, 1000);
	m_DispCtrlMaxIter.SetInteger(TRUE);

	m_maxdispunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DispCtrlMaxDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_DispCtrlDirectionCmb.ResetContent();
	m_DispCtrlDirectionCmb.AddString(_T("Dx"));
	m_DispCtrlDirectionCmb.AddString(_T("Dy"));
	m_DispCtrlDirectionCmb.AddString(_T("Dz"));
	m_DispCtrlDirectionCmb.SetCurSel(0);

	m_DispCtrlMasterNode.SetAttNodeList();
	m_DispCtrlMasterNode.SetLButtonDownNotifyWindow(this);
	m_DispCtrlMasterNode.SetEnterNotifyWindow(this);
	m_DispCtrlMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_DispCtrlMasterNode.SetMaxNodeKeyNum(1);

	Data2Dlg();
	///////////////////////////////
//	UpdateData(FALSE);

	SetListCtrlHeader();
	OnChangeMethod();
	OnCmdNlacEngergyChk();
	OnCmdNlacDispChk();
	OnCmdNlacForceChk();  

	AlignControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMNlacDlg::AlignControls()
{  
	CRect rRef;
	CRect rToMove;
	int nDistY;
	
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlArcLeng, &m_aCtrlDispCtrl};
	UINT aFirstCtrlID[] = {IDC_CMD_NLAC_ARCLENG_FRM, IDC_CMD_NLAC_DISPCTRL_FRM};
	
	GetDlgItem(IDC_CMD_NLAC_NEWTON_FRM)->GetWindowRect(rRef);
	for(int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);    
	}

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CCMNlacDlg::Data2Dlg()
{
	if (!m_pDoc->m_pAttrCtrl->GetNlct(m_Data)) m_Data.Initialize();

	if (m_Data.GlobalData.nNonlinearType == 0)
	{
		m_chkGeom.SetCheck(1);
		m_chkMatl.SetCheck(0);
	}
	else if (m_Data.GlobalData.nNonlinearType == 1)
	{
		m_chkGeom.SetCheck(0);
		m_chkMatl.SetCheck(1);
	}
	else
	{
		m_chkGeom.SetCheck(1);
		m_chkMatl.SetCheck(1);
	}
	m_nMethod = m_Data.GlobalData.nIterationMethod;

	m_engergyChk.SetCheck(m_Data.GlobalData.bConvFlag[0]);
	m_dispChk.SetCheck(m_Data.GlobalData.bConvFlag[1]);
	m_forceChk.SetCheck(m_Data.GlobalData.bConvFlag[2]);

	m_engergyEdit.SetEditUnit(m_Data.GlobalData.dConvValue[0]);
	m_dispEdit.SetEditUnit(m_Data.GlobalData.dConvValue[1]);
	m_forceEdit.SetEditUnit(m_Data.GlobalData.dConvValue[2]);
		
	m_number.SetValue(m_Data.GlobalData.nLoadStepNewton);
	m_maximum.SetValue(m_Data.GlobalData.nMaxIterPerLoadStepNewton);
	m_maxincr.SetValue(m_Data.GlobalData.nMaxIncStepArclen);
	m_maxiter.SetValue(m_Data.GlobalData.nMaxIterIncStepArclen);
	m_initforce.SetValue(m_Data.GlobalData.dIniForceRatioArclen);
	m_maxdisp.SetEditUnit(m_Data.GlobalData.dMaxDisplacementArclen);
	m_DispCtrlDispStep.SetValue(m_Data.GlobalData.nMumDispStepDispct);
	m_DispCtrlMaxIter.SetValue(m_Data.GlobalData.nMaxIterIncStepDispct);
	m_DispCtrlMasterNode.SetKeyPressedFlag(FALSE);
	m_DispCtrlMasterNode.AddNodeKey(m_Data.GlobalData.MasterNodeKeyDispct);
	m_DispCtrlDirectionCmb.SetCurSel(m_Data.GlobalData.nDirectionDispct);
	m_DispCtrlMaxDisp.SetEditUnit(m_Data.GlobalData.dMaxDisplacementDispct);	  

	m_arNewton.RemoveAll();
	m_arArclen.RemoveAll();
	m_arDispct.RemoveAll();
	switch(m_nMethod)
	{
		case 0 : 
		case 2 :
			m_arNewton.Copy(m_Data.GlobalData.arNewton); 
			m_arDispct.Copy(m_Data.GlobalData.arDispct);
			break;
		case 1 : 
			m_arArclen.Copy(m_Data.GlobalData.arArclen); 
			break;
	}
	// 초기 데이타가 없으면 Remove Button Disable ... ///
	CButton* bt = (CButton*)GetDlgItem(IDC_CMD_NLAC_REMOVE);
	bt->EnableWindow(m_pDoc->m_pAttrCtrl->ExistNlct());
	/////////////////////////////////////////////////////
	UpdateData(FALSE);
}

BOOL CCMNlacDlg::Dlg2Data(T_NLCT_D& data)
{
	UpdateData();
	data.Initialize();

	BOOL bGeom, bMatl;
	bGeom = m_chkGeom.GetCheck();
	bMatl = m_chkMatl.GetCheck();
	if (bGeom && bMatl) data.GlobalData.nNonlinearType = 2;
	else if (bGeom) data.GlobalData.nNonlinearType = 0;
	else data.GlobalData.nNonlinearType = 1;
	data.GlobalData.nIterationMethod = m_nMethod;
	
	if(m_engergyChk.GetCheck())
	{
		data.GlobalData.bConvFlag[0] = TRUE;
		data.GlobalData.dConvValue[0] = m_engergyEdit.GetEditValue();
	}
	else data.GlobalData.bConvFlag[0] = FALSE;

	if(m_dispChk.GetCheck())
	{
		data.GlobalData.bConvFlag[1] = TRUE;
		data.GlobalData.dConvValue[1] = m_dispEdit.GetEditValue();
	}
	else data.GlobalData.bConvFlag[1] = FALSE;

	if(m_forceChk.GetCheck())
	{
		data.GlobalData.bConvFlag[2] = TRUE;
		data.GlobalData.dConvValue[2] = m_forceEdit.GetEditValue();
	}
	else data.GlobalData.bConvFlag[2] = FALSE;

	if(m_nMethod == 0)  // Newton-Raphson
	{
		data.GlobalData.nLoadStepNewton = m_number.GetEditValueInt();
		data.GlobalData.nMaxIterPerLoadStepNewton = m_maximum.GetEditValueInt();
	}
	else if(m_nMethod == 1)  // Arc-Length
	{
		data.GlobalData.nMaxIncStepArclen      = m_maxincr.GetEditValueInt();
		data.GlobalData.nMaxIterIncStepArclen  = m_maxiter.GetEditValueInt();
		data.GlobalData.dIniForceRatioArclen   = m_initforce.GetEditValue();
		data.GlobalData.dMaxDisplacementArclen = m_maxdisp.GetEditValue();
	}
	else if(m_nMethod == 2)  // Displacement-Control
	{
		data.GlobalData.nMumDispStepDispct	    = m_DispCtrlDispStep.GetEditValueInt();
		data.GlobalData.nMaxIterIncStepDispct  = m_DispCtrlMaxIter.GetEditValueInt();

		int nMasterNodeKey = 0;
		CFormulaEdit::GetEditValue(&m_DispCtrlMasterNode, nMasterNodeKey);
		data.GlobalData.MasterNodeKeyDispct    = nMasterNodeKey;

		data.GlobalData.nDirectionDispct       = m_DispCtrlDirectionCmb.GetCurSel();
		data.GlobalData.dMaxDisplacementDispct = m_DispCtrlMaxDisp.GetEditValue();
	}

	switch(m_nMethod)
	{
		case 0: 
		case 2:
			data.GlobalData.arNewton.Copy(m_arNewton); // Newton-Raphson
			data.GlobalData.arDispct.Copy(m_arDispct); // Displacement-Control
			break;
		case 1: 
			data.GlobalData.arArclen.Copy(m_arArclen); // Arc-Length
			break;   
	}
	
	return TRUE;
}

void CCMNlacDlg::OnChangeMethod() 
{  
	UpdateData();
	CDlgUtil::CtrlShowHide(this, m_aCtrlNewton,   m_nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlArcLeng,  m_nMethod == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDispCtrl, m_nMethod == 2);
	m_ListNewton.ShowWindow(m_nMethod==0 || m_nMethod==2);   // Newton-Raphson, Diplacement Control
	m_ListArclen.ShowWindow(m_nMethod==1);   // Arc length
	m_ListDispct.ShowWindow(SW_HIDE);
	MakeItemEx();
}

void CCMNlacDlg::OnCmdNlacEngergyChk() 
{
	BOOL bForceflg = m_forceChk.GetCheck();
	BOOL bDispflg  = m_dispChk.GetCheck();	
	BOOL bEnerflg  = m_engergyChk.GetCheck();

	if(!bForceflg && !bDispflg && !bEnerflg)
	{
		bEnerflg = TRUE;
		m_engergyChk.SetCheck(bEnerflg);
	}
	m_engergyEdit.EnableWindow(bEnerflg);
}

void CCMNlacDlg::OnCmdNlacDispChk() 
{
	BOOL bForceflg = m_forceChk.GetCheck();
	BOOL bDispflg  = m_dispChk.GetCheck();
	BOOL bEnerflg  = m_engergyChk.GetCheck();

	if(!bForceflg && !bDispflg && !bEnerflg)
	{
		bDispflg = TRUE;
		m_dispChk.SetCheck(bDispflg);
	}
	m_dispEdit.EnableWindow(bDispflg);
}

void CCMNlacDlg::OnCmdNlacForceChk() 
{
	BOOL bForceflg = m_forceChk.GetCheck();
	BOOL bDispflg  = m_dispChk.GetCheck();
	BOOL bEnerflg  = m_engergyChk.GetCheck();

	if(!bForceflg && !bDispflg && !bEnerflg)
	{
		bForceflg = TRUE;
		m_forceChk.SetCheck(bForceflg);
	}
	m_forceEdit.EnableWindow(bForceflg);
}


void CCMNlacDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Load_Case),_LS(IDS_CMD_NLAC_INTERACTION_METHOD)};
	int nColWidth[] = {120,145};  
	CString title;  
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListNewton.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListNewton.GetSafeHwnd(), dwStyle);
	for(int i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListNewton.InsertColumn(i,&lvcolumn);
		m_ListNewton.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}

	dwStyle = ListView_GetExtendedListViewStyle(m_ListArclen.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListArclen.GetSafeHwnd(), dwStyle);
	for(int i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListArclen.InsertColumn(i,&lvcolumn);
		m_ListArclen.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMNlacDlg::MakeItemEx()
{  
	m_ListNewton.DeleteAllItems();  
	m_ListArclen.DeleteAllItems();  
	int nItemCount, nCount;

	if(m_nMethod==0 || m_nMethod==2)
	{
		nItemCount = m_arNewton.GetSize();
		if(nItemCount > 0)
		{
			for(nCount = 0; nCount < nItemCount; nCount++)
				SetItemNewton(m_arNewton.GetAt(nCount));
			m_ListNewton.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);     
		}

		nItemCount = m_arDispct.GetSize();
		if(nItemCount)
		{
			for(nCount = 0; nCount < nItemCount; nCount++)
				SetItemDispct(m_arDispct.GetAt(nCount));
			m_ListNewton.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);    
		}
	}
	else if(m_nMethod == 1)
	{
		nItemCount = m_arArclen.GetSize();
		if(nItemCount == 0) return;
		for(nCount = 0; nCount < nItemCount; nCount++)
			SetItemArclen(m_arArclen.GetAt(nCount));
		m_ListArclen.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);     
	}
}

BOOL CCMNlacDlg::GetSelectedItem(int &nIndex)
{
	if(m_nMethod==0 || m_nMethod==2)
	{
		nIndex = m_ListNewton.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		if(nIndex == -1) return FALSE;
		return TRUE;
	}
	else if(m_nMethod == 1)
	{
		nIndex = m_ListArclen.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		if(nIndex == -1) return FALSE;
		return TRUE;
	}
	else return FALSE;
}

CString CCMNlacDlg::DataToStrNewton(int i, T_NLCT_NEWTON &data)
{
	CString value;
	if(i == 0) 
	{  
		T_STLD_K KeyStld = data.StldKey;
		T_STLD_D DataStld;
		m_pDoc->m_pAttrCtrl->GetStld(KeyStld, DataStld);
		value = DataStld.LoadCaseName;  
	}
	else if(i == 1) 
	{  
		value = _LS(IDS_CMD_NLAC_NEWTON_RAPHSON);  
	}
	else ASSERT(FALSE);
	return value;
}

CString CCMNlacDlg::DataToStrArclen(int i, T_NLCT_ARCLEN &data)
{
	CString value;
	if(i == 0) 
	{  
		T_STLD_K KeyStld = data.StldKey;
		T_STLD_D DataStld;
		m_pDoc->m_pAttrCtrl->GetStld(KeyStld, DataStld);
		value = DataStld.LoadCaseName;  
	}
	else if(i == 1) 
	{  
		value = _LS(IDS_CMD_NLAC_ARC_LENGTH);  
	}
	else ASSERT(FALSE);
	return value;
}

CString CCMNlacDlg::DataToStrDispct(int i, T_NLCT_DISPCT &data)
{
	CString value;
	if(i == 0) 
	{  
		T_STLD_K KeyStld = data.StldKey;
		T_STLD_D DataStld;
		m_pDoc->m_pAttrCtrl->GetStld(KeyStld, DataStld);
		value = DataStld.LoadCaseName;  
	}
	else if(i == 1) 
	{  
		value = _LS(IDS_CMD_NLAC_DISPLACEMENT_CONTROL);  
	}
	else ASSERT(FALSE);
	return value;
}

void CCMNlacDlg::SetItemNewton(T_NLCT_NEWTON& DataNewton)
{
	LVITEM lvitem;
	CString str;
	T_STLD_K KeyStld;
	int nIndex;

	for(nIndex = 0; nIndex < m_ListNewton.GetItemCount(); nIndex++)
	{
		KeyStld = (T_STLD_K)m_ListNewton.GetItemData(nIndex);
		if(KeyStld == DataNewton.StldKey) {m_ListNewton.DeleteItem(nIndex); break;}
	}

	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		KeyStld = DataNewton.StldKey;
		str = DataToStrNewton(i, DataNewton);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		if(i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)KeyStld;
			m_ListNewton.InsertItem(&lvitem);
		}
		else m_ListNewton.SetItem(&lvitem);		
	  str.ReleaseBuffer();
	}  
}

void CCMNlacDlg::SetItemArclen(T_NLCT_ARCLEN& DataArclen)
{
	LVITEM lvitem;
	CString str;
	T_STLD_K KeyStld;
	int nIndex;

	for(nIndex = 0; nIndex < m_ListArclen.GetItemCount(); nIndex++)
	{
		KeyStld = (T_STLD_K)m_ListArclen.GetItemData(nIndex);
		if(KeyStld == DataArclen.StldKey) {m_ListArclen.DeleteItem(nIndex); break;}
	}

	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		KeyStld = DataArclen.StldKey;
		str = DataToStrArclen(i, DataArclen);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		if(i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)KeyStld;
			m_ListArclen.InsertItem(&lvitem);
		}
		else m_ListArclen.SetItem(&lvitem);		
	  str.ReleaseBuffer();
	}  
}

void CCMNlacDlg::SetItemDispct(T_NLCT_DISPCT& DataDispct)
{
	LVITEM lvitem;
	CString str;
	T_STLD_K KeyStld;
	int nIndex;

	for(nIndex = 0; nIndex < m_ListNewton.GetItemCount(); nIndex++)
	{
		KeyStld = (T_STLD_K)m_ListNewton.GetItemData(nIndex);
		if(KeyStld == DataDispct.StldKey) {m_ListNewton.DeleteItem(nIndex); break;}
	}

	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		KeyStld = DataDispct.StldKey;
		str = DataToStrDispct(i, DataDispct);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		if(i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)KeyStld;
			m_ListNewton.InsertItem(&lvitem);
		}
		else m_ListNewton.SetItem(&lvitem);		
	  str.ReleaseBuffer();
	}  
}

// m_arNewton에 추가하고자 하는 Data의 StldKey와 같은 Data가 존재할 때, 
// 기존 데이터를 삭제하고 새 데이터를 m_arNewton에 넣는다.
// 만약, m_arNewton에 추가하고자 하는 Data의 StldKey와 같은 Data가 존재하지 않으면,
// 기존 데이터 삭제없이 새 데이터를 m_arNewton에 넣는다.
void CCMNlacDlg::SetDataNewton(T_NLCT_NEWTON& DataNewton)
{
	BOOL bReplace = FALSE;
	for(int i = 0; i < m_arNewton.GetSize(); i++)
	{
		if(DataNewton.StldKey == m_arNewton[i].StldKey) 
		{
			bReplace = TRUE;
			m_arNewton.RemoveAt(i);
			m_arNewton.Add(DataNewton);
			break;
		}
	}
	if(!bReplace) m_arNewton.Add(DataNewton);  
}

void CCMNlacDlg::SetDataArclen(T_NLCT_ARCLEN& DataArclen)
{
	BOOL bReplace = FALSE;
	for(int i = 0; i < m_arArclen.GetSize(); i++)
	{
		if(DataArclen.StldKey == m_arArclen[i].StldKey) 
		{
			bReplace = TRUE;
			m_arArclen.RemoveAt(i);
			m_arArclen.Add(DataArclen);
			break;
		}
	}
	if(!bReplace) m_arArclen.Add(DataArclen);  
}

void CCMNlacDlg::SetDataDispct(T_NLCT_DISPCT& DataDispct)
{
	BOOL bReplace = FALSE;
	for(int i = 0; i < m_arDispct.GetSize(); i++)
	{
		if(DataDispct.StldKey == m_arDispct[i].StldKey) 
		{
			bReplace = TRUE;
			m_arDispct.RemoveAt(i);
			m_arDispct.Add(DataDispct);
			break;
		}
	}
	if(!bReplace) m_arDispct.Add(DataDispct);  
}

void CCMNlacDlg::UpdateNewtonList(const T_NLCT_NEWTON& DataNewton)
{
	int nIndex = -1;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	CString strMethod;
	if(m_nMethod == 0 || m_nMethod == 2) strMethod = m_ListNewton.GetItemText(nIndex, 1);
	else if(m_nMethod == 1)            strMethod = m_ListArclen.GetItemText(nIndex, 1);
	const int nMethod = GetInteractionMethodByName(strMethod);

	T_STLD_K KeyStld;
	if(nMethod == 0 || nMethod == 2) KeyStld = static_cast<T_STLD_K>(m_ListNewton.GetItemData(nIndex));
	else if(nMethod == 1)          KeyStld = static_cast<T_STLD_K>(m_ListArclen.GetItemData(nIndex));

	BOOL bDelete = TRUE;
	if(DataNewton.StldKey != KeyStld)
	{// Stld Key가 변경되었고, 이 변경된 Key에 해당하는 Data가 없는 경우 기존의 StldKey에 해당하는 Data 삭제
		for(int i = 0; i < m_ListNewton.GetItemCount(); i++)
		{
			if(DataNewton.StldKey == m_arNewton[i].StldKey)
			{
				bDelete = FALSE;
				break;
			}
		}

		if(bDelete) 
		{
			m_ListNewton.DeleteItem(nIndex);
			for(int i = 0; i < m_arNewton.GetSize(); i++)
			{
				if(m_arNewton[i].StldKey == KeyStld)
					m_arNewton.RemoveAt(i);
			}
		}
	}

	// 동일한 Stld key를 쓰는 Displacement가 있으면 삭제
	for(int i = m_arDispct.GetSize()-1; i >= 0; --i)
	{
		if(DataNewton.StldKey == m_arDispct[i].StldKey)
			m_arDispct.RemoveAt(i);
	}
}

void CCMNlacDlg::UpdateArclenList(T_NLCT_ARCLEN& DataArclen)
{
	int nIndex = -1;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	CString strMethod;
	if(m_nMethod == 0 || m_nMethod == 2) strMethod = m_ListNewton.GetItemText(nIndex, 1);
	else if(m_nMethod == 1)            strMethod = m_ListArclen.GetItemText(nIndex, 1);
	const int nMethod = GetInteractionMethodByName(strMethod);

	T_STLD_K KeyStld;
	if(nMethod == 0 || nMethod == 2) KeyStld = static_cast<T_STLD_K>(m_ListNewton.GetItemData(nIndex));
	else if(nMethod == 1)          KeyStld = static_cast<T_STLD_K>(m_ListArclen.GetItemData(nIndex));

	BOOL bDelete = TRUE;
	if(DataArclen.StldKey != KeyStld)
	{// Stld Key가 변경되었고, 이 변경된 Key에 해당하는 Data가 없는 경우 기존의 StldKey에 해당하는 Data 삭제
		for(int i = 0; i < m_ListArclen.GetItemCount(); i++)
		{
			if(DataArclen.StldKey == m_arArclen[i].StldKey)
			{
				bDelete = FALSE;
				break;
			}
		}

		if(bDelete)
		{
			m_ListArclen.DeleteItem(nIndex);
			for(int i = 0; i < m_arArclen.GetSize(); i++)
				if(m_arArclen[i].StldKey == KeyStld) m_arArclen.RemoveAt(i);
		}
	}
}

void CCMNlacDlg::UpdateDispctList(T_NLCT_DISPCT& DataDispct)
{
	int nIndex = -1;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	CString strMethod;
	if(m_nMethod == 0 || m_nMethod == 2) strMethod = m_ListNewton.GetItemText(nIndex, 1);
	else if(m_nMethod == 1)            strMethod = m_ListArclen.GetItemText(nIndex, 1);
	const int nMethod = GetInteractionMethodByName(strMethod);

	T_STLD_K KeyStld;
	if(nMethod == 0 || nMethod == 2) KeyStld = static_cast<T_STLD_K>(m_ListNewton.GetItemData(nIndex));
	else if(nMethod == 1)          KeyStld = static_cast<T_STLD_K>(m_ListArclen.GetItemData(nIndex));

	BOOL bDelete = TRUE;

	if(DataDispct.StldKey != KeyStld)
	{// Stld Key가 변경되었고, 이 변경된 Key에 해당하는 Data가 없는 경우 기존의 StldKey에 해당하는 Data 삭제
		for(int i = 0; i < m_ListNewton.GetItemCount(); i++)
		{
			if(DataDispct.StldKey == m_arDispct[i].StldKey)
			{
				bDelete = FALSE;
				break;
			}
		}

		if(bDelete)
		{
			m_ListNewton.DeleteItem(nIndex);
			for(int i = 0; i < m_arDispct.GetSize(); i++)
			{
				if(m_arDispct[i].StldKey == KeyStld)
					m_arDispct.RemoveAt(i);
			}
		}
	}        

	// 동일한 Stld key를 쓰는 Newton이 있으면 삭제
	for(int i = m_arNewton.GetSize()-1; i >= 0; --i)
	{
		if(DataDispct.StldKey == m_arNewton[i].StldKey)
			m_arNewton.RemoveAt(i);
	}
}

void CCMNlacDlg::OnCmdBtnAdd() 
{	
	UpdateData();

	CCMNlacItemDlg* pDlg = static_cast<CCMNlacItemDlg*>(m_pDoc->m_pDlgCtrl->GetDlg(CCMNlacItemDlg::IDD));
	if(pDlg == nullptr)
	{
		pDlg = new CCMNlacItemDlg(this);
		pDlg->Create(CCMNlacItemDlg::IDD, this);
	}

	pDlg->SetParamData(FALSE, m_nMethod, T_NLCT_NEWTON(), T_NLCT_DISPCT(), m_arNewton, m_arDispct);

	pDlg->ShowWindow(SW_SHOW);

	const int nIndex = m_ListNewton.GetItemCount();
	m_ListNewton.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMNlacDlg::OnCmdBtnModify() 
{	
	int nIndex, i;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	CString strMethod;
	if(m_nMethod == 0 || m_nMethod == 2) strMethod = m_ListNewton.GetItemText(nIndex, 1);
	else if(m_nMethod == 1)            strMethod = m_ListArclen.GetItemText(nIndex, 1);
	int nMethod = GetInteractionMethodByName(strMethod);

	T_STLD_K KeyStld;
	if(nMethod == 0 || nMethod == 2) KeyStld = static_cast<T_STLD_K>(m_ListNewton.GetItemData(nIndex));
	else if(nMethod == 1)          KeyStld = static_cast<T_STLD_K>(m_ListArclen.GetItemData(nIndex));

	CCMNlacItemDlg* pDlg = static_cast<CCMNlacItemDlg*>(m_pDoc->m_pDlgCtrl->GetDlg(CCMNlacItemDlg::IDD));
	if(pDlg == nullptr)
	{
		pDlg = new CCMNlacItemDlg(this);
		pDlg->Create(CCMNlacItemDlg::IDD, this);
	}

	T_NLCT_NEWTON dataNewton;
	T_NLCT_DISPCT dataDispct;
	if(nMethod == 0)      
	{
		for(i = 0; i < m_arNewton.GetSize(); i++)
		{
			if(KeyStld == m_arNewton[i].StldKey)
			{
				dataNewton = m_arNewton.GetAt(i);
				break;
			}
		}
	}
	else if(nMethod == 1) 
	{
		ASSERT(false);
	}    
	else if(nMethod == 2)
	{
		for(i = 0; i < m_arDispct.GetSize(); i++)
		{
			if(KeyStld == m_arDispct[i].StldKey)
			{
				dataDispct = m_arDispct.GetAt(i);
				break;
			}
		}
	}

	pDlg->SetParamData(TRUE, nMethod, dataNewton, dataDispct, m_arNewton, m_arDispct);

	pDlg->ShowWindow(SW_SHOW);

	m_ListNewton.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMNlacDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdBtnModify();		
	*pResult = 0;
}

void CCMNlacDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if(!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	CString csName;
	T_STLD_K KeyStld;
	if(m_nMethod == 0)
	{
		KeyStld = (T_STLD_K)m_ListNewton.GetItemData(nIndex);    
		for(int i = 0; i < m_arNewton.GetSize(); i++)
			if(m_arNewton[i].StldKey == KeyStld) {m_arNewton.RemoveAt(i); break;}
		for(int i = 0; i < m_arDispct.GetSize(); i++)
			if(m_arDispct[i].StldKey == KeyStld) {m_arDispct.RemoveAt(i); break;}
		m_ListNewton.DeleteItem(nIndex);    
		int nCount = m_ListNewton.GetItemCount();
		if (nCount <= nIndex) nIndex -= 1;
		if (nIndex >= 0) 
			m_ListNewton.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	}
	else if(m_nMethod == 1)
	{
		KeyStld = (T_STLD_K)m_ListArclen.GetItemData(nIndex);    
		for(int i = 0; i < m_arArclen.GetSize(); i++)
			if(m_arArclen[i].StldKey == KeyStld) {m_arArclen.RemoveAt(i); break;}
		m_ListArclen.DeleteItem(nIndex);
		int nCount = m_ListArclen.GetItemCount();
		if (nCount <= nIndex) nIndex -= 1;
		if (nIndex >= 0) 
			m_ListArclen.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	}
	else if(m_nMethod == 2)
	{
		KeyStld = (T_STLD_K)m_ListNewton.GetItemData(nIndex);    
		for(int i = 0; i < m_arNewton.GetSize(); i++)
			if(m_arNewton[i].StldKey == KeyStld) {m_arNewton.RemoveAt(i); break;}
		for(int i = 0; i < m_arDispct.GetSize(); i++)
			if(m_arDispct[i].StldKey == KeyStld) {m_arDispct.RemoveAt(i); break;}
		m_ListNewton.DeleteItem(nIndex);    
		int nCount = m_ListNewton.GetItemCount();
		if (nCount <= nIndex) nIndex -= 1;
		if (nIndex >= 0) 
			m_ListNewton.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	}
}

void CCMNlacDlg::OnOK() 
{
	T_NLCT_D data;
	if(!Dlg2Data(data)) return;
	if (!m_pDoc->m_pDataCtrl->AddNlct(data)) return;

	DestroyWindow();
}

void CCMNlacDlg::OnCancel()
{
	DestroyWindow();
}

void CCMNlacDlg::OnCmdNlacRemove() 
{
	if(m_pDoc->m_pDataCtrl->DelNlct())
	{
		DestroyWindow();
	}
}

void CCMNlacDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMNlacDlg::UpdateBuffer()
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
				for(i = m_arNewton.GetSize() - 1; i >=0; i--)          
					if(m_arNewton[i].StldKey == Key) m_arNewton.RemoveAt(i);          
				for(i = m_ListNewton.GetItemCount() - 1; i >= 0; i--)
					if((T_STLD_K)m_ListNewton.GetItemData(i) == Key) m_ListNewton.DeleteItem(i);
				for(i = m_arArclen.GetSize() - 1; i >=0; i--)          
					if(m_arArclen[i].StldKey == Key) m_arArclen.RemoveAt(i);          
				for(i = m_ListArclen.GetItemCount() - 1; i >= 0; i--)
					if((T_STLD_K)m_ListArclen.GetItemData(i) == Key) m_ListArclen.DeleteItem(i);
				for(i = m_arDispct.GetSize() - 1; i >=0; i--)          
					if(m_arDispct[i].StldKey == Key) m_arDispct.RemoveAt(i);          
//         for(i = m_ListDispct.GetItemCount() - 1; i >= 0; i--)
//           if((T_STLD_K)m_ListDispct.GetItemData(i) == Key) m_ListDispct.DeleteItem(i);
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
				for(i = m_arNewton.GetSize() - 1; i >=0; i--)          
					if(m_arNewton[i].StldKey == Key) m_arNewton[i].StldKey = KeyBak;
				for(i = m_arArclen.GetSize() - 1; i >=0; i--)          
					if(m_arArclen[i].StldKey == Key) m_arArclen[i].StldKey = KeyBak;
				for(i = m_arDispct.GetSize() - 1; i >=0; i--)          
					if(m_arDispct[i].StldKey == Key) m_arDispct[i].StldKey = KeyBak;
				MakeItemEx();                  
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CCMNlacDlg::DestroyWindow()
{
	CCMNlacItemDlg* pDlg = static_cast<CCMNlacItemDlg*>(m_pDoc->m_pDlgCtrl->GetDlg(CCMNlacItemDlg::IDD));
	if(pDlg != nullptr) pDlg->DestroyWindow();
	return CCMDlgBase::DestroyWindow();
}

inline void CCMNlacDlg::PostNcDestroy()
{
	DestroyWindow();
	CCMDlgBase::PostNcDestroy();
}

void CCMNlacDlg::OnCmdBtnGeom()
{
	BOOL bMatl;
	bMatl = m_chkMatl.GetCheck();
	if (bMatl) return;
	m_chkMatl.SetCheck(1);
}

void CCMNlacDlg::OnCmdBtnMatl()
{
	BOOL bGeom;
	bGeom = m_chkGeom.GetCheck();
	if (bGeom) return;
	m_chkGeom.SetCheck(1);
}

int CCMNlacDlg::GetInteractionMethodByName(CString strMethod)
{
			 if(strMethod.CompareNoCase(_LS(IDS_CMD_NLAC_NEWTON_RAPHSON))==0) return 0;
	else if(strMethod.CompareNoCase(_LS(IDS_CMD_NLAC_ARC_LENGTH))==0) return 1;
	else if(strMethod.CompareNoCase(_LS(IDS_CMD_NLAC_DISPLACEMENT_CONTROL))==0) return 2;
	else { ASSERT(0); return 0; }
}