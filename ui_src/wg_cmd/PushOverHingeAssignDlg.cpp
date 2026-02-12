// PushOverHingeAssignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PushOverHingeAssignDlg.h"


#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_common\wg_common_Query.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlg dialog

//IMPLEMENT_DYNCREATE(CPushOverHingeAssignDlg, CCMChildBarBase)

CPushOverHingeAssignDlg::CPushOverHingeAssignDlg()
	: CCMChildBarBase(CPushOverHingeAssignDlg::IDD)
{
	//{{AFX_DATA_INIT(CPushOverHingeAssignDlg)
	m_nLocation = 0;
	m_nElemType = 0;
	m_nCommandType = 0;
	//}}AFX_DATA_INIT
	m_nHingeType = 0;
}


void CPushOverHingeAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPushOverHingeAssignDlg)
	DDX_Control(pDX, IDC_PUSHOVER_ASSIGN_HINGETYPE    , m_wndHingeTypeInform);
	DDX_Control(pDX, IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB, m_cmbHingeType      );
	DDX_Control(pDX, IDC_PUSHOVER_NLNK_STC            , m_stcNlnkInput      );  
	DDX_Control(pDX, IDC_PUSHOVER_NLNK_EDT            , m_edtNlnkInput      );  
	DDX_Control(pDX, IDC_GLINK_RDO1                   , m_rdoGlink1      );  
	DDX_Control(pDX, IDC_GLINK_RDO2                   , m_rdoGlink2      );  
	DDX_Control(pDX, IDC_GLINK_CMB                    , m_cmbGlink       );  
	DDX_Control(pDX, IDC_GLINK_BTN                    , m_btnGlink       );  
	DDX_Radio(pDX, IDC_PUSHOVER_ASSIGN_LOC_I, m_nLocation);
	DDX_Radio(pDX, IDC_PUSHOVER_ELEMTYPE_BEAM, m_nElemType);
	DDX_Radio(pDX, IDC_PUSHOVER_ASSIGN_ADDREPL, m_nCommandType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPushOverHingeAssignDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CPushOverHingeAssignDlg)
	ON_CBN_SELCHANGE(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB , OnSelchangePushoverAssignHingetypeCmb)
	ON_BN_CLICKED(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN, OnPushoverAssignHingetypelistBtn)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_BEAM           , OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_TRUSS          , OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_WALL           , OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_NLNK           , OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ELEMTYPE_PSSP           , OnPushOverElemType)
	ON_BN_CLICKED(IDC_PUSHOVER_ASSIGN_DELETE           , OnPushoverAssignCmd)
	ON_BN_CLICKED(IDC_PUSHOVER_ASSIGN_ADDREPL          , OnPushoverAssignCmd)
	ON_BN_CLICKED(IDC_GLINK_RDO1                       , OnPushoverGLinkSelRdo)
	ON_BN_CLICKED(IDC_GLINK_RDO2                       , OnPushoverGLinkSelRdo)
	ON_BN_CLICKED(IDC_GLINK_BTN                        , OnPushoverGLinkPropBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	//ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlg message handlers

/*
void CPushOverHingeAssignDlg::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMChildBarBase::OnInitialUpdate();

#if defined(_CIVIL)
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(FALSE);
#else
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(TRUE);
#endif

	OnSelchangePushoverAssignHingetypeCmb();
}
*/

LRESULT CPushOverHingeAssignDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_edtNlnkInput.GetSafeHwnd() == (HWND)lParam)
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_edtNlnkInput.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			return 0L;
		//back=OnApply();
		//back=m_edtNlnkInput.ClearContents();
	}
	return 0L;  
}

LRESULT CPushOverHingeAssignDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_edtNlnkInput.GetSafeHwnd() == (HWND)lParam)
	{
		Execute();
	}
	return 0L;
}

BOOL CPushOverHingeAssignDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMChildBarBase::OnInitDialog();	 

