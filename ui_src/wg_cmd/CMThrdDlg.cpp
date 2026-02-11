// CMThrdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBDocExtVar.h"

#include "CMThMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NCOL    2
/////////////////////////////////////////////////////////////////////////////
// CCMThrdDlg dialog
#define CDialog CCMChildBarBase

CCMThrdDlg::CCMThrdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMThrdDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdDlg)
	m_nFunctionType = 0;
	//}}AFX_DATA_INIT
}
void CCMThrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdDlg)
	DDX_Control(pDX, IDC_CMD_THRD_FUNC_LIST, m_ctrlList);
	DDX_Control(pDX, IDC_CMD_THRD_MENUCOMBO, m_ctrlFunction);
	DDX_Radio(pDX, IDC_CMD_THRD_FUNC_RDO1, m_nFunctionType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThrdDlg)
	ON_BN_CLICKED(IDC_CMD_THRD_FUNC_RDO1, OnCmdChangeCombo)
	ON_BN_CLICKED(IDC_CMD_THRD_SETFUNC, OnCmdThrdSetfunc)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnBarClose)
	ON_BN_CLICKED(IDC_CMD_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_CMD_BACK, OnBack)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_CMD_THRD_FUNC_RDO2, OnCmdChangeCombo)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_THRD_FUNC_LIST, OnDblclkCmdThrdFuncList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdDlg message handlers

BOOL CCMThrdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();

	OnCmdChangeCombo();
	MakeListHeader();
	MakeItemEx();

	if(CDBDocExtVar::Instance())  GetDlgItem(IDC_CMD_BACK)->EnableWindow(CDBDocExtVar::Instance()->GPS_m_nReturnMenuID);
	else                          ASSERT(0);
	
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThrdDlg::OnCmdThrdSetfunc() 
{
	UpdateData();
	//int nSel = m_ctrlFunction.GetCurSel();
	DWORD nSel;
	CDlgUtil::CobxGetItemDataByCurSel(m_ctrlFunction, nSel);
	
	CCMThMainDlg* pDlg = (CCMThMainDlg*)(GetParent()->GetParent());
	if (m_nFunctionType == 0)
	{
		switch(nSel)
		{
		case 0:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_DISP__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_DISP__);
			break;
		case 1:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_TRUSS__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_TRUSS__);
			break;
		case 2:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_BEAM__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_BEAM__);
			break;
		case 3:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_PLANE_STRESS__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_PLANE_STRESS__);
			break;
		case 4:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_PLANE_STRAIN__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_PLANE_STRAIN__);
			break;
		case 5:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_PLATE__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_PLATE__);
			break;
		case 6:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_SOLID__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_SOLID__);
			break;
		case 7:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_GENERAL_LINK__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_GENERAL_LINK__);
			break;
		case 8:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_INELASTIC_HINGE__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_INELASTIC_HINGE__);
			break;
		case 9:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_INELASTIC_HINGE__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRC_TIME_INCREMENT__);
			break;
		case 10:
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_DISP_STORY__);
			break;
		case 11:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_BEAM__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_WALL__);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	else
	{
		switch(nSel)
		{
		case 0:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_STEP_TIMESTEP__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_TIMESTEP__);
			break;
		case 1:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_STEP_DISPVELACC__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_DISPVELACC__);
			break;
		case 2:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_STEP_TRUSS__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_TRUSS__);
			break;
		case 3:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_STEP_BEAM__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_BEAM__);
			break;
		case 4:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_STEP_DESIGDOF__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_DESIGDOF__);
			break;
		case 5:
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_GNLK__);
			break;
		case 6:
			//CTreeMenuBarBase::SetModeST(__TMMODE_CMD_THRD_STEP_BEAM__);
			pDlg->ShowChildDlgByID(__TMMODE_CMD_THRD_STEP_WALL__);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}

void CCMThrdDlg::OnBarClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
	CDBDocExtVar::Instance()->GPS_m_nReturnMenuID = 0;
}

void CCMThrdDlg::OnCmdChangeCombo()
{
	UpdateData();
	MakeMenuCombo(m_nFunctionType);
}

void CCMThrdDlg::DlgInit(int nFuncType, int nComboIndex)
{
	MakeMenuCombo(nFuncType);
	int nSize = m_ctrlFunction.GetCount();
	if (nComboIndex > nSize-1)
		return;

	//m_ctrlFunction.SetCurSel(nComboIndex);
	CDlgUtil::CobxSetCurSelItemData(m_ctrlFunction, nComboIndex);
}

