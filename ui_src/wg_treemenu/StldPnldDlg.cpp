// StldPnldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPnldDlg.h"

#include "..\wg_db\flagctrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_I_GenModelBase.h"

#include "StldPnldUCSListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldPnldDlg dialog
#define ELEM_SEL  0
#define GROUP_SEL 1

CStldPnldDlg::CStldPnldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldPnldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldPnldDlg)
	m_sDesc = _T("");
	m_sFstPt = _T("");
	m_sSndPt = _T("");
	m_sTrdPt = _T("");
	m_bNodeDefChk = FALSE;
	m_bCopyLdChk = FALSE;
	m_nAxisRd = -1;
	m_sDist = _T("");
	m_sNodeList = _T("");
	//}}AFX_DATA_INIT
	this->m_nCurDir = -1;
}


void CStldPnldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPnldDlg)
	DDX_Control(pDX, IDC_TM_PROJECT_COMBO, m_cbxPrj);
	DDX_Control(pDX, IDC_TM_STGROUP_COMBO, m_CobxStGroup);
	DDX_Control(pDX, IDC_TM_SELTYPE_COMBO, m_CobxSelType);
	DDX_Control(pDX, IDC_TM_FACE_NO_COMBO, m_CobxFaceNo);
	DDX_Control(pDX, IDC_TM_PNL_TRLC, m_edTrlc);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_cbxLdc);
	DDX_Control(pDX, IDC_TM_PNL_FSTPT, m_edFstPt);
	DDX_Control(pDX, IDC_TM_PNL_TRDPT, m_edTrdPt);
	DDX_Control(pDX, IDC_TM_PNL_SNDPT, m_edSndPt);
	DDX_Control(pDX, IDC_TM_PNL_NODE_LIST, m_edNodeList);
	DDX_Control(pDX, IDC_TM_PNL_LOADTYPE, m_cbxLType);
	DDX_Control(pDX, IDC_TM_PNL_LOAD_DIR, m_cbxLdDir);
	DDX_Control(pDX, IDC_TM_PNL_ELE_TYPE, m_cbxEType);
	DDX_Control(pDX, IDC_TM_PNLD_LDGR_CBX, m_cbxLdgr);
	DDX_Text(pDX, IDC_TM_PNL_DESCRIPTION, m_sDesc);
	DDX_Control(pDX, IDC_TM_PNL_TRLC_UNIT, m_stUnitTrlc);
	DDX_Control(pDX, IDC_TM_PNL_TRDPT_UNIT, m_stUnitTrdPt);
	DDX_Control(pDX, IDC_TM_PNL_SND_UNIT, m_stUnitSnd);
	DDX_Control(pDX, IDC_TM_PNL_FSTPT_UNIT, m_stUnitFstPt);
	DDX_Control(pDX, IDC_TM_PNL_DIST_UNIT, m_stUnitDist);
	DDX_Text(pDX, IDC_TM_PNL_FSTPT, m_sFstPt);
	DDX_Text(pDX, IDC_TM_PNL_SNDPT, m_sSndPt);
	DDX_Text(pDX, IDC_TM_PNL_TRDPT, m_sTrdPt);
	DDX_Check(pDX, IDC_TM_PNL_NODEDEF_CHK, m_bNodeDefChk);
	DDX_Check(pDX, IDC_TM_PNL_COPY_PNLD, m_bCopyLdChk);
	DDX_Radio(pDX, IDC_TM_PNL_AXIS_X, m_nAxisRd);
	DDX_Text(pDX, IDC_TM_PNL_DIST_EDIT, m_sDist);
	DDX_Text(pDX, IDC_TM_PNL_NODE_LIST, m_sNodeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPnldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldPnldDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_PNL_NODEDEF_CHK, OnTmPnlNodedefChk)
	ON_BN_CLICKED(IDC_TM_PNL_COPY_PNLD, OnTmPnlCopyPnld)
	ON_BN_CLICKED(IDC_TM_PNL_UCS_BTN, OnTmPnlUcsBtn)
	ON_BN_CLICKED(IDC_TM_PNL_NMDPLANE_BTN, OnTmPnlNmdplaneBtn)
	ON_BN_CLICKED(IDC_TM_PNL_LOADTYPE_BTN, OnTmPnlLoadtypeBtn)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLc)
	ON_CBN_SELCHANGE(IDC_TM_PNL_LOAD_DIR, OnSelchangeTmPnlLoadDir)
	ON_EN_SETFOCUS(IDC_TM_PNL_NODE_LIST, OnSetfocusTmPnlNodeList)
	ON_CBN_SELCHANGE(IDC_TM_SELTYPE_COMBO, OnSelchangeTmSeltypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_PNL_ELE_TYPE, OnSelchangeTmPnlEleType)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,OnMouseEditEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldPnldDlg message handlers