#if defined(_CIVIL)
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(FALSE);
	int nOffsetDistUp = globalUtils.ScaleByDPI(-24);
	CArray<UINT,UINT> anCtrlsUp;
	anCtrlsUp.Add(IDC_PUSHOVER_ELEMTYPE_TRUSS);
	anCtrlsUp.Add(IDC_PUSHOVER_ELEMTYPE_NLNK);
	anCtrlsUp.Add(IDC_GLINK_RDO1);
	anCtrlsUp.Add(IDC_GLINK_CMB);
	anCtrlsUp.Add(IDC_GLINK_BTN);
	anCtrlsUp.Add(IDC_GLINK_RDO2);
	anCtrlsUp.Add(IDC_PUSHOVER_NLNK_STC);
	anCtrlsUp.Add(IDC_PUSHOVER_NLNK_EDT);
	anCtrlsUp.Add(IDC_PUSHOVER_ELEMTYPE_PSSP);
	anCtrlsUp.Add(IDC_WG_CMD_STATIC2);
	anCtrlsUp.Add(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB);
	anCtrlsUp.Add(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN);
	CDlgUtil::CtrlMoveDistY( this, anCtrlsUp  , nOffsetDistUp  , TRUE );
	GetDlgItem(IDC_CMD_APPLY)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);

	CRect rMove;
	GetDlgItem(IDC_WG_CMD_STATIC1)->GetWindowRect(rMove);
	rMove.bottom += nOffsetDistUp;
	ScreenToClient(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC1)->MoveWindow(rMove);
#else
	GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->ShowWindow(TRUE);