void CCMThrdDlg::MakeMenuCombo(int nType)
{
	m_ctrlFunction.ResetContent();

	if (m_nFunctionType == 0)
	{
#if defined(_MGEN)
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_DISP_VEL_ACC_STOR), 10);
#endif
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_DISP_VEL_ACC), 0);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_TRUSS_FORCE_STRESS), 1);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_BEAM_FORCE_STRESS), 2);
#if !defined(_CIVIL_JP)
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_WALL_FORCE), 11);
#endif
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_PLANE_STRESS_FORCE_STRESS), 3);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_PLANE_STRAIN_FORCE_STRESS), 4);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_PLATE_FORCE_STRESS), 5);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_SOLID_FORCE_STRESS), 6);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_NLLINK_DEFROM_FORCE), 7);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_INELASTIC_HINGE_DEFORM_FORCE), 8);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_LOAD_INCREMENT), 9);

#if defined(_MGEN)
		CDlgUtil::CobxSetCurSelItemData(m_ctrlFunction, 10);
#else
		CDlgUtil::CobxSetCurSelItemData(m_ctrlFunction, 0);
#endif
	}
	else
	{
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_TIME_STEP), 0);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_DISP_VEL_ACC), 1);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_TRUSS_FORCE_STRESS), 2);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_BEAM_FORCE_STRESS), 3);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_WALL_FORCE), 6);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_DESIGNATED_DOF), 4);
		CDlgUtil::CobxAddItem(m_ctrlFunction, _LS(IDS_WG_CMD_GENERAL_LINK), 5);
		CDlgUtil::CobxSetCurSelItemData(m_ctrlFunction, 0);
	}
}

void CCMThrdDlg::MakeListHeader()
{
	CString aTitle[] = {_LS(IDS_CMD_THRD_NAME), _LS(IDS_CMD_THRD_TYPE)};
	int nColWidth[] = {50,130};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ctrlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ctrlList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i=0; i<NCOL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ctrlList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMThrdDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
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

void CCMThrdDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;

		switch(nCmd)
		{
		case(UR_THRD_ADD):
		case(UR_THRD_DEL):
		case(UR_THRD_MFD):
		case(UR_THRD_MFS):
			bMFD = TRUE;
			break;
		default:
			break;
		}
	}

	if (bMFD)
		MakeItemEx();
}

void CCMThrdDlg::MakeItemEx()
{
	m_ctrlList.DeleteAllItems();

	T_THRD_D ThrdD;
	CArray<T_THRD_K, T_THRD_K> arKey;
	m_pDoc->m_pAttrCtrl->GetThrdKeyList(arKey);

	int nSize = arKey.GetSize();
	if (nSize == 0)
		return;

	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetThrd(arKey[i], ThrdD);
		m_ctrlList.InsertItem(i, _T(""));
		SetItem(i, arKey[i], ThrdD);
	}
}

void CCMThrdDlg::SetItem(int nIndex, T_THRD_K ThrdK, T_THRD_D& ThrdD)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < NCOL; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, ThrdD);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ctrlList.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_ctrlList.SetItemData(nIndex, (DWORD)ThrdK);
}

CString CCMThrdDlg::DataToStr(int nIndex, T_THRD_D& ThrdD)
{
	CString strName;
	strName = _T("");
	switch(nIndex)
	{
	case 0:
		strName = ThrdD.Name;
		break;
	case 1:
		if (ThrdD.nGraphStep == 0)  //Graph
			strName = GetGraphTitle(ThrdD.nFunctionType);
		else                        //Step
			strName = GetStepTitle(ThrdD.nStepFunction);
	}
	return strName;
}

CString CCMThrdDlg::GetGraphTitle(int nType)
{
	CString str;
	
	switch(nType)
	{
	case 2:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_DISP_VEL_ACC));
		break;
	case 3:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_TRUSS_FORCE_STRESS));
		break;
	case 4:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_BEAM_FORCE_STRESS));
		break;
	case 5:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_PLANE_STRESS_FORCE_STRESS));
		break;
	case 6:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_PLATE_FORCE_STRESS));
		break;
	case 7:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_SOLID_FORCE_STRESS));
		break;
	case 8:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_NLLINK_DEFROM_FORCE));
		break;
	case 9:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_INELASTIC_HINGE_DEFORM_FORCE));
		break;
	case 10:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_PLANE_STRAIN_FORCE_STRESS));
		break;
	case 11:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_LOAD_INCREMENT));
		break;
	case 12:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_DISP_VEL_ACC_STOR));
		break;
	case 13:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_GRAPH), _LS(IDS_WG_CMD_WALL_FORCE));
		break;
	}
	return str;
}