void CStldPnldDlg::Execute()
{
	OnTmExecute();
}

void CStldPnldDlg::OnTmClose() 
{
	CloseDlg();
}

BOOL CStldPnldDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	InitUnit();
	SetDataToDlg();
	EnableNodeDefLdArea();
	EnableCopyPnldCtrl();
	EnablePrjCbx();
	ElemSelCtrlInit();

	ElemSelCtrlInit();
	ElemSelCtrlMan();

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////
// DATE : 2002.1.24. by KYE-HONG
// FUNC : data exchange
//				
///////////////////////////////////////////////////////////////////
#define ETYPE_PLATE 0
#define ETYPE_SOLID 1
void	CStldPnldDlg::SetDataToDlg()
{
	T_PNLA_D	data;	
	data.Initialize();

	// element type combo box
	m_cbxEType.ResetContent();
	m_cbxEType.AddString(_LS(IDS_WG_TREEMENU_PNLD_Plate));
	m_cbxEType.SetItemData(0,ETYPE_PLATE);
	m_cbxEType.AddString(_LS(IDS_WG_TREEMENU_PNLD_Solid));
	m_cbxEType.SetItemData(1,ETYPE_SOLID);
	m_cbxEType.SetCurSel(0);
	
 // load direction combo box
	m_cbxLdDir.ResetContent();
	m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_0524_Normal_Loading_Plane_));
	m_cbxLdDir.SetItemData(0,0);
	m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_0524_Normal_Element_));
	m_cbxLdDir.SetItemData(1,1);
	m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_cbxLdDir.SetItemData(2,2);
	m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_cbxLdDir.SetItemData(3,3);
	m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
	m_cbxLdDir.SetItemData(4,4);
	m_cbxLdDir.SetCurSel(0);	
	CDlgUtil::CobxAdjustListBoxWidth(m_cbxLdDir);

	// point edit initialize
	InitPtEditValue();
	
												 //MOUSEEDIT_USE_GET_POS
	m_edFstPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);	// 좌표값을 가져옴 
	m_edSndPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);	// 좌표값을 가져옴 
	m_edTrdPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);	// 좌표값을 가져옴 
	m_edFstPt.SetAttWcsPos();		// world coordinate로 함 
	m_edSndPt.SetAttWcsPos();		// world coordinate로 함 
	m_edTrdPt.SetAttWcsPos();		// world coordinate로 함 
	m_edFstPt.SetForceLevel1(TRUE);
	m_edSndPt.SetForceLevel1(TRUE);
	m_edTrdPt.SetForceLevel1(TRUE);

	m_edFstPt.SetNextLink(&m_edSndPt);	// 다음  editor로 forcus이동 
	m_edSndPt.SetNextLink(&m_edTrdPt);	// 다음  editor로 forcus이동 
	m_edTrdPt.SetNextLink(NULL);	    


	
	// node list edit box
	//m_edNodeList.SetNextLink(NULL);
	m_edNodeList.SetAttNodeList();	
	m_edNodeList.SetLButtonDownNotifyWindow(this);
	m_edNodeList.SetEnterNotifyWindow(this);
	m_edNodeList.SetEscNotifyWindow(this);
	m_edNodeList.SetModeToUse(MOUSEEDIT_USE_GET_FLOAD_NODE_LIST);
	m_edNodeList.SetMaxNodeKeyNum(D_PNLA_MAXNOD);	

	// etc control initialize
	//m_sTrlc.Format("%g", m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit());
	m_edTrlc.SetEditUnit(m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit());

	//this->m_nPrj				= 1; 
	m_cbxPrj.AddString(_LS(IDS_WG_TREEMENU_0524_No_));
	m_cbxPrj.SetItemData(0,0);
	m_cbxPrj.AddString(_LS(IDS_WG_TREEMENU_0524_Load_Direction)); 
	m_cbxPrj.SetItemData(1,1);
	m_cbxPrj.AddString(_LS(IDS_WG_TREEMENU_0524_Loading_Plane));
	m_cbxPrj.SetItemData(2,2);
	m_cbxPrj.SetCurSel(0);


	this->m_sDesc				= data.Description;
	this->m_bNodeDefChk = 0;
	this->m_bCopyLdChk	= 0;
	this->m_sDist.Format(_T("0"));
	this->m_nAxisRd			= 2;