#endif

	m_edtNlnkInput.SetAttNodeList();
	m_edtNlnkInput.SetLButtonDownNotifyWindow(this);
	m_edtNlnkInput.SetEnterNotifyWindow(this);
	m_edtNlnkInput.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_edtNlnkInput.SetMaxNodeKeyNum(2);

	// Setting with default value
	m_rdoGlink1.SetCheck( TRUE ); // default
	m_rdoGlink2.SetCheck( FALSE);
	DlgGlinkCtrlChanges();
	//back=m_stcNlnkInput.EnableWindow(m_nElemType == 3);
	//back=m_edtNlnkInput.EnableWindow(m_nElemType == 3);
	//back=m_rdoGlink1.EnableWindow(m_nElemType == 3);
	//back=m_rdoGlink2.EnableWindow(m_nElemType == 3);
	//back=m_cmbGlink .EnableWindow(m_nElemType == 3);

	
	//  int nElemType; 0=SPG(Spring), 1=DSP(Linear Dashpot), 2=SLD(Spring and Linear Dashpot)
	//                 3=SND1(Spring and Nonlinear Dashpot2), 4=SND1(Spring and Nonlinear Dashpot2) 
	m_cmbGlink.SetComboBoxList( 0); // Setting with Spring

	UpdateData( TRUE);
				if( m_nElemType == 0) m_cmbHingeType.SetComboBoxList( D_PHGE_BEAMCOLUMN );  
	else  if( m_nElemType == 1) m_cmbHingeType.SetComboBoxList( D_PHGE_WALL       );  
	else  if( m_nElemType == 2) m_cmbHingeType.SetComboBoxList( D_PHGE_TRUSS      );  
	else  if( m_nElemType == 3) m_cmbHingeType.SetComboBoxList( D_PHGE_GLINK      );  
	else  if( m_nElemType == 4) m_cmbHingeType.SetComboBoxList( D_PHGE_PSPR       );  
	m_cmbHingeType.SetCurSel(0);
	UpdateData( FALSE);

	OnSelchangePushoverAssignHingetypeCmb();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPushOverHingeAssignDlg::Execute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey, aFailKey, aRealKey;
	GetSelectedElemKeyList(aSelKey);

	CArray<UINT, UINT> aNodeKeyList;
	GetSelectedNodeKeyList(aNodeKeyList);

	if(m_nElemType == 4 )
	{
		if(aNodeKeyList.GetSize() == 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD_NO_NODE));
			return;
		}
	}
	else
	{
		if( m_nCommandType == 1  && aSelKey.GetSize() == 0 || // delete option
			m_nElemType    != 3  && aSelKey.GetSize() == 0 ) 
		{
			GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_element_is_selected_));
			return;
		}
	}
	
	
	CArray<T_ELEM_K, T_ELEM_K> aSelKey1, aSelKey2, aSelKey3;
	if(m_nCommandType == 0)// Add Or Replace, Filtering by Element type
	{
		switch(m_nElemType)
		{
		case 2:// Truss
			aSelKey1.Copy(aSelKey); aSelKey2.Copy(aSelKey); aSelKey3.Copy(aSelKey);
			aSelKey.RemoveAll();
			GetElementByKind(aSelKey1, TRUSS_EL); aSelKey.Append(aSelKey1);
			GetElementByKind(aSelKey2, TRTENS_EL); aSelKey.Append(aSelKey2);
			GetElementByKind(aSelKey3, TRCOMP_EL); aSelKey.Append(aSelKey3);
			if (aSelKey.GetSize() == 0) 
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Truss_is_selected__));
				return;
			}
			break;
		case 0:// Beam
			GetElementByKind(aSelKey,BEAM_EL);
			if (aSelKey.GetSize() == 0) 
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Beam_is_selected__));
				return;
			}
			break;
		case 1:// Wall
			GetElementByKind(aSelKey,WALL_EL);
			if (aSelKey.GetSize() == 0) 
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Wall_is_selected__));
				return;
			}
			break;
		case 3:// General Link
			//------------------------------------------------------------------------
			if( m_rdoGlink1.GetCheck())
			{
				CArray<T_NLNK_K, T_NLNK_K> aNlnkK;
				CArray<T_NLNK_D, T_NLNK_D&> aNlnkD;
				T_NLLP_K NllpK;
				m_cmbGlink.GetSelectedNllp( NllpK);
		    if (NllpK == 0) 
		    {
			    GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_1)); // No General link property is selected
			    return;
		    }
				
				int nNumNlnk = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNlnkList(aNlnkK, aNlnkD);
		    if (nNumNlnk == 0) 
		    {
			    GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_2)); // No General link exists.
			    return;
		    }

				aSelKey.RemoveAll();
				for( int nNlnkCnt = 0 ; nNlnkCnt < nNumNlnk; ++nNlnkCnt)
				{
					if( aNlnkD[nNlnkCnt].PropKey == NllpK) aSelKey.Add( aNlnkK[nNlnkCnt]);
				}
				if (aSelKey.GetSize() == 0) 
		    {
			    GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_3)); // No General link matching selected property exists.
			    return;
		    }
			}
			//------------------------------------------------------------------------
			else if( m_rdoGlink2.GetCheck())
			{
				CArray<int, int> aNodes;
				CString csNodeList;
				m_edtNlnkInput.GetWindowText(csNodeList);
				if (!GetNodeList(csNodeList, aNodes) || aNodes.GetSize() != 2)
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_2_nodes_));
					return;
				}
				CArray<T_NLNK_K, T_NLNK_K> aNlnkK; aNlnkK.RemoveAll();
				int nNumNlnk = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNlnkKeyListBetween2Nodes(aNodes[0], aNodes[1], aNlnkK);
		    if (nNumNlnk == 0) 
		    {
			    GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_4)); // No General link is selected.
			    return;
		    }
				aSelKey.RemoveAll(); aSelKey.Append( aNlnkK);
			}
			//------------------------------------------------------------------------
			break;
		case 4:
			GetPointSpringSupportNode(aNodeKeyList);
			if (aNodeKeyList.GetSize() == 0) 
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD_NO_NODE_POINTSPRING));
				return;
			}
			break;
		}
	}

	int i=0;
	aFailKey.RemoveAll();
	aRealKey.RemoveAll();
	T_ELEM_D ElemD; ElemD.Initialize();
	T_MATL_D MatlD; MatlD.Initialize();
	T_SECT_D SectD; SectD.Initialize();
	int nMembType=0;

	T_PHGT_D Phgt_d;
	T_PHGT_K Phgt_k;
