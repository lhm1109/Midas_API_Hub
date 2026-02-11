// StldTbldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldTbldDlg.h"

#include "..\MIT_frx\MTreeChildImpl.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldTbldDlg dialog


CStldTbldDlg::CStldTbldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldTbldDlg::IDD, pParent)
{
	m_pBitmap = 0;
	m_nBitmap = 0;
	m_bInputMode = FALSE;
	//{{AFX_DATA_INIT(CStldTbldDlg)
	m_nOption = 0;
	m_nProj = 1;
	m_csScale = _LS(IDS_WG_TREEMENU_1_0);
	//}}AFX_DATA_INIT
	m_aLoadTypeCtrl.Add(IDC_TM_LOADTYPE1);
	m_aLoadTypeCtrl.Add(IDC_TM_LOADTYPE2);
	m_aLoadTypeCtrl.Add(IDC_TM_LOADTYPE3);
	m_aLoadTypeCtrl.Add(IDC_TM_LOADTYPE4);
	m_aLoadTypeCtrl.Add(IDC_TM_LOADTYPE5);
	m_aLoadTypeCtrl.Add(IDC_TM_LOADTYPE6);

	m_aLoadValueCtrl.Add(IDC_TM_PICTURE);
	m_aLoadValueCtrl.Add(IDC_TM_FRAME);
	m_aLoadValueCtrl.Add(IDC_TM_FORCE_TITLE);
	m_aLoadValueCtrl.Add(IDC_TM_FORCE_EDIT);
	m_aLoadValueCtrl.Add(IDC_TM_FORCE_UNIT);
	m_aLoadValueCtrl.Add(IDC_TM_SPAN_TITLE);
	m_aLoadValueCtrl.Add(IDC_TM_SPAN_EDIT);
	m_aLoadValueCtrl.Add(IDC_TM_SPAN_UNIT);
	m_aLoadValueCtrl.Add(IDC_TM_SW_TITLE1);
	m_aLoadValueCtrl.Add(IDC_TM_SW_TITLE);
	m_aLoadValueCtrl.Add(IDC_TM_SW_EDIT);
	m_aLoadValueCtrl.Add(IDC_TM_SW_UNIT);
	m_aLoadValueCtrl.Add(IDC_TM_DIR_TITLE);
	m_aLoadValueCtrl.Add(IDC_TM_DIR_COMBO);
	m_aLoadValueCtrl.Add(IDC_TM_CANCEL);

	m_aEtcCtrl.Add(IDC_WG_TREEMENU_STATIC3);
	m_aEtcCtrl.Add(IDC_TM_PROJ_TITLE);
	m_aEtcCtrl.Add(IDC_TM_PROJ_YES);
	m_aEtcCtrl.Add(IDC_TM_PROJ_NO);
	m_aEtcCtrl.Add(IDC_TM_SCALE_TITLE);
	m_aEtcCtrl.Add(IDC_TM_SCALE_EDIT);
}

CStldTbldDlg::~CStldTbldDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CStldTbldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldTbldDlg)
	DDX_Control(pDX, IDC_TM_SW_UNIT, m_wndSWUnit);
	DDX_Control(pDX, IDC_TM_SW_EDIT, m_wndSWEdit);
	DDX_Control(pDX, IDC_TM_SPAN_EDIT, m_wndSpanEdit);
	DDX_Control(pDX, IDC_TM_FORCE_EDIT, m_wndForceEdit);
	DDX_Control(pDX, IDC_TM_SPAN_UNIT, m_wndSpanUnit);
	DDX_Control(pDX, IDC_TM_FORCE_UNIT, m_wndForceUnit);
	DDX_Control(pDX, IDC_TM_SCALE_EDIT, m_wndScaleFactor);
	DDX_Control(pDX, IDC_TM_DIR_COMBO, m_wndDirection);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_PROJ_YES, m_nProj);
	DDX_CBString(pDX, IDC_TM_SCALE_EDIT, m_csScale);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndCurLoadTypePicture);
	DDX_Control(pDX, IDC_TM_LOADTYPE1, m_wndLoadType[0]);
	DDX_Control(pDX, IDC_TM_LOADTYPE2, m_wndLoadType[1]);
	DDX_Control(pDX, IDC_TM_LOADTYPE3, m_wndLoadType[2]);
	DDX_Control(pDX, IDC_TM_LOADTYPE4, m_wndLoadType[3]);
	DDX_Control(pDX, IDC_TM_LOADTYPE5, m_wndLoadType[4]);
	DDX_Control(pDX, IDC_TM_LOADTYPE6, m_wndLoadType[5]);
	//}}AFX_DATA_MAP
}