#ifdef _CIVIL
	this->m_nAxisRd	= 0;
#endif
	
	UpdateData(FALSE);
}

BOOL	CStldPnldDlg::SetDlgToData()
{
	UpdateData(TRUE);
	
	T_PNLA_D	data;
	T_LDGR_K ldg_key;
	
	data.Initialize();
	
	// load type combo
	if (!this->m_cbxLType.GetSelectedPnld(data.LoadTypeKey))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Load_type_is_not_selected_));
		return FALSE;
	}

	// load type combo
	unsigned int nLoadCaseType, nLoadCaseKey;
	if (!this->m_cbxLdc.GetSelectedLoad(nLoadCaseType, nLoadCaseKey))return FALSE;
	data.LoadCaseKey = nLoadCaseKey;

	//
	if(!this->m_cbxLdgr.GetSelectedLdgr(ldg_key)){return FALSE;}
	data.GroupKey				= ldg_key;	
	data.nElementType		= this->m_cbxEType.GetCurSel();	
	data.nLoadDirection = m_cbxLdDir.GetItemData(m_cbxLdDir.GetCurSel());
	
	/*
	if(this->m_nPrj==1 || this->m_nPrj == 2)	data.nProjection=1;		
	else								data.nProjection=0;			
	*/

	int nPrj = m_cbxPrj.GetItemData(m_cbxPrj.GetCurSel());

	data.nProjection = nPrj;

	data.Description		= this->m_sDesc;
	data.dTolerance			= m_edTrlc.GetEditValue();

	//data.GroupKey;
	//data.FA
	//data.

	this->m_CobxStGroup.GetSelectedGrup(data.ElemGroupKey);
	data.nFaceNo = (int)this->m_CobxFaceNo.GetItemData(m_CobxFaceNo.GetCurSel());
	data.nElemSelection = (int)this->m_CobxSelType.GetItemData(this->m_CobxSelType.GetCurSel());
	
	/*
	T_GRUP_K  ElemGroupKey;
	int       nFaceNo;        // 1-6
	*/
	
	
	


	
	// Point orgin,x-Axis,y-Axis
	MyParser Parser;
	CArray<double, double> XYZ;
	if(FALSE==Parser.ParsingPositionArg(m_sFstPt))   
		return FALSE; 
	if(FALSE==Parser.GetPositionData(data.dPoint[0][0],data.dPoint[0][1],data.dPoint[0][2]))
		return FALSE; 

	if(FALSE==Parser.ParsingPositionArg(m_sSndPt))   
		return FALSE; 
	if(FALSE==Parser.GetPositionData(data.dPoint[1][0],data.dPoint[1][1],data.dPoint[1][2]))
		return FALSE; 

	if(FALSE==Parser.ParsingPositionArg(m_sTrdPt))   
		return FALSE; 
	if(FALSE==Parser.GetPositionData(data.dPoint[2][0],data.dPoint[2][1],data.dPoint[2][2]))
		return FALSE; 

	

	/*
	if(!CStrParser::GetUEDistance(this->m_sFstPt,XYZ))return FALSE;
	if(XYZ.GetSize()!=3) return FALSE;
	data.dPoint[0][0] = XYZ[0];
	data.dPoint[0][1] = XYZ[1];
	data.dPoint[0][2] = XYZ[2];
	XYZ.RemoveAll();

	if(!CStrParser::GetUEDistance(this->m_sSndPt,XYZ))return FALSE;
	if(XYZ.GetSize()!=3) return FALSE;
	data.dPoint[1][0] = XYZ[0];
	data.dPoint[1][1] = XYZ[1];
	data.dPoint[1][2] = XYZ[2];
	XYZ.RemoveAll();

	if(!CStrParser::GetUEDistance(this->m_sTrdPt,XYZ))return FALSE;
	if(XYZ.GetSize()!=3) return FALSE;
	data.dPoint[2][0] = XYZ[0];
	data.dPoint[2][1] = XYZ[1];
	data.dPoint[2][2] = XYZ[2];
	XYZ.RemoveAll();
	*/
	
	// node defining load area
	BOOL is_defchk;
	CArray<double,double> ardNode;
	CArray<T_NODE_K,T_NODE_K> arnNode;

	is_defchk					= this->m_bNodeDefChk;
	data.bLoadingArea = is_defchk;
	if(is_defchk)
	if(!CStrParser::GetUEDistance(this->m_sNodeList,ardNode))return FALSE;
	for(int i=0; i<ardNode.GetSize(); i++) arnNode.Add((T_NODE_K)(ardNode[i])); 
	if(arnNode.GetSize()>D_PNLA_MAXNOD)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_PNLD_NodeArea_Err));
		return FALSE;
	}
	for(int i=0; i<arnNode.GetSize(); i++)data.nConnectivity[i] = arnNode[i];
	arnNode.RemoveAll();
	ardNode.RemoveAll(); 
	
	// copy plane load
	BOOL is_copy;
	double ux=0.0,uy=0.0,uz=0.0;
	CArray<double,double> arDist;
	
	is_copy = this->m_bCopyLdChk;
	if(is_copy)
	{
		if(!I_GENModelBase::GetCurMySelfST())ASSERT(FALSE);
		switch(this->m_nAxisRd)
		{
		case 0:
			I_GENModelBase::GetCurMySelfST()->GetUcsVector(1,0,0,ux,uy,uz);
			break;
		case 1:	
			I_GENModelBase::GetCurMySelfST()->GetUcsVector(0,1,0,ux,uy,uz);
			break;
		case 2:	
			I_GENModelBase::GetCurMySelfST()->GetUcsVector(0,0,1,ux,uy,uz);
			break;
		}
	  if(!CStrParser::GetUEDistance(this->m_sDist,arDist))return FALSE;
	}
	
	if(!m_pDoc->m_pDataCtrl->AddPnla(data,is_copy,ux,uy,uz,arDist))
	{
		return FALSE;
	}

	return TRUE;
}