//   T_PHGE_D Phge_d;
	Phgt_d.Initialize();

	if(!m_cmbHingeType.GetSelectedPhgt(Phgt_k)) return;
	if(!m_pDoc->m_pAttrCtrl->GetPhgt(Phgt_k,Phgt_d)) return;
	
	BOOL bReal = FALSE;
	//back=Hnge_d.HingeType = Hngt_k;
	//back=BOOL bReal = ((Hngt_d.nMultiATCType==0 && Hngt_d.Prop.bHysUseCalcValue) || (Hngt_d.nMultiATCType==1 && Hngt_d.Prop.bUseCalcValue));
	//back= -----------------------------------------------------------------------------
	//back= 전처리에서 쓰이는 Iehp에 있던 부분입니다. 만약을 대비해서 남겨두었습니다.
	//back= -----------------------------------------------------------------------------
	/*back=
	if(!bReal) goto GT_ASGN_SLECT;

	if( m_nElemType != 3) // in the case of not general link
	{
		// KJH20031229 => Assign 할 수 없는 단면 형상을 골라내고 메시지...(신미영)
		for(i=0; i<aSelKey.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemD);
			m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD);
			if(MatlD.Type == _T("C"))
			{

	#if defined(_MGEN)	// Gen.
			  nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
			  m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
			  if(m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))	// Wall.
				{
				  // Change by ZINU.('04.02.10). Wall is NOT Section.
				  aRealKey.Add(aSelKey[i]);
				}
				else if(nMembType == D_MBTP_BEAM)
				{
		      if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					  (SectD.SectBefore.Shape==_T("T") || SectD.SectBefore.Shape==_T("SB")))
					  aRealKey.Add(aSelKey[i]);
					else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
						aRealKey.Add(aSelKey[i]);
					else aFailKey.Add(aSelKey[i]);
			  }
			  else if(nMembType == D_MBTP_COLUMN || nMembType == D_MBTP_BRACE)
			  {
					if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					  (SectD.SectBefore.Shape==_T("P") || SectD.SectBefore.Shape==_T("SR") ||
			       SectD.SectBefore.Shape==_T("SB")))
						 aRealKey.Add(aSelKey[i]);
					else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
						aRealKey.Add(aSelKey[i]);
					else aFailKey.Add(aSelKey[i]);
				}
				else aFailKey.Add(aSelKey[i]);

	#else	// Civil.
			  nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
			  m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
				if(nMembType == D_MBTP_BEAM)
				{
		      if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					  (SectD.SectBefore.Shape==_T("T") || SectD.SectBefore.Shape==_T("SB")))
					  aRealKey.Add(aSelKey[i]);
					else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
						aRealKey.Add(aSelKey[i]);
					else aFailKey.Add(aSelKey[i]);
			  }
			  else if(nMembType == D_MBTP_COLUMN || nMembType == D_MBTP_BRACE)
			  {
					if(SectD.nStype==D_SECT_TYPE_REGULAR && 
					  (SectD.SectBefore.Shape==_T("B")   || SectD.SectBefore.Shape==_T("P")    ||
			       SectD.SectBefore.Shape==_T("SR")  || SectD.SectBefore.Shape==_T("SB")   ||
			       SectD.SectBefore.Shape==_T("OCT") || SectD.SectBefore.Shape==_T("SOCT") ||
			       SectD.SectBefore.Shape==_T("TRK") || SectD.SectBefore.Shape==_T("STRK") ||
			       SectD.SectBefore.Shape==_T("HTRK")))
						 aRealKey.Add(aSelKey[i]);
					else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
						aRealKey.Add(aSelKey[i]);
					else aFailKey.Add(aSelKey[i]);
				}
				else aFailKey.Add(aSelKey[i]);
	#endif

			}
			else if(MatlD.Type == _T("S"))
			{
				m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
				if(SectD.nStype == D_SECT_TYPE_REGULAR)
				{
					if(SectD.SectBefore.Shape==_T("L")  || SectD.SectBefore.Shape==_T("C")  ||
		         SectD.SectBefore.Shape==_T("H")  || SectD.SectBefore.Shape==_T("T")  ||
		         SectD.SectBefore.Shape==_T("B")  || SectD.SectBefore.Shape==_T("P")  ||
		         SectD.SectBefore.Shape==_T("SR") || SectD.SectBefore.Shape==_T("SB") ||
		         SectD.SectBefore.Shape==_T("2L") || SectD.SectBefore.Shape==_T("2C"))
		         aRealKey.Add(aSelKey[i]);
					else aFailKey.Add(aSelKey[i]);
				}
				else if (SectD.nStype == D_SECT_TYPE_USER)  // MNET1597-JBSEON-050913
					aRealKey.Add(aSelKey[i]);
				else aFailKey.Add(aSelKey[i]);
			}
			else if(MatlD.Type == _T("SRC"))
			{
				m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD);
				if(SectD.nStype == D_SECT_TYPE_SRC)
				{
					if(SectD.SectBefore.Shape==_T("RBO") || SectD.SectBefore.Shape==_T("RBC") ||
						 SectD.SectBefore.Shape==_T("RPO") || SectD.SectBefore.Shape==_T("RPC") ||
						 SectD.SectBefore.Shape==_T("RHB") || SectD.SectBefore.Shape==_T("CBO") ||
						 SectD.SectBefore.Shape==_T("CBC") || SectD.SectBefore.Shape==_T("CPO") ||
						 SectD.SectBefore.Shape==_T("CPC") || SectD.SectBefore.Shape==_T("CHB") ||
						 SectD.SectBefore.Shape==_T("EBC") || SectD.SectBefore.Shape==_T("EPC") )
						 aRealKey.Add(aSelKey[i]);
					else aFailKey.Add(aSelKey[i]);
				}
				else aFailKey.Add(aSelKey[i]);
			}
			else aFailKey.Add(aSelKey[i]);
		}
	// END KJH
	}*/

