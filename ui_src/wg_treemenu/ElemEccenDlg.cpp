// ElemEccenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemEccenDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_db\ClassElem.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemEccenDlg dialog


CElemEccenDlg::CElemEccenDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemEccenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemEccenDlg)
	//}}AFX_DATA_INIT

	m_DisableOnMove.Add(IDC_TM_COPY_NODEATTR_CHECK );
	m_DisableOnMove.Add(IDC_TM_COPY_ELEMATTR_CHECK );
	m_DisableOnMove.Add(IDC_TM_COPYNODEDATA);
	m_DisableOnMove.Add(IDC_TM_COPYELEMDATA);
	m_DisableOnMove.Add(IDC_TM_MATL_INC_TITLE);
	m_DisableOnMove.Add(IDC_TM_MATL_INC_EDIT);
	//m_DisableOnMove.Add(IDC_TM_MATL_INC_SPIN);
	m_DisableOnMove.Add(IDC_TM_SECT_INC_TITLE);
	m_DisableOnMove.Add(IDC_TM_SECT_INC_EDIT);
	//m_DisableOnMove.Add(IDC_TM_SECT_INC_SPIN);
	m_DisableOnMove.Add(IDC_TM_MATL_INC_REP);
	m_DisableOnMove.Add(IDC_TM_SECT_INC_REP);
	m_DisableOnMove.Add(IDC_TM_THIK_INC_REP);
	// MNET1742-JBSEON-050926
	//m_DisableOnMove.Add(IDC_TM_TEXT_BNGR);
	//m_DisableOnMove.Add(IDC_TM_COBX_BNGR);
	//m_DisableOnMove.Add(IDC_TM_BTN_BNGR);

	m_aCtrlCopyMove.Add(IDC_TM_COPY_RADIO);
	m_aCtrlCopyMove.Add(IDC_TM_MOVE_RADIO);
}


void CElemEccenDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemEccenDlg)
	DDX_Control(pDX, IDC_TM_UNIT_GDZJ, m_unitGDZj);
	DDX_Control(pDX, IDC_TM_UNIT_GDZI, m_unitGDZi);
	DDX_Control(pDX, IDC_TM_UNIT_GDYJ, m_unitGDYj);
	DDX_Control(pDX, IDC_TM_UNIT_GDYI, m_unitGDYi);
	DDX_Control(pDX, IDC_TM_UNIT_GDXJ, m_unitGDXj);
	DDX_Control(pDX, IDC_TM_UNIT_GDXI, m_unitGDXi);
	DDX_Control(pDX, IDC_TM_EDIT_GDZJ, m_editGDZj);
	DDX_Control(pDX, IDC_TM_EDIT_GDZI, m_editGDZi);
	DDX_Control(pDX, IDC_TM_EDIT_GDYJ, m_editGDYj);
	DDX_Control(pDX, IDC_TM_EDIT_GDYI, m_editGDYi);
	DDX_Control(pDX, IDC_TM_EDIT_GDXJ, m_editGDXj);
	DDX_Control(pDX, IDC_TM_EDIT_GDXI, m_editGDXi);
	DDX_Control(pDX, IDC_TM_SECT_INC_REP, m_wndSectIncRep);
	DDX_Control(pDX, IDC_TM_MATL_INC_REP, m_wndMatlIncRep);
	DDX_Control(pDX, IDC_TM_SECT_INC_EDIT, m_wndSectInc);
	DDX_Control(pDX, IDC_TM_MATL_INC_EDIT, m_wndMatlInc);
	DDX_Control(pDX, IDC_TM_COPY_NODEATTR_CHECK, m_chkCopyNodeAttr);
	DDX_Control(pDX, IDC_TM_COPY_ELEMATTR_CHECK, m_chkCopyElemAttr);
	DDX_Control(pDX, IDC_TM_COBX_BNGR, m_cobxBngr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElemEccenDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CElemEccenDlg)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyRadio)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmMoveRadio)
	ON_BN_CLICKED(IDC_TM_COPYNODEDATA, OnTmCopynodedata)
	ON_BN_CLICKED(IDC_TM_COPYELEMDATA, OnTmCopyelemdata)
	ON_BN_CLICKED(IDC_TM_BTN_BNGR, OnTmBtnBngr)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_MATL_INC_SPIN, OnDeltaposTmIncSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_SECT_INC_SPIN, OnDeltaposTmIncSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemEccenDlg message handlers