void CStldPnldDlg::InitPtEditValue()
{
	m_sFstPt = _LS(IDS_WG_TREEMENU_0___0___0);
	m_sSndPt = _LS(IDS_WG_TREEMENU_0___0___0);
	m_sTrdPt = _LS(IDS_WG_TREEMENU_0___0___0);
}

///////////////////////////////////////////////////////////////////
// DATE : 2002.1.24. by KYE-HONG
// FUNC : mouse edit의 message처리 
//				주로 화면 update관련 메세지 처리 
///////////////////////////////////////////////////////////////////
LRESULT CStldPnldDlg::OnMouseEditEscape(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(pIGM)
	{
		pIGM->EndTempFLoad();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		m_edNodeList.ClearNodeKeyList();
		pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
	}
	return 0L;
}

LRESULT CStldPnldDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(pIGM)
	{
		//BOOL bRetVal = SetDlgToData();
		pIGM->EndTempFLoad();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
		m_edNodeList.ClearNodeKeyList();
	}
	return 0L;
}

LRESULT CStldPnldDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	if(pIGM)
	{
		if(pIGM->IsCompleteTempFLoad()) // 같은 점 
		{
			//BOOL bRetVal = SetDlgToData();
			pIGM->EndTempFLoad();
			//m_edNodeList.ClearNodeKeyList();
			pIGM->SetModelingModeST(IG_STATE_INITALL);
			pIGM->SetModelingModeST(IG_STATE_GET_FLOAD_NODE_LIST);
			
		}
	}
	return 0L;
}