//GT_ASGN_SLECT:
 	UpdateData(TRUE);
	BOOL bSuccess;
	int nElemType = 0;
	switch( m_nElemType)
	{
	case 2: // Truss
		nElemType = D_PHGE_TRUSS;
		break;
	case 0: // Beam
		nElemType = D_PHGE_BEAMCOLUMN;
		break;
	case 1: // Wall
		nElemType = D_PHGE_WALL;
		break;
	case 3: // General Link
		nElemType = D_PHGE_GLINK;
		break;
	case 4: // Point Spring Support
		nElemType = D_PHGE_PSPR;
		break;
	default:
		ASSERT(0);
		return;
		break;
	}

	if (m_nCommandType == 0) // add or replace
	{
		if(m_nLocation !=3)
		{
			//back=Hnge_d.nHingeLocation = m_nLocation +1;	
			if(nElemType==D_PHGE_PSPR)
			{
				bSuccess = m_pDoc->m_pDataCtrl->AddPhge(nElemType, aNodeKeyList, Phgt_k);
			}
			else
			{
				if(bReal)
			    bSuccess = m_pDoc->m_pDataCtrl->AddPhge(nElemType, aRealKey, Phgt_k);
				else 
					bSuccess = m_pDoc->m_pDataCtrl->AddPhge(nElemType, aSelKey, Phgt_k);
			}

		}
		else
		{
			//back=Hnge_d.nHingeLocation = 1;	
			if(bReal)
			  bSuccess = m_pDoc->m_pDataCtrl->AddPhge(nElemType, aRealKey, Phgt_k);
			else
				bSuccess = m_pDoc->m_pDataCtrl->AddPhge(nElemType, aSelKey, Phgt_k);
			
			//back=Hnge_d.nHingeLocation = 3;	
			//back=if(bReal)
			//back=  bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aRealKey, Hnge_d)*bSuccess;
			//back=else 
			//back=  bSuccess = m_pDoc->m_pDataCtrl->AddHnge(aSelKey, Hnge_d)*bSuccess;
		}
	}
	else if (m_nCommandType == 1)  // delete
	{      
		if(nElemType==D_PHGE_PSPR) bSuccess = m_pDoc->m_pDataCtrl->DelPhge(nElemType, aNodeKeyList);
		else                       bSuccess = m_pDoc->m_pDataCtrl->DelPhge(nElemType, aSelKey);	
	}
	if(bReal)
	{
		CNumericOptimizer optimizer;
		int nFailCount = aFailKey.GetSize();
		if(nFailCount>0 && m_nCommandType==0)
		{
			CString msg = _T("");
		
			long* aNum = new long[nFailCount];
			for(int i=0; i<nFailCount; i++) aNum[i] = aFailKey.GetAt(i);

			qsort((void*)aNum, nFailCount, sizeof(long), CNumericOptimizer::comparei);
			msg = optimizer.Optimize(aNum, nFailCount);
			delete [] aNum;

			GSaveHistoryFormatNF(_LS(IDS_CMD_PUSH_ERROR__SECT_SHAP), msg);
		}
	}
	if(bSuccess)
	{
		if(m_nCommandType == 0)  // Add or Replace
		{
			if(nElemType==D_PHGE_PSPR)
			{
				m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodeKeyList, TRUE);
			}
			else
			{
				if(bReal)
					m_pDoc->m_pViewCtrl->UnSelectElem(NULL, aRealKey, TRUE);
				else 
					m_pDoc->m_pViewCtrl->UnSelectElem(NULL, aSelKey, TRUE);
			}
		}
		else
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CPushOverHingeAssignDlg::OnTmClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
	
}