BOOL CElemEccenDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editGDXi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editGDYi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editGDZi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editGDXj.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editGDYj.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editGDZj.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGDXi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGDYi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGDZi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGDXj.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGDYj.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGDZj.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editGDXi.SetEditUnit(0.0);
	m_editGDYi.SetEditUnit(0.0);
	m_editGDZi.SetEditUnit(0.0);
	m_editGDXj.SetEditUnit(0.0);
	m_editGDYj.SetEditUnit(0.0);
	m_editGDZj.SetEditUnit(0.0);

	m_wndMatlInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndMatlInc.SetValue(0);
	m_wndSectInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndSectInc.SetValue(0);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCopyMove, 0);
	CtrlEnableDisable(m_DisableOnMove,TRUE);

	m_chkCopyNodeAttr.SetCheck(1);
	m_chkCopyElemAttr.SetCheck(1);

	m_wndMatlIncRep.ShowWindow(SW_HIDE);  // ÇÊ¿ä ¾ø¾î¼­ ¼û±è
	m_wndSectIncRep.ShowWindow(SW_HIDE);  // ÇÊ¿ä ¾ø¾î¼­ ¼û±è

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemEccenDlg::OnTmCopyRadio() 
{
	CtrlEnableDisable(m_DisableOnMove,TRUE);
}

void CElemEccenDlg::OnTmMoveRadio() 
{
	CtrlEnableDisable(m_DisableOnMove,FALSE);
}

void CElemEccenDlg::OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const MSG* pMsg = GetCurrentMessage();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int NIter;
	
	CString strIter;

	CFormulaEditSpin* pIterNum;
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_MATL_INC_SPIN: pIterNum = &m_wndMatlInc; break;
	case IDC_TM_SECT_INC_SPIN: pIterNum = &m_wndSectInc; break;
	default: ASSERT(0);
	}

	pIterNum->GetEditValue(NIter);
	NIter -= pNMUpDown->iDelta;
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	pIterNum->SetWindowText(strIter);
	*pResult = 0;
}

void CElemEccenDlg::OnTmCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}

void CElemEccenDlg::OnTmCopyelemdata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_ELEM,0));
}

void CElemEccenDlg::OnTmBtnBngr()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD);
}

void CElemEccenDlg::Execute()
{
	// Get Input Value
	int nCopyMove;   // 0:Copy, 1:Move
	CArray<T_ELEM_K, T_ELEM_K> aElemK;  // Selected Element Keys
	double dGDxyz[6]; // x, y, z for i and j
	int nMatlInc;
	int nSectInc;
	BOOL bMatlIncRep;
	BOOL bSectIncRep;
	BOOL bCopyNodeAttr;
	BOOL bCopyElemAttr;
	BOOL bMergeNode = TRUE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aElemK);
	T_ELEM_D ElemD;
	int nCountElem = aElemK.GetSize();
	for (int i = nCountElem-1; i >= 0; i--)
	{
		pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
		if (!pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			aElemK.RemoveAt(i);
	}
	if (aElemK.GetSize() == 0)
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_frame_element_));
		return;    
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCopyMove, nCopyMove);
	if (nCopyMove == 0)
	{
		m_wndMatlInc.GetEditValue(nMatlInc);
		bMatlIncRep = m_wndMatlIncRep.GetCheck() != 0;
		m_wndSectInc.GetEditValue(nSectInc);
		bSectIncRep = m_wndSectIncRep.GetCheck() != 0;
	}
	else
	{
		nMatlInc = 0;
		bMatlIncRep = FALSE;
		nSectInc = 0;
		bSectIncRep = FALSE;
	}
	bCopyNodeAttr = (m_chkCopyNodeAttr.GetCheck() != 0);
	bCopyElemAttr = (m_chkCopyElemAttr.GetCheck() != 0);

	dGDxyz[0] = m_editGDXi.GetEditValue();
	dGDxyz[1] = m_editGDYi.GetEditValue();
	dGDxyz[2] = m_editGDZi.GetEditValue();
	dGDxyz[3] = m_editGDXj.GetEditValue();
	dGDxyz[4] = m_editGDYj.GetEditValue();
	dGDxyz[5] = m_editGDZj.GetEditValue();

	T_BNGR_K BngrK;
	if (!m_cobxBngr.GetSelectedBngr(BngrK)) BngrK = 0;

	// Execute 
	CClassElem* pElemC = M_GetClass2(Elem);
	BOOL bCreated = pElemC->CreateEccentricElements(nCopyMove==0, aElemK, dGDxyz, 
																	nMatlInc, bMatlIncRep, 
																	nSectInc, bSectIncRep, 
																	BngrK,
																	bCopyNodeAttr, bCopyElemAttr, bMergeNode);
	if (bCreated) pDoc->m_pViewCtrl->UnselectAll(NULL);
}