void CStldTbldDlg::AlignControls()
{
	// TODO: Add your control notification handler code here
	CtrlShowHide(m_aLoadValueCtrl, FALSE);
	int nDistY;
	CWnd *pWnd1, *pWnd2;
	CRect rect1, rect2;
	pWnd1 = GetDlgItem(m_aLoadTypeCtrl[0]);
	pWnd2 = GetDlgItem(m_aLoadValueCtrl[0]);
	pWnd1->GetWindowRect(rect1);
	pWnd2->GetWindowRect(rect2);
	nDistY = rect1.top - rect2.top;
	
	CtrlMoveDistY(m_aLoadValueCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_CLOSE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+5;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CStldTbldDlg::AlignProjectionControls(const bool& bInit)
{
#ifdef _RUS
	// TODO: Add your control notification handler code here
	int nDistY;
	CWnd* pWnd1, * pWnd2;
	CRect rect1, rect2;
	if(bInit)
	{
		pWnd1 = GetDlgItem(IDC_WG_TREEMENU_STATIC2);
	}
	else
	{
		pWnd1 = GetDlgItem(IDC_TM_DIR_COMBO);
	}
	pWnd2 = GetDlgItem(m_aEtcCtrl[0]);
	pWnd1->GetWindowRect(rect1);
	pWnd2->GetWindowRect(rect2);
	nDistY = rect1.bottom - rect2.top + 4;

	CtrlMoveDistY(m_aEtcCtrl, nDistY);
#endif // _RUS
}

void CStldTbldDlg::ChangeBitmap(int nBitmap)
{
	CString aCurLoadTypePictures[] = {
	_T("SVG\\illustration\\Dialog\\tm_blt1.svg"),
	_T("SVG\\illustration\\Dialog\\tm_blt2.svg"),
	_T("SVG\\illustration\\Dialog\\tm_blt3.svg"),
	_T("SVG\\illustration\\Dialog\\tm_blt4.svg"),
	_T("SVG\\illustration\\Dialog\\tm_blt5.svg"),
	_T("SVG\\illustration\\Dialog\\tm_blt6.svg"),
	};

	m_wndCurLoadTypePicture.SetImage(aCurLoadTypePictures[nBitmap]);
}


BEGIN_MESSAGE_MAP(CStldTbldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldTbldDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_LOADTYPE1, OnClickLoadTypeBtn)
	ON_BN_CLICKED(IDC_TM_CANCEL, OnTmCancel)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_LOADTYPE2, OnClickLoadTypeBtn)
	ON_BN_CLICKED(IDC_TM_LOADTYPE3, OnClickLoadTypeBtn)
	ON_BN_CLICKED(IDC_TM_LOADTYPE4, OnClickLoadTypeBtn)
	ON_BN_CLICKED(IDC_TM_LOADTYPE5, OnClickLoadTypeBtn)
	ON_BN_CLICKED(IDC_TM_LOADTYPE6, OnClickLoadTypeBtn)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldTbldDlg message handlers

BOOL CStldTbldDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_wndDirection.ResetContent();
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_x));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_y));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Local_z));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z));

	m_wndScaleFactor.AddString(_LS(IDS_WG_TREEMENU_1_0));
	m_wndScaleFactor.AddString(_LS(IDS_WG_TREEMENU_2_0));

	CString aLoadTypePicture[] = {
	_T("SVG\\illustration\\Dialog\\tm_lt1.svg"),
	_T("SVG\\illustration\\Dialog\\tm_lt2.svg"),
	_T("SVG\\illustration\\Dialog\\tm_lt3.svg"),
	_T("SVG\\illustration\\Dialog\\tm_lt4.svg"),
	_T("SVG\\illustration\\Dialog\\tm_lt5.svg"),
	_T("SVG\\illustration\\Dialog\\tm_lt6.svg"),
	};
	for (int i = 0; i < 6; i++)
	{
		m_wndLoadType[i].SetImage(aLoadTypePicture[i]);
		m_wndLoadType[i].SetCustomizeColor(TRUE);
	}

	m_wndForceEdit.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_wndForceUnit.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_wndSpanEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSpanUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSWEdit.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_wndSWUnit.SetUnitType(D_UNITSYS_BASE_UNITFORCE);

	// Initialize dialog member
	m_nOption = 0;                // Default : Add
	m_wndDirection.SetCurSel(5);  // Default : Global Z
	m_nProj = 1;                  // Default : No
	m_wndScaleFactor.SetCurSel(0); // Default : 1.0

	// Enable/Disable, Show/Hide, Align Controls.
	m_bInputMode = FALSE;

	AlignControls();
	AlignProjectionControls(true);
	//CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
	//pWnd->EnableWindow(FALSE); // Apply를 Disable한다.
	MTreeChildImpl* pParent = dynamic_cast<MTreeChildImpl*>(GetParent()->GetParent());
	pParent->GetExecuteBtn()->EnableWindow(FALSE);

	CtrlEnableDisable(m_aLoadTypeCtrl, m_nOption != 2);
	CtrlEnableDisable(m_aEtcCtrl, FALSE);

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldTbldDlg::OnTmExecute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST(); 
	CArray<UINT, UINT> aSelKey;
	pIGM->GetSelectedElemKeyList(aSelKey);
	T_ELEM_D ElemData;
	
	// Remove element which is not beam type
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}
	if (aSelKey.GetSize() <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_TBLD_D data;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	if (m_nOption == 0 || m_nOption == 1)
	{
		data.LoadCaseKey = nLoadCaseKey;
		data.LoadType = m_nBitmap+1;
		data.LoadDirection = m_wndDirection.GetCurSel()+1;
		data.FloorLoad = m_wndForceEdit.GetEditValue();
		data.Width = m_wndSpanEdit.GetEditValue();
		data.dW = m_wndSWEdit.GetEditValue();
		data.Projection = (m_nProj==0)?1:0;
		if(!GetFloatNumber(m_csScale,data.Factor)) return;
	}

	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		
		T_LDGR_K LdgrK;
		m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
		data.GroupKey = LdgrK;
		

		bSuccess = m_pDoc->m_pDataCtrl->AddTbld(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	}
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelTbld(aSelKey, nLoadCaseKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldTbldDlg::OnTmClose()
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}


void CStldTbldDlg::OnClickLoadTypeBtn() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_LOADTYPE1: m_nBitmap = 0; break;
	case IDC_TM_LOADTYPE2: m_nBitmap = 1; break;
	case IDC_TM_LOADTYPE3: m_nBitmap = 2; break;
	case IDC_TM_LOADTYPE4: m_nBitmap = 3; break;
	case IDC_TM_LOADTYPE5: m_nBitmap = 4; break;
	case IDC_TM_LOADTYPE6: m_nBitmap = 5; break;
	}
	// Button을 숨기고 Value를 Open한다.
	CtrlShowHide(m_aLoadTypeCtrl, FALSE);
	CtrlShowHide(m_aLoadValueCtrl, TRUE);
	CtrlEnableDisable(m_aEtcCtrl, TRUE);
	AlignProjectionControls(false);
	//CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
	m_wndSWEdit.EnableWindow(m_nBitmap >= 3);
	ChangeBitmap(m_nBitmap);
	//pWnd->EnableWindow(TRUE); // Apply를 Enable한다.
	MTreeChildImpl* pParent = dynamic_cast<MTreeChildImpl*>(GetParent()->GetParent());
	pParent->GetExecuteBtn()->EnableWindow(TRUE);
	m_bInputMode = TRUE;

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CStldTbldDlg::OnTmCancel() 
{
	// TODO: Add your control notification handler code here
	CtrlShowHide(m_aLoadValueCtrl, FALSE);
	CtrlShowHide(m_aLoadTypeCtrl, TRUE);
	CtrlEnableDisable(m_aEtcCtrl, FALSE);
	AlignProjectionControls(true);
	//CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
	//pWnd->EnableWindow(FALSE); // Apply를 Disable한다.
	MTreeChildImpl* pParent = dynamic_cast<MTreeChildImpl*>(GetParent()->GetParent());
	pParent->GetExecuteBtn()->EnableWindow(FALSE);
	m_bInputMode = FALSE;

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CStldTbldDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD:
		m_nOption = 0;
		break;
	case IDC_TM_OPT_REPLACE:
		m_nOption = 1;
		break;
	case IDC_TM_OPT_DELETE:
		m_nOption = 2;
		break;
	}

	if (m_nOption == 2)
	{
		CtrlShowHide(m_aLoadValueCtrl, FALSE);
		CtrlShowHide(m_aLoadTypeCtrl, TRUE);
		CtrlEnableDisable(m_aEtcCtrl, FALSE);
		//CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
		//pWnd->EnableWindow(TRUE); // Apply를 Enable한다.
		MTreeChildImpl* pParent = dynamic_cast<MTreeChildImpl*>(GetParent()->GetParent());
		pParent->GetExecuteBtn()->EnableWindow(TRUE);
		m_bInputMode = FALSE;
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else if (!m_bInputMode) // Add, Replace로 바뀔때 Input Mode가 아니면
	{
		//CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
		//pWnd->EnableWindow(FALSE); // Apply를 Disble한다.
		MTreeChildImpl* pParent = dynamic_cast<MTreeChildImpl*>(GetParent()->GetParent());
		pParent->GetExecuteBtn()->EnableWindow(FALSE);
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
	CtrlEnableDisable(m_aLoadTypeCtrl, m_nOption != 2);
}

void CStldTbldDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldTbldDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