void	CStldPnldDlg::EnableCopyPnldCtrl(void)
{
	CWnd *pWnd[4];
	
	UpdateData(TRUE);
	pWnd[0] = GetDlgItem(IDC_TM_PNL_AXIS_X);
	pWnd[1] = GetDlgItem(IDC_TM_PNL_AXIS_Y);
	pWnd[2] = GetDlgItem(IDC_TM_PNL_AXIS_Z);
	pWnd[3] = GetDlgItem(IDC_TM_PNL_DIST_EDIT);

	if(m_bCopyLdChk)for(int i=0; i<4; i++) pWnd[i]->EnableWindow(TRUE);
	else						for(int i=0; i<4; i++) pWnd[i]->EnableWindow(FALSE);	

}

void	CStldPnldDlg::EnableNodeDefLdArea(void)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_TM_PNL_NODE_LIST);

	UpdateData(TRUE);
	if(m_bNodeDefChk)	pWnd->EnableWindow(TRUE);
	else							pWnd->EnableWindow(FALSE);	
}

void CStldPnldDlg::OnTmPnlNodedefChk() 
{
	EnableNodeDefLdArea();
}

void CStldPnldDlg::OnTmPnlCopyPnld() 
{
	EnableCopyPnldCtrl();
}

///////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.24. by KYE-HONG
// FUNC : 
///////////////////////////////////////////////////////////////////////////
void CStldPnldDlg::OnTmPnlUcsBtn() 
{
	CRect rect;
	CArray<CString, CString&> ucs_list;
	CStldPnldUCSListDlg *pDlg = new CStldPnldUCSListDlg(this);
	ASSERT(pDlg);

	// ucs named plane정보를 얻어옴 
	this->m_pDoc->m_pAttrCtrl->GetNucsList(ucs_list); 
	pDlg->SetList(ucs_list);

	// selection list dialog를 띄움 
	CWnd *pBtn = GetDlgItem(IDC_TM_PNL_UCS_BTN);
	pBtn->GetWindowRect(&rect);
	pDlg->SetDlgData(0,rect.left,rect.bottom);
	pDlg->Create(CStldPnldUCSListDlg::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

void CStldPnldDlg::OnTmPnlNmdplaneBtn() 
{
	CRect rect;
	CArray<CString, CString&> name_list;
	
	CStldPnldUCSListDlg *pDlg = new CStldPnldUCSListDlg(this);
	ASSERT(pDlg);

	// named plane정보를 얻어옴 
	this->m_pDoc->m_pAttrCtrl->GetNplnList(name_list); 
	pDlg->SetList(name_list);
	
	// selection list dialog를 띄움 
	CWnd *pBtn = GetDlgItem(IDC_TM_PNL_NMDPLANE_BTN);
	pBtn->GetWindowRect(&rect);
	pDlg->SetDlgData(1,rect.left,rect.bottom);
	pDlg->Create(CStldPnldUCSListDlg::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.25. by KYE-HONG
// FUNC : Named UCS dlg가 닫힌후. 선택된.. list에 맞게.. update
///////////////////////////////////////////////////////////////////////////
void CStldPnldDlg::DestroyUCSDlg(CString sel_list)
{
	CString str;
	double p[3][3];
	T_NUCS_D Data;

	// error check
	if(sel_list.IsEmpty()) return;
	if(!this->m_pDoc->m_pAttrCtrl->GetNucs(sel_list,Data))return; 
	
	// update data
	CalP3CoorNamedUCS(p,Data);
	this->m_sFstPt.Format(_T("%g, %g, %g"),p[0][0],p[0][1],p[0][2]);
	this->m_sSndPt.Format(_T("%g, %g, %g"),p[1][0],p[1][1],p[1][2]);
	this->m_sTrdPt.Format(_T("%g, %g, %g"),p[2][0],p[2][1],p[2][2]);
	
	UpdateData(FALSE);
}

void CStldPnldDlg::DestroyNPLNDlg(CString sel_list)
{
	CString str;
	double p[3][3];
	T_NPLN_D Data;

	// error check
	if(sel_list.IsEmpty()) return;
	if(!this->m_pDoc->m_pAttrCtrl->GetNpln(sel_list,Data))return; 
	
	// update data
	CalP3CoorNPLN(p,Data);
	//this->m_sFstPt.Format("%g, %g, %g",p[0][0],p[0][1],p[0][2]);
	//this->m_sSndPt.Format("%g, %g, %g",p[1][0],p[1][1],p[1][2]);
	//this->m_sTrdPt.Format("%g, %g, %g",p[2][0],p[2][1],p[2][2]);
	str.Format(_T("%g, %g, %g"),p[0][0],p[0][1],p[0][2]);
	this->m_edFstPt.SetWindowText(str);
	str.Format(_T("%g, %g, %g"),p[1][0],p[1][1],p[1][2]);
	this->m_edSndPt.SetWindowText(str);
	str.Format(_T("%g, %g, %g"),p[2][0],p[2][1],p[2][2]);
	this->m_edTrdPt.SetWindowText(str);
	
	//UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.25. by KYE-HONG
// FUNC : Name UCS와 Named Plane data를 세점정보로 바꾼다. 
///////////////////////////////////////////////////////////////////////////
void CStldPnldDlg::CalP3CoorNamedUCS(double p[3][3],T_NUCS_D &data)
{
	int			i,j;
	double	tp[3],len;
	for( i=0; i<3; i++)for(j=0; j<3; j++)p[i][j]=0.0;

	// origin point
	for( i=0; i<3; i++)p[0][i] = data.dOrg[i];

	// x-Axis
	for( i=0; i<3; i++)tp[i]		= data.dVx[i];			
	len = sqrt(tp[0]*tp[0]+tp[1]*tp[1]+tp[2]*tp[2]);			// vector길이 
	if(len==0.0) return;	for( i=0; i<3; i++)tp[i]/= len;	// 단위 vector
	for( i=0; i<3; i++)tp[i]		= data.dOrg[i] + tp[i];			// 실제 point coordinate
	for( i=0; i<3; i++)p[1][i]	= tp[i];

	// y-Axis
	for( i=0; i<3; i++)tp[i]		= data.dVy[i];			
	len = sqrt(tp[0]*tp[0]+tp[1]*tp[1]+tp[2]*tp[2]);			// vector길이 
	if(len==0.0) return;	for( i=0; i<3; i++)tp[i]/= len;	// 단위 vector
	for( i=0; i<3; i++)tp[i]		= data.dOrg[i] + tp[i];			// 실제 point coordinate
	for( i=0; i<3; i++)p[2][i]	= tp[i];
}

void CStldPnldDlg::CalP3CoorNPLN(double p[3][3],T_NPLN_D &data)
{
	int			i,j;
	double	val;
	for(int i=0; i<3; i++)for(j=0; j<3; j++)p[i][j]=0.0;
	
	val = data.dCoor;
	switch(data.nPlaneType)
	{
		case 1:		// 3 point	
			for( i=0; i<3; i++)for(j=0; j<3; j++) p[i][j]=data.dPoint[i][j];
			break;
		case 2:		// X-Y
			p[0][0]=0; p[0][1]=0; p[0][2]=val; 
			p[1][0]=1; p[1][1]=0; p[1][2]=val; 
			p[2][0]=0; p[2][1]=1; p[2][2]=val; 
			break;
		case 3:		// X-Z
			p[0][0]=0; p[0][1]=val; p[0][2]=0; 
			p[1][0]=1; p[1][1]=val; p[1][2]=0; 
			p[2][0]=0; p[2][1]=val; p[2][2]=1; 
			break;
		case 4:		// Y-Z
			p[0][0]=val; p[0][1]=0; p[0][2]=0; 
			p[1][0]=val; p[1][1]=1; p[1][2]=0; 
			p[2][0]=val; p[2][1]=0; p[2][2]=1; 
			break;
	}
}

void CStldPnldDlg::OnTmPnlLoadtypeBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_BLOAD_DEFPNLD, 0));
}

void CStldPnldDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

void CStldPnldDlg::OnTmExecute() 
{
	this->SetDlgToData();
}


void CStldPnldDlg::EnablePrjCbx(void)
{
	 

	//CWnd *pWnd = GetDlgItem(IDC_TM_PNL_LOAD_DIR);
	/*
	CWnd *pWnd1 = GetDlgItem(IDC_TM_PNL_PROJ_YES);
	CWnd *pWnd2 = GetDlgItem(IDC_TM_PNL_PROJ_NO);
	UpdateData(TRUE);

	int nSelT = m_CobxSelType.GetCurSel();
	if(nSelT < 0) return;

	int SelT = m_CobxSelType.GetItemData(nSelT);

	int nSel = m_cbxLdDir.GetCurSel();
	if(nSel < 0) return;

	if(GROUP_SEL != SelT)
	{
		switch(m_cbxLdDir.GetItemData(nSel))
		{
			case 1:	  // Normal 
				{
				  m_nPrj = 0;
					((CButton*)pWnd1)->SetCheck(BST_CHECKED);
					((CButton*)pWnd2)->SetCheck(BST_UNCHECKED);
					pWnd1->EnableWindow(TRUE);	
				  pWnd2->EnableWindow(FALSE);	
				}
				break;
			case 2:	  // GX
			case 3:	  // GY 
			case 4:	  // GZ
				pWnd1->EnableWindow(TRUE);	
				pWnd2->EnableWindow(TRUE);	
				break;
		}
	}
	*/
}

/*
void CStldPnldDlg::OnTmPnlProjNo() 
{
	// EnablePrjCbx();
}

void CStldPnldDlg::OnTmPnlProjYes() 
{
	// EnablePrjCbx();
}
*/

void CStldPnldDlg::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldPnldDlg::OnSelchangeTmPnlLoadDir() 
{
	EnablePrjCbx();		
}

void CStldPnldDlg::OnSetfocusTmPnlNodeList() 
{
	m_edNodeList.ClearNodeKeyList();	
}


void CStldPnldDlg::InitUnit(void)
{
	m_stUnitTrlc.SetUnitType(CUnitCtrl::m_PNLA_UNIT.dTolerance);
	m_stUnitTrdPt.SetUnitType(CUnitCtrl::m_PNLA_UNIT.dPoint);
	m_stUnitSnd.SetUnitType(CUnitCtrl::m_PNLA_UNIT.dPoint);
	m_stUnitFstPt.SetUnitType(CUnitCtrl::m_PNLA_UNIT.dPoint);
	m_stUnitDist.SetUnitType(CUnitCtrl::m_PNLA_UNIT.dPoint);

	this->m_edTrlc.SetUnitType(CUnitCtrl::m_PNLA_UNIT.dTolerance);
}

void CStldPnldDlg::ElemSelCtrlInit()
{
	m_CobxSelType.ResetContent();
	m_CobxSelType.AddString(_LS(IDS_WG_TREEMENU_0524_Elements_on_the_Loading_));
	m_CobxSelType.SetItemData(0,ELEM_SEL);
	m_CobxSelType.AddString(_LS(IDS_WG_TREEMENU_0524_Group));
	m_CobxSelType.SetItemData(1,GROUP_SEL);

	m_CobxFaceNo.ResetContent();
	m_CobxFaceNo.AddString(_LS(IDS_WG_TREEMENU_Face__1 )); m_CobxFaceNo.SetItemData(0,1);
	m_CobxFaceNo.AddString(_LS(IDS_WG_TREEMENU_Face__2 )); m_CobxFaceNo.SetItemData(1,2);
	m_CobxFaceNo.AddString(_LS(IDS_WG_TREEMENU_Face__3 )); m_CobxFaceNo.SetItemData(2,3);
	m_CobxFaceNo.AddString(_LS(IDS_WG_TREEMENU_Face__4 )); m_CobxFaceNo.SetItemData(3,4);
	m_CobxFaceNo.AddString(_LS(IDS_WG_TREEMENU_Face__5 )); m_CobxFaceNo.SetItemData(4,5);
	m_CobxFaceNo.AddString(_LS(IDS_WG_TREEMENU_Face__6 )); m_CobxFaceNo.SetItemData(5,6);

	m_CobxSelType.SetCurSel(0);
	m_CobxFaceNo.SetCurSel(0);
}

void CStldPnldDlg::ElemSelCtrlMan()
{
	UpdateData(TRUE);
	int nSel = m_CobxSelType.GetCurSel();
	int nDir = (int)m_CobxSelType.GetItemData(nSel);
	
	
	if(m_nCurDir != nDir)
	{
		m_nCurDir = nDir;
	
		/*
		m_cbxLdDir.ResetContent();
		
		m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Normal));
		m_cbxLdDir.SetItemData(0,1);
		
		if(nDir == ELEM_SEL)
		{
			m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Global_X));
			m_cbxLdDir.SetItemData(m_cbxLdDir.GetCount()-1,2);
			m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
			m_cbxLdDir.SetItemData(m_cbxLdDir.GetCount()-1,3);
			m_cbxLdDir.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
			m_cbxLdDir.SetItemData(m_cbxLdDir.GetCount()-1,4);
		}

		if(nDir == ELEM_SEL)
		  m_cbxLdDir.SetCurSel(3);	
		else
			m_cbxLdDir.SetCurSel(0);

		if(nDir == GROUP_SEL)
		{
		
			m_nPrj = 0;
			CButton* pBtn;
			pBtn = (CButton*)GetDlgItem(IDC_TM_PNL_PROJ_YES);
			if(pBtn)
			{
				pBtn->SetCheck(BST_CHECKED);
				pBtn->EnableWindow(TRUE);
			}

			pBtn = (CButton*)GetDlgItem(IDC_TM_PNL_PROJ_NO);
			if(pBtn)
			{
				pBtn->SetCheck(BST_UNCHECKED);
				pBtn->EnableWindow(FALSE);
			}
		
		}
		else
		{
		
			CWnd*pWnd = GetDlgItem(IDC_TM_PNL_PROJ_NO);
			if(pWnd)
				pWnd->EnableWindow(TRUE);
		
		}
		*/

		if(nDir == GROUP_SEL)
		{
			m_bCopyLdChk  = FALSE;
			m_bNodeDefChk = FALSE;
			CButton *pBtn;
			pBtn = (CButton*)GetDlgItem(IDC_TM_PNL_NODEDEF_CHK);
			if(pBtn)
				pBtn->EnableWindow(FALSE);
			pBtn = (CButton*)GetDlgItem(IDC_TM_PNL_COPY_PNLD);
			if(pBtn)
				pBtn->EnableWindow(FALSE);
		}
		else
		{

			CButton *pBtn;
			pBtn = (CButton*)GetDlgItem(IDC_TM_PNL_NODEDEF_CHK);
			if(pBtn)
				pBtn->EnableWindow(TRUE);
			pBtn = (CButton*)GetDlgItem(IDC_TM_PNL_COPY_PNLD);
			if(pBtn)
				pBtn->EnableWindow(TRUE);

		}
		EnableNodeDefLdArea();
	  EnableCopyPnldCtrl();
	}

	int nEType = (int)m_cbxEType.GetItemData(m_cbxEType.GetCurSel());
		
	if(nDir ==  GROUP_SEL)
	{
		if(nEType == ETYPE_SOLID)
			m_CobxFaceNo.EnableWindow(TRUE);
		else
			m_CobxFaceNo.EnableWindow(FALSE);

		m_CobxStGroup.EnableWindow(TRUE);
	}
	else
	{
		m_CobxStGroup.EnableWindow(FALSE);
		m_CobxFaceNo.EnableWindow(FALSE);
	}
}


void CStldPnldDlg::OnSelchangeTmSeltypeCombo() 
{
	ElemSelCtrlMan();
}

void CStldPnldDlg::OnSelchangeTmPnlEleType() 
{
	ElemSelCtrlMan();
}