void CPushOverHingeAssignDlg::OnSelchangePushoverAssignHingetypeCmb() 
{
	/*back=
	T_PHGT_D Phgt_d;
	T_PHGT_K Phgt_k;
	if(!m_cmbHingeType.GetSelectedPhgt(Phgt_k)) return;
	if(!m_pDoc->m_pAttrCtrl->GetPhgt(Phgt_k,Phgt_d)) return;
	m_nHingeType = Hngt_d.nHingeType;
	SetHingeTypeInform(m_nHingeType);
	switch(m_nHingeType)
	{
		case 0://P
		case 2:// Shear
		case 4:// Torsion
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_BEAM)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->EnableWindow(TRUE);
			m_nLocation = 1;// Center
			if(m_nHingeType==2) // Modified by KJH20040713 from 강은경
			{
				GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(FALSE);
				if(m_nElemType==0) m_nElemType = 1;
			}
			else
			{
				GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(TRUE);
			}
			break;
		case 1://M
		case 3://PMM
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_TRUSS)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_BEAM)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ELEMTYPE_WALL)->EnableWindow(TRUE);
			
			if(m_nLocation == 1)
				m_nLocation = 0;// I
			if(m_nElemType == 0)
				m_nElemType = 1;
			break;
	}*/
	
	UpdateData(FALSE);
}

void CPushOverHingeAssignDlg::OnPushoverAssignHingetypelistBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PUSHOVER_DEFINE_HINGE, 0));
	
}

void CPushOverHingeAssignDlg::OnPushOverElemType() 
{
	UpdateData(TRUE);
	if(m_nElemType == 2)// Truss
	{
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
		m_nLocation = 1;// Center
	}
	else
	{
		if(m_nHingeType == 1 || m_nHingeType == 3) // M or PMM
		{	
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(TRUE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		}
		else// P,V,T
		{	
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
			GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		}
	}

	DlgGlinkCtrlChanges();
	//back=m_stcNlnkInput.EnableWindow(m_nElemType == 3);
	//back=m_edtNlnkInput.EnableWindow(m_nElemType == 3);
	//back=m_rdoGlink1.EnableWindow(m_nElemType == 3);
	//back=m_rdoGlink2.EnableWindow(m_nElemType == 3);
	//back=m_cmbGlink .EnableWindow(m_nElemType == 3);

// Assign Pushover Hinge Type/Properties
// #define D_PHGE_BEAMCOLUMN 1
// #define D_PHGE_WALL       2
// #define D_PHGE_TRUSS      3
// #define D_PHGE_GLINK      4
				if( m_nElemType == 0) m_cmbHingeType.SetComboBoxList( D_PHGE_BEAMCOLUMN );  
	else  if( m_nElemType == 1) m_cmbHingeType.SetComboBoxList( D_PHGE_WALL       );  
	else  if( m_nElemType == 2) m_cmbHingeType.SetComboBoxList( D_PHGE_TRUSS      );  
	else  if( m_nElemType == 3) m_cmbHingeType.SetComboBoxList( D_PHGE_GLINK      );  
	else  if( m_nElemType == 4) m_cmbHingeType.SetComboBoxList( D_PHGE_PSPR       );  
	else  ASSERT(0);
	
	m_cmbHingeType.SetCurSel(0);

	UpdateData(FALSE);
}

void CPushOverHingeAssignDlg::GetPointSpringSupportNode(CArray<UINT,UINT>& aKey)
{
	CArray<T_NSPR_K, T_NSPR_K> aKeyList;
	CArray<T_NSPR_D, T_NSPR_D&> aDataList;
	
	CArray<UINT,UINT> aExistPointSpringSupportNodeKey;
	int nNodeCnt = aKey.GetSize();
	for (int i=0; i<nNodeCnt; i++)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNsprNode(aKey[i], aKeyList, aDataList);
		if(aKeyList.GetSize() > 0)
		{
			aExistPointSpringSupportNodeKey.Add(aKey[i]);
		}
	}

	aKey.RemoveAll();
	aKey.Copy(aExistPointSpringSupportNodeKey);
}