CString CCMThrdDlg::GetStepTitle(int nType)
{
	CString str;

	switch(nType)
	{
	case 1:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_TIME_STEP));
		break;
	case 2:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_DISP_VEL_ACC));
		break;
	case 3:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_TRUSS_FORCE_STRESS));
		break;
	case 4:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_BEAM_FORCE_STRESS));
		break;
	case 5:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_DESIGNATED_DOF));
		break;
	case 6:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_GENERAL_LINK));
	case 7:
		str.Format(_T("%s-%s"), _LS(IDS_WG_CMD_STEP), _LS(IDS_WG_CMD_WALL_FORCE));
		break;
	}
	return str;
}

void CCMThrdDlg::OnModify()
{
	int nIndex;
	if (!GetSelectedIndex(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_NO_SELECTED_THRD));
		return;
	}
	T_THRD_K ThrdK;
	T_THRD_D ThrdD;
	ThrdK = m_ctrlList.GetItemData(nIndex);
	m_pDoc->m_pAttrCtrl->GetThrd(ThrdK, ThrdD);
	
	CCMThMainDlg* pDlg = (CCMThMainDlg*)(GetParent()->GetParent());
	//CBarTreeMenu* pTree = (CBarTreeMenu*)CTreeMenuBarBase::GetMySelfST();
	if (ThrdD.nGraphStep == 0)
	{
		switch(ThrdD.nFunctionType)
		{
		case 2:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_DISP__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_DISP__, ThrdK);
			break;
		case 3:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_TRUSS__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_TRUSS__, ThrdK);
			break;
		case 4:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_BEAM__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_BEAM__, ThrdK);
			break;
		case 5:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_PLANE_STRESS__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_PLANE_STRESS__, ThrdK);
			break;
		case 6:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_PLATE__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_PLATE__, ThrdK);
			break;
		case 7:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_SOLID__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_SOLID__, ThrdK);
			break;
		case 8:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_GENERAL_LINK__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_GENERAL_LINK__, ThrdK);
			break;
		case 9:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_INELASTIC_HINGE__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_INELASTIC_HINGE__, ThrdK);
			break;
		case 10:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_PLANE_STRAIN__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_PLANE_STRAIN__, ThrdK);
			break;
		case 11:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_PLANE_STRAIN__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRC_TIME_INCREMENT__, ThrdK);
			break;
		case 12:
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_DISP_STORY__, ThrdK);
			break;
		case 13:
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_WALL__, ThrdK);
			break;
		}
	}
	else
	{
		switch(ThrdD.nStepFunction)
		{
		case 1:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_TIMESTEP__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_TIMESTEP__, ThrdK);
			break;
		case 2:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_DISPVELACC__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_DISPVELACC__, ThrdK);
			break;
		case 3:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_TRUSS__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_TRUSS__, ThrdK);
			break;
		case 4:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_BEAM__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_BEAM__, ThrdK);
			break;
		case 5:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_DESIGDOF__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_DESIGDOF__, ThrdK);
			break;
		case 6:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_GNLK__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_GNLK__, ThrdK);
			break;
		case 7:
			//pTree->ShowDlgWithInit(__TMMODE_CMD_THRD_STEP_GNLK__, ThrdK);
			pDlg->InitChildDlgByID(__TMMODE_CMD_THRD_STEP_WALL__, ThrdK);
			break;
		}
	}
}

void CCMThrdDlg::OnDelete()
{
	int nIndex = -1;
	int nSelCount = m_ctrlList.GetSelectedCount();

	if (nSelCount < 1)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_NO_SELECTED_THRD));
		return;
	}

	T_THRD_K ThrdK;
	T_THRD_D ThrdD;
	
	POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
	CArray<CString, CString&> rstrName;
	while(pos)
	{
		nIndex = m_ctrlList.GetNextSelectedItem(pos);
		ThrdK = m_ctrlList.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetThrd(ThrdK, ThrdD);
		rstrName.Add(ThrdD.Name);
	}
	m_pDoc->m_pDataCtrl->DelThrd(rstrName);
}

BOOL CCMThrdDlg::GetSelectedIndex(int &nIndex)
{
	int iItem = m_ctrlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMThrdDlg::OnDblclkCmdThrdFuncList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnModify();
	
	*pResult = 0;
}

BOOL CCMThrdDlg::ExternalInit(UINT key)
{
	int nItemCount = m_ctrlList.GetItemCount();
	int i = 0;
	for (i=0; i<nItemCount; i++)
		if (key == m_ctrlList.GetItemData(i)) break;
	if (i == nItemCount) { ASSERT(0); return FALSE; }

	m_ctrlList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	
	OnModify();

	return TRUE;
}


void CCMThrdDlg::OnBack()
{
	if(CDBDocExtVar::Instance())  
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, CDBDocExtVar::Instance()->GPS_m_nReturnMenuID, 0);
	}
	else                          
	{
		ASSERT(0);
	}
}