void CPushOverHingeAssignDlg::GetElementByKind(CArray<UINT,UINT>& aKey,int nElemType)
{
	CQueryCtrl QueryCtrl;
	CString strElement;
	strElement.Format(_T("%ld"),nElemType);
	CArray<UINT,UINT> ElemKeys;
	CArray<UINT,UINT> SelKeys;
	SelKeys.Copy(aKey);
	aKey.RemoveAll();

	CQueryCondition QCond;          // 요소의 종류에 따른 쿼리 조건을 만든다.
	QCond.m_nCategory = CAT_ELEM_TYPE;
	QCond.m_nOperation = QC_OP_AND;
	QCond.m_aValue.Add(strElement);
	
	QueryCtrl.SelectKeyFromElem(ElemKeys,QCond);
	QueryCtrl.IntersectKey(ElemKeys,SelKeys,aKey);

}	
	

void CPushOverHingeAssignDlg::SetHingeTypeInform(int nHingeType)
{
	switch(nHingeType)
	{
		case 0:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Axial_P));
			break;
		case 1:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Moment_My_Mz));
			break;
		case 2:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Shear_Vy_Vz));
			break;
		case 3:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__P_My_Mz));
			break;
		case 4:
			m_wndHingeTypeInform.SetWindowText(_LS(IDS_PUSHOVER_Hinge_Type__Torsion_T));
			break;
		default:
			m_wndHingeTypeInform.SetWindowText(_T(""));
			break;
	}
}


BOOL CPushOverHingeAssignDlg::DlgGlinkCtrlChanges() 
{
	UpdateData();

	m_stcNlnkInput.EnableWindow(m_nCommandType != 1 && m_nElemType == 3 && m_rdoGlink2.GetCheck());
	m_edtNlnkInput.EnableWindow(m_nCommandType != 1 && m_nElemType == 3 && m_rdoGlink2.GetCheck());
	m_rdoGlink1   .EnableWindow(m_nCommandType != 1 && m_nElemType == 3 && TRUE);
	m_rdoGlink2   .EnableWindow(m_nCommandType != 1 && m_nElemType == 3 && TRUE);
	m_cmbGlink    .EnableWindow(m_nCommandType != 1 && m_nElemType == 3 && m_rdoGlink1.GetCheck());
	m_btnGlink    .EnableWindow(m_nCommandType != 1 && m_nElemType == 3 && m_rdoGlink1.GetCheck());

	return TRUE;
}

void CPushOverHingeAssignDlg::OnPushoverGLinkPropBtn()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_BNDR_NLLINK_PROPERTY, 0));
}

void CPushOverHingeAssignDlg::OnPushoverGLinkSelRdo() 
{
	DlgGlinkCtrlChanges();
}

void CPushOverHingeAssignDlg::OnPushoverAssignCmd() 
{
	UpdateData(TRUE);
	if(m_nCommandType == 1)// delete
	{
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_I)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_J)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_LOC_IJ)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN)->EnableWindow(FALSE);
		m_stcNlnkInput.EnableWindow(FALSE);
		m_edtNlnkInput.EnableWindow(FALSE);
		m_rdoGlink1.EnableWindow(FALSE);
		m_rdoGlink2.EnableWindow(FALSE);
		m_cmbGlink .EnableWindow(FALSE);
		m_btnGlink .EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPE_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_PUSHOVER_ASSIGN_HINGETYPELIST_BTN)->EnableWindow(TRUE);
		DlgGlinkCtrlChanges();
		OnSelchangePushoverAssignHingetypeCmb();
	}
}


