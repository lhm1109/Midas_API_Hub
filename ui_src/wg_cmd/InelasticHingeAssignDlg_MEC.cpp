// InelasticHingeAssignDlg_MEC.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "InelasticHingeAssignDlg_MEC.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_StoryData.h"
#include "..\wg_common\wg_common_Query.h"


#include "..\wg_main\wg_mainRes2.h"

#include "InelasticHingeImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeAssignDlg_MEC dialog

//IMPLEMENT_DYNCREATE(CInelasticHingeAssignDlg_MEC, CCMChildBarBase)

CInelasticHingeAssignDlg_MEC::CInelasticHingeAssignDlg_MEC()
	: CCMChildBarBase(CInelasticHingeAssignDlg_MEC::IDD)
{
	//{{AFX_DATA_INIT(CInelasticHingeAssignDlg_MEC)
	m_nElemType = ET_BEAM;
	m_nAddDelete = 0;
	m_strMembLoc = _T("");
	m_strMatl = _T("");
	m_strMembType = _T("");
	m_strSectThik = _T("");
	//}}AFX_DATA_INIT

	m_Data.Initialize();
}


void CInelasticHingeAssignDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInelasticHingeAssignDlg_MEC)
	DDX_Control(pDX, IDC_INELASTIC_HINGE_PROPERTY_CMB, m_cmbHingeProp);
	DDX_Radio(pDX, IDC_INELASTIC_ELEMTYPE_BEAM, m_nElemType);
	DDX_Radio(pDX, IDC_INELASTIC_ADD, m_nAddDelete);
	DDX_Text(pDX, IDC_SECT_NAME_STC, m_strSectThik);
	DDX_Control(pDX, IDC_INELASTIC_NLNK_STC, m_stcNlnkInput);
	DDX_Control(pDX, IDC_INELASTIC_NLNK_EDT, m_edtNlnkInput);
	DDX_Control(pDX, IDC_INELASTIC_NLNK_NUMBER_EDT, m_edtNlnkNumber);
	DDX_Control(pDX, IDC_GLINK_RDO1, m_rdoGlink1);
	DDX_Control(pDX, IDC_GLINK_RDO2, m_rdoGlink2);
	DDX_Control(pDX, IDC_GLINK_RDO3, m_rdoGlink3);
	DDX_Control(pDX, IDC_GLINK_CMB, m_cmbGlink);
	DDX_Control(pDX, IDC_GLINK_BTN, m_btnGlink);
	//DDX_Text(pDX, IDC_MATL_NAME_STC, m_strMatl);
	//DDX_Text(pDX, IDC_MEMB_LOCA_STC, m_strMembLoc);
	//DDX_Text(pDX, IDC_MEMB_TYPE_STC, m_strMembType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInelasticHingeAssignDlg_MEC, CCMChildBarBase)
	//{{AFX_MSG_MAP(CInelasticHingeAssignDlg_MEC)
	ON_CBN_SELCHANGE(IDC_INELASTIC_HINGE_PROPERTY_CMB, OnSelchangeInelasticHingePropCmb)
	ON_BN_CLICKED(IDC_INELASTIC_HINGE_PROPERTY_BTN, OnInelasticHingePropertyBtn)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_BEAM, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_TRUSS, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_WALL, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_NLNK, OnInelasticElemType)
	ON_BN_CLICKED(IDC_INELASTIC_ELEMTYPE_PSSP, OnInelasticElemType)
	ON_BN_CLICKED(IDC_GLINK_RDO1, OnInelasticGLinkSelRdo)
	ON_BN_CLICKED(IDC_GLINK_RDO2, OnInelasticGLinkSelRdo)
	ON_BN_CLICKED(IDC_GLINK_RDO3, OnInelasticGLinkSelRdo)
	ON_BN_CLICKED(IDC_GLINK_BTN,  OnInelasticGLinkPropBtn)
	ON_BN_CLICKED(IDC_INELASTIC_ADD, OnInelasticAddDelete)
	ON_BN_CLICKED(IDC_INELASTIC_DELETE, OnInelasticAddDelete)
	ON_BN_CLICKED(IDC_SELECT_BTN, OnSelectBtn)
	ON_BN_CLICKED(IDC_CMD_IMPORT_HINGE_BTN, OnImportHingeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeAssignDlg_MEC message handlers

LRESULT CInelasticHingeAssignDlg_MEC::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if ( m_edtNlnkInput.GetSafeHwnd() == (HWND)lParam )
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_edtNlnkInput.GetWindowText(csNodeList);
		if ( !GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2 )
			return 0L;
		//back=OnApply();
		//back=m_edtNlnkInput.ClearContents();
	}
	return 0L;
}

LRESULT CInelasticHingeAssignDlg_MEC::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if ( m_edtNlnkInput.GetSafeHwnd() == (HWND)lParam )
	{
		Execute();
	}
	return 0L;
}

BOOL CInelasticHingeAssignDlg_MEC::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMChildBarBase::OnInitDialog();	 

	MakeIehpCombo(m_nElemType);
	OnSelchangeInelasticHingePropCmb();

#if defined(_CIVIL)
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->ShowWindow(FALSE);
	// y 방향으로 이동하는데 WALL Ctrl 위치로 Wall 아래 있는 Ctrl 들을 이동시킨다.
	CRect rc1, rc2, rc3;
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->GetWindowRect(&rc1);
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_NLNK)->GetWindowRect(&rc2);
	GetDlgItem(IDC_WG_CMD_STATIC2)->GetWindowRect(&rc3);
	ScreenToClient(&rc3);
	const double dist = rc2.top - rc1.top;
	// Resize Static Ctrl
	GetDlgItem(IDC_WG_CMD_STATIC2)->MoveWindow(rc3.left, rc3.top, rc3.Width(), rc3.Height() - dist);

	CArray<UINT, UINT> anCtrlsUp;
	anCtrlsUp.Add(IDC_INELASTIC_ELEMTYPE_NLNK);
	anCtrlsUp.Add(IDC_GLINK_RDO1);
	anCtrlsUp.Add(IDC_GLINK_CMB);
	anCtrlsUp.Add(IDC_GLINK_BTN);
	anCtrlsUp.Add(IDC_GLINK_RDO2);
	anCtrlsUp.Add(IDC_INELASTIC_NLNK_STC);
	anCtrlsUp.Add(IDC_INELASTIC_NLNK_EDT);
	anCtrlsUp.Add(IDC_INELASTIC_ELEMTYPE_PSSP);
	anCtrlsUp.Add(IDC_GLINK_RDO3);
	anCtrlsUp.Add(IDC_INELASTIC_NLNK_NUMBER_EDT);
	//
	anCtrlsUp.Add(IDC_WG_CMD_STATIC1);
	anCtrlsUp.Add(IDC_INELASTIC_HINGE_PROPERTY_CMB);
	anCtrlsUp.Add(IDC_INELASTIC_HINGE_PROPERTY_BTN);
	anCtrlsUp.Add(IDC_SECT_NAME_TXT);
	anCtrlsUp.Add(IDC_SECT_NAME_STC);
	anCtrlsUp.Add(IDC_SELECT_BTN);
	anCtrlsUp.Add(IDC_CMD_IMPORT_HINGE_BTN);
	anCtrlsUp.Add(IDC_CMD_APPLY);
	anCtrlsUp.Add(IDC_CMD_CLOSE);

	CDlgUtil::CtrlMoveDistY(this, anCtrlsUp, -dist, TRUE);
#else
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->ShowWindow(TRUE);
#endif
	m_edtNlnkInput.SetAttNodeList();
	m_edtNlnkInput.SetLButtonDownNotifyWindow(this);
	m_edtNlnkInput.SetEnterNotifyWindow(this);
	m_edtNlnkInput.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_edtNlnkInput.SetMaxNodeKeyNum(2);

	// Setting with default value
	m_rdoGlink1.SetCheck(TRUE); // default
	m_rdoGlink2.SetCheck(FALSE);
	m_rdoGlink3.SetCheck(FALSE);
	DlgGlinkCtrlChanges();

return TRUE;  // return TRUE unless you set the focus to a control
	            // EXCEPTION: OCX Property Pages should return FALSE
}

void CInelasticHingeAssignDlg_MEC::UpdateIehpCombo()
{
	int nSel = m_cmbHingeProp.GetCurSel();
	if (nSel == CB_ERR)
	{
		MakeIehpCombo(m_nElemType);
	}
	else
	{
		UINT key = m_cmbHingeProp.GetItemData(nSel);
		MakeIehpCombo(m_nElemType);
		ChangeIehpComboSel(key);
		OnSelchangeInelasticHingePropCmb();
	}
}

void CInelasticHingeAssignDlg_MEC::MakeIehpCombo(int nElemType/*=-1*/)
{
	m_cmbHingeProp.ResetContent();

	CArray<T_IEHP_K, T_IEHP_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetIehpKeyList(rKeyList);

	int nSize = rKeyList.GetSize();
	if ( nSize < 1 )
	{
		return;
	}

	int nIndex = 0;
	T_IEHP_D IehpD;
	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetIehp(rKeyList[i], IehpD);

		switch ( m_nElemType )
		{
		case ET_TRUSS:
			if ( IehpD.nHingeType != D_IEHP_HTYP_TRUSS ) continue;
			break;
		case ET_BEAM:
			if ( IehpD.nHingeType != D_IEHP_HTYP_BEAM_LUMP 
			  && IehpD.nHingeType != D_IEHP_HTYP_BEAM_DIST ) continue;
			break;
		case ET_WALL:
			if ( IehpD.nHingeType != D_IEHP_HTYP_WALL ) continue;
			break;
		case ET_NLNK:
			if ( IehpD.nHingeType != D_IEHP_HTYP_SPRING ) continue;
			break;
		case ET_PSSP:
			if ( IehpD.nHingeType != D_IEHP_HTYP_POINTSPRING ) continue;
			break;
		default: ASSERT(0); break;
		}

		nIndex = m_cmbHingeProp.AddString(IehpD.strName);
		m_cmbHingeProp.SetItemData(nIndex, rKeyList[i]);
	}
}

void CInelasticHingeAssignDlg_MEC::ChangeIehpComboSel(UINT key)
{
	int nSize = m_cmbHingeProp.GetCount();

	UINT cmbKey;
	for (int i=0; i<nSize; i++)
	{
		cmbKey = m_cmbHingeProp.GetItemData(i);
		if (cmbKey == key)
		{
			m_cmbHingeProp.SetCurSel(i);
			break;
		}
	}
}

void CInelasticHingeAssignDlg_MEC::Execute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	GetSelectedElemKeyList(aSelKey);

	CArray<UINT, UINT> aNodeKeyList;
	GetSelectedNodeKeyList(aNodeKeyList);

	if ( m_nElemType == ET_PSSP )
	{
		if ( aNodeKeyList.GetSize() == 0 )
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD_NO_NODE));
			return;
		}
	}
	else
	{
		if ( m_nAddDelete == 1 && aSelKey.GetSize() == 0 || // delete option
			m_nElemType != ET_NLNK && aSelKey.GetSize() == 0 )
		{
			GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_element_is_selected_));
			return;
		}
	}

	CArray<T_ELEM_K, T_ELEM_K> aSelKey1, aSelKey2, aSelKey3;
	
	if(m_nAddDelete == 0)// Add Or Replace, Filtering by Element type
	{
		switch ( m_nElemType )
		{
		case ET_TRUSS:// Truss
			aSelKey1.Copy(aSelKey); aSelKey2.Copy(aSelKey); aSelKey3.Copy(aSelKey);
			aSelKey.RemoveAll();
			GetElementByKind(aSelKey1, TRUSS_EL); aSelKey.Append(aSelKey1);
			GetElementByKind(aSelKey2, TRTENS_EL); aSelKey.Append(aSelKey2);
			GetElementByKind(aSelKey3, TRCOMP_EL); aSelKey.Append(aSelKey3);

			if ( aSelKey.GetSize() == 0 )
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Truss_is_selected__));
				return;
			}
			break;
		case ET_BEAM:// Beam
			GetElementByKind(aSelKey, BEAM_EL);
			if ( aSelKey.GetSize() == 0 )
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Beam_is_selected__));
				return;
			}
			break;
		case ET_WALL:// Wall
			{
				GetElementByKind(aSelKey, WALL_EL);
				if ( aSelKey.GetSize() == 0 )
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Wall_is_selected__));
					return;
				}
				BOOL bRet = m_pDoc->m_pAttrCtrl->MakeRootWallElemKeyList(aSelKey);	//Root Elem 들만 aElemKeyList에 저장한다.
				if ( !bRet )
				{
					GSaveHistoryFormatNF(_LS(IDS_DB_POSTCTRL_____MakePostData_WALL_STORYERROR)); // _T("층정보가 없어 할당이 되지 않습니다.");
					return;
				}

				//CDBDoc* pDoc = CDBDoc::GetDocPoint();
				//CStoryData StoryData(m_pDoc);
				//StoryData.MakeStoryData(1);
				CStoryData* pStoryData = m_pDoc->GetStoryData();

				for ( int i = 0; i < aSelKey.GetSize(); i++ )
				{
					CArray<T_ELEM_K, T_ELEM_K> SortedWallIdElemList;
					CArray<T_ELEM_K, T_ELEM_K> ConnectedLinearElemList;
					CArray<T_ELEM_K, T_ELEM_K> CoordiLinearElemList;

					m_pDoc->m_pAttrCtrl->GetEqualWallIdElemList(SortedWallIdElemList, aSelKey[i], pStoryData);
					double dCompareLevel = -1;
					for ( int j = 0; j < SortedWallIdElemList.GetSize(); j++ )
					{
						T_STOR_K StorK;
						T_STOR_D StorD;
						m_pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, SortedWallIdElemList[j]);
						m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD);
						if ( dCompareLevel == -1 )
							dCompareLevel = StorD.dStoryLevel;
						else if ( dCompareLevel != StorD.dStoryLevel )
						{
							//error = 같은 층, 같은 Wall ID로 정렬 한 결과 중, 층 Level 이 다를 경우
							m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_DiffLevel));
							return;
						}
					}
					m_pDoc->m_pAttrCtrl->GetConnectedLinearElem(ConnectedLinearElemList, aSelKey[i], SortedWallIdElemList);
					if ( ConnectedLinearElemList.GetSize() != SortedWallIdElemList.GetSize() )
					{
						//error = 연결이 안되어 있는 경우 (중간중간이 빌 경우)
						m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_Connected));
						return;
					}
					m_pDoc->m_pAttrCtrl->GetEqualCoordLinearElem(ConnectedLinearElemList, CoordiLinearElemList);
					if ( CoordiLinearElemList.GetSize() != ConnectedLinearElemList.GetSize() )
					{
						//error = 연결은 되어있는지 꺾여서 연결되는 경우
						m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_Coordi));
						return;
					}
				}
			}
			break;
		case ET_NLNK:// General Link
			{
				if ( m_rdoGlink1.GetCheck() )
				{
					CArray<T_NLNK_K, T_NLNK_K> aNlnkK;
					CArray<T_NLNK_D, T_NLNK_D&> aNlnkD;
					T_NLLP_K NllpK;
					m_cmbGlink.GetSelectedNllp(NllpK);
					if ( NllpK == 0 )
					{
						GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_1)); // No General link property is selected
						return;
					}

					int nNumNlnk = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNlnkList(aNlnkK, aNlnkD);
					if ( nNumNlnk == 0 )
					{
						GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_2)); // No General link exists.
						return;
					}

					aSelKey.RemoveAll();
					for ( int nNlnkCnt = 0; nNlnkCnt < nNumNlnk; ++nNlnkCnt )
					{
						if ( aNlnkD[nNlnkCnt].PropKey == NllpK ) aSelKey.Add(aNlnkK[nNlnkCnt]);
					}
					if ( aSelKey.GetSize() == 0 )
					{
						GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_3)); // No General link matching selected property exists.
						return;
					}
				}
				//------------------------------------------------------------------------
				else if ( m_rdoGlink2.GetCheck() )
				{
					CArray<int, int> aNodes;
					CString csNodeList;
					m_edtNlnkInput.GetWindowText(csNodeList);
					if ( !GetNodeList(csNodeList, aNodes) || aNodes.GetSize() != 2 )
					{
						GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_2_nodes_));
						return;
					}
					CArray<T_NLNK_K, T_NLNK_K> aNlnkK; aNlnkK.RemoveAll();
					int nNumNlnk = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNlnkKeyListBetween2Nodes(aNodes[0], aNodes[1], aNlnkK);
					if ( nNumNlnk == 0 )
					{
						GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_4)); // No General link is selected.
						return;
					}
					aSelKey.RemoveAll(); aSelKey.Append(aNlnkK);
				}
				else if ( m_rdoGlink3.GetCheck() )
				{
					int nGlinkNumber = 0;
					CString strGlinkNumber;
					m_edtNlnkNumber.GetWindowText(strGlinkNumber);
					if ( strGlinkNumber == _T("") )
					{
						GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_ERROR_GENERAL_LINK_2));
						return;
					}
					if ( !CStrParser::GetINumber(strGlinkNumber, nGlinkNumber) ) return;

					if ( CDBDoc::GetDocPoint()->m_pAttrCtrl->ExistNlnk(nGlinkNumber) )
					{
						aSelKey.RemoveAll(); 
						aSelKey.Add(nGlinkNumber);
					}
				}
				else ASSERT(0);
			}
			break;
		case ET_PSSP: // Point Spring
			{
				GetPointSpringSupportNode(aNodeKeyList);
				if ( aNodeKeyList.GetSize() == 0 )
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_CMD_NO_NODE_POINTSPRING));
					return;
				}
			}
			break;
		default: ASSERT(0); break;
		}
	}

	BOOL bSuccess = FALSE;
	int nConvElemType = 0;
	if (m_nAddDelete == 0) // add or replace
	{		
		T_IEHP_K KeyIehp;
		T_IEHP_D DataIehp;
		T_IEHG_D DataIehg;
		DataIehp.Initialize();
		
		int nSel = m_cmbHingeProp.GetCurSel();
		if (nSel==CB_ERR) return;
		KeyIehp = m_cmbHingeProp.GetItemData(nSel);
		if(!m_pDoc->m_pAttrCtrl->GetIehp(KeyIehp, DataIehp)) return;
	
		switch ( m_nElemType )
		{
		case ET_BEAM:
			{
				nConvElemType = D_IEHG_TYPE_BEAMCOLUMN;
				if ( DataIehp.nHingeType != D_IEHP_HTYP_BEAM_LUMP && DataIehp.nHingeType != D_IEHP_HTYP_BEAM_DIST )
				{
					GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
					return;
				}
			}
			break;
		case ET_TRUSS:
			{
				nConvElemType = D_IEHG_TYPE_TRUSS;
				if ( DataIehp.nHingeType != D_IEHP_HTYP_TRUSS )
				{
					GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
					return;
				}
			}
			break;
		case ET_WALL:
			{
				nConvElemType = D_IEHG_TYPE_WALL;
				if ( DataIehp.nHingeType != D_IEHP_HTYP_WALL )
				{
					GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
					return;
				}
				// 요소와 비탄성힌지의 벽체 타입(Membrace/Plate)이 같은지 검사
				T_ELEM_D ElemD;
				for ( int i = 0; i < aSelKey.GetSize(); i++ )
				{
					m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemD);
					if ( ElemD.nSubType - 1 != DataIehp.nWallType )
					{
						GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_WALL_TYPE_DIFFERENT), aSelKey[i]);
						return;
					}
				}
			}
			break;
		case ET_NLNK:
			{
				nConvElemType = D_IEHG_TYPE_GLINK;
				if ( DataIehp.nHingeType != D_IEHP_HTYP_SPRING )
				{
					GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
					return;
				}
			}
			break;
		case ET_PSSP:
			{
				nConvElemType = D_IEHG_TYPE_POINTSPRING;
				if ( DataIehp.nHingeType != D_IEHP_HTYP_POINTSPRING )
				{
					GSaveHistoryFormatNF(_LS(IDS_CMD_IEHP_ERROR_Element_Type));
					return;
				}
			}
			break;
		default: ASSERT(0); break;
		}

		DataIehg.IehpKey = KeyIehp;
		if ( nConvElemType == D_IEHG_TYPE_POINTSPRING )
		{
			CArray<T_IEHG_K, T_IEHG_K> aIehgKey;
			const int nNodeKSize = aNodeKeyList.GetSize();
			T_IEHG_K IehgK;
			for ( int i = 0; i < nNodeKSize; i++ )
			{
				IehgK.key.objK = aNodeKeyList[i];
				IehgK.key.type = D_IEHG_TYPE_POINTSPRING;
				aIehgKey.Add(IehgK);
			}
			bSuccess = m_pDoc->m_pDataCtrl->AddIehg(aIehgKey, DataIehg);
		}
		else
		{
			CArray<T_IEHG_K, T_IEHG_K> aIehgKey;
			const int nSelKSize = aSelKey.GetSize();
			T_IEHG_K IehgK;
			for ( int i = 0; i < nSelKSize; i++ )
			{
				IehgK.key.objK = aSelKey[i];
				IehgK.key.type = nConvElemType;
				aIehgKey.Add(IehgK);
			}
			bSuccess = m_pDoc->m_pDataCtrl->AddIehg(aIehgKey, DataIehg);
		}
	}
	else if ( m_nAddDelete == 1 )
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelIehg(nConvElemType, aSelKey);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
}

void CInelasticHingeAssignDlg_MEC::OnSelchangeInelasticHingePropCmb()
{
	UpdateData(TRUE);

	int nCount = m_cmbHingeProp.GetCount();
	if(nCount <= 0)
	{
		GetDlgItem(IDC_SECT_NAME_TXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SECT_NAME_STC)->EnableWindow(FALSE);
		GetDlgItem(   IDC_SELECT_BTN)->EnableWindow(FALSE);
		return;
	}

	int nSel = m_cmbHingeProp.GetCurSel();
	if (nSel==CB_ERR) 
	{
		m_cmbHingeProp.SetCurSel(0);
		nSel = 0;
	}
	T_IEHP_K key = m_cmbHingeProp.GetItemData(nSel);

	m_Data.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetIehp(key, m_Data)) return;

	const bool bEnableWindow = [this] ()
	{
		if ( m_Data.nFiberAutoUser == 0 ) // Auto
		{
			return false;
		}
		if ( m_Data.nDefinition == 1 ) // Fiber
		{
			return true;
		}

		return false;
	} ();

	GetDlgItem(IDC_SECT_NAME_TXT)->EnableWindow(bEnableWindow);
	GetDlgItem(IDC_SECT_NAME_STC)->EnableWindow(bEnableWindow);
	GetDlgItem(   IDC_SELECT_BTN)->EnableWindow(bEnableWindow);

	if(m_Data.nHingeType==4)
		GetDlgItem(IDC_SECT_NAME_TXT)->SetWindowText(_LS(IDS_CMD_THICKNESS_thickness));
	else
		GetDlgItem(IDC_SECT_NAME_TXT)->SetWindowText(_LS(IDS_CMD_CSCS__Section));
	

	if( bEnableWindow )
	{
		if(m_Data.nHingeType==D_IEHP_HTYP_BEAM_DIST)
		{
			T_SECT_D SectD; SectD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetSect(m_Data.SectKey, SectD)) 
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_SectionData_Invalid));
				return;
			}
			m_strSectThik = SectD.SName;
		}
		else
		{
			T_THIK_D ThikD; ThikD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetThik(m_Data.SectKey, ThikD)) 
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_ThicknessData_Invalid));
				return;
			}
			m_strSectThik = m_pDoc->m_pAttrCtrl->GetThikNameAuto(m_Data.SectKey);
		}
	}

	UpdateData(FALSE);
}

void CInelasticHingeAssignDlg_MEC::OnInelasticHingePropertyBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_ITHA_DEFINE_HINGE, 0));	
}

void CInelasticHingeAssignDlg_MEC::OnInelasticElemType()
{	
	UpdateData(TRUE);

	DlgGlinkCtrlChanges();
	MakeIehpCombo(m_nElemType);
	int nSel = m_cmbHingeProp.GetCurSel();
	if (nSel==CB_ERR) 
	{
		m_cmbHingeProp.SetCurSel(0);
	}
}

void CInelasticHingeAssignDlg_MEC::GetElementByKind(CArray<UINT,UINT>& aKey, int nElemType)
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

// CRB/Plate 구분하지 않고 비선형 해석 가능
//   if(nElemType == WALL_EL)
//   {
//     CArray<UINT,UINT> aWallKeyTemp;
//     aWallKeyTemp.Copy(aKey);
// 
//     aKey.RemoveAll();
//     T_ELEM_D ElemD;
//     for(int i = 0; i < aWallKeyTemp.GetSize(); i++)
//     {
//       if(!m_pDoc->m_pAttrCtrl->GetElem(aWallKeyTemp[i], ElemD)) continue;
//       if(ElemD.nWallType==1) aKey.Add(aWallKeyTemp[i]);
//     }
//   }
}	


void CInelasticHingeAssignDlg_MEC::OnInelasticAddDelete()
{
	UpdateData(TRUE);

	const bool bEnable = m_nAddDelete == 0;
	
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_BEAM)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_TRUSS)->EnableWindow(bEnable); // MNET:XXXX-JWKWON-20140404 : MQC 7554
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_WALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_NLNK)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_ELEMTYPE_PSSP)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_HINGE_PROPERTY_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_HINGE_PROPERTY_BTN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GLINK_RDO1)->EnableWindow(bEnable);
	GetDlgItem(IDC_GLINK_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_GLINK_BTN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GLINK_RDO2)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_NLNK_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_NLNK_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_GLINK_RDO3)->EnableWindow(bEnable);
	GetDlgItem(IDC_INELASTIC_NLNK_NUMBER_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_WG_CMD_STATIC1)->EnableWindow(bEnable);
	GetDlgItem(IDC_SECT_NAME_TXT)->EnableWindow(bEnable);
	GetDlgItem(IDC_SECT_NAME_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_SELECT_BTN)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_IMPORT_HINGE_BTN)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_CLOSE)->EnableWindow(bEnable);

	if ( bEnable )
	{
		DlgGlinkCtrlChanges();
		OnSelchangeInelasticHingePropCmb();
	}
}

void CInelasticHingeAssignDlg_MEC::OnSelectBtn()
{
	UpdateData(TRUE);
	T_ELEM_K ElemK;
	T_ELEM_D ElemD;
	T_MATL_D MatlD;

	if(m_Data.nDefinition == 1)
	{
		if(m_Data.nHingeType==D_IEHP_HTYP_BEAM_DIST)
		{
		  T_SECT_D SectD; SectD.Initialize();
		  if(!m_pDoc->m_pAttrCtrl->GetSect(m_Data.SectKey, SectD)) 
		  {
			  AfxMessageBox(_LS(IDS_CMD_ERROR_SectionData_Invalid));
			  return;
		  }
		  m_strSectThik = SectD.SName;
		}
		else
		{
			T_THIK_D ThikD; ThikD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetThik(m_Data.SectKey, ThikD)) 
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_ThicknessData_Invalid));
				return;
			}
			m_strSectThik = m_pDoc->m_pAttrCtrl->GetThikNameAuto(m_Data.SectKey);
		}
	}

	CArray<T_ELEM_K, T_ELEM_K> aElemK, aMatlElemK, aSectElemK, aSelKey;
	aElemK.RemoveAll(); aMatlElemK.RemoveAll(); aSectElemK.RemoveAll(); aSelKey.RemoveAll();
	
	if(m_Data.nHingeType==D_IEHP_HTYP_BEAM_DIST)
	{
		CArray<T_SECT_K, T_SECT_K> aSectK;
		aSectK.Add(m_Data.SectKey);
		m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectK, NULL, &aElemK);
	}
	else
	{
		CArray<T_THIK_K, T_THIK_K> aThikK;
		aThikK.Add(m_Data.SectKey);
		m_pDoc->m_pAttrCtrl->GetKeyListThikUser(&aThikK, NULL, &aElemK);
	}

	//m_pDoc->m_pAttrCtrl->GetElemKeyListMatlSectUser(NULL, m_Data.SectKey, &aElemK);

	int nSize = aElemK.GetSize();
	if(nSize<1) 
	{
		AfxMessageBox(_LS(IDS_DB_NO_ELEMENT));
		return;
	}
	for(int i=0; i<nSize; i++)
	{
		ElemD.Initialize();
		ElemK = aElemK[i];
		if(m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
			if(TransMembType(nMembType)<0) { ASSERT(0); return; }
			if(nMembType == 0) // Beam
			{
				// 멤버냐?
				T_MEMB_K MembK;
				if(m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK, MembK))
				{
					//공구리냐?
					if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) { ASSERT(0); return; }
					if(MatlD.Type == _T("C"))
					{
						T_MEMB_D MembD; MembD.Initialize();
						if(m_pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))
						{
							if(MembD.aElemList.GetSize()<=1) aSelKey.Add(ElemK); // Single element member는 location무시
							else
							{
								CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
								int nPos = pMembCtrl->GetMembPosiByElem(ElemK);
								if(nPos == m_Data.nLocation) aSelKey.Add(ElemK); 
							}
						}
					}
					else aSelKey.Add(ElemK);// beam인데 공구리가 아니면 location 고려안함
				}
				else aSelKey.Add(ElemK);// Member에 assign되지 않았다(single element member로 간주)
			}
			else aSelKey.Add(ElemK); // Beam이 아니면 member location무시
		}
	}
	// Element Type이 맞지 않은것은 뺀다. 
	// 속도향상 고려
	int nTgtElemType;
	switch(m_nElemType)
	{
	case ET_TRUSS: nTgtElemType = TRUSS_EL; break;
	case ET_BEAM : nTgtElemType = BEAM_EL; break;
	case ET_WALL : nTgtElemType = WALL_EL; break;
	default: ASSERT(0); break;
	}
	
	CArray<T_ELEM_K, T_ELEM_K> aTgtElemK;
	aTgtElemK.SetSize(aSelKey.GetSize());
	int nCnt = 0;
	for (int n = 0; n < aSelKey.GetSize(); n++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[n], ElemD)) continue;
		if (ElemD.eltyp == nTgtElemType) aTgtElemK[nCnt++] = aSelKey[n];
	}
	aTgtElemK.SetSize(nCnt);
	aSelKey.Copy(aTgtElemK);
	
	if(aSelKey.GetSize()<1)
	{
		AfxMessageBox(_LS(IDS_CMD_IEHP_NO_COND_ELEM));
		return;
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelKey, FALSE, TRUE, TRUE);
	
}

BOOL CInelasticHingeAssignDlg_MEC::ExternalInit(T_IEHG_K key)
{
	if (key == 0) return FALSE;

	T_IEHG_D data;
	data.Initialize();
	m_pDoc->m_pAttrCtrl->GetIehg(key, data);
	//m_cmbHingeProp.ChangeSelect(data.IehpKey);
	ChangeIehpComboSel(data.IehpKey);

	OnSelchangeInelasticHingePropCmb();

	return TRUE;
}

int CInelasticHingeAssignDlg_MEC::TransMembType(int& nType)
{
	switch(nType)
	{
	case 1: // Column
		nType = 1;
		break;
	case 2: // Beam
		nType = 0;
		break;
	case 3: // Brace
		nType = 2;
		break;
	case 4: // Wall
		nType = 3;
		break;
	default: ASSERT(0);
		nType = -1;
		break;
	}

	return nType;
}

void CInelasticHingeAssignDlg_MEC::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		break;
	}
}
void CInelasticHingeAssignDlg_MEC::UpdateBuffer()
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
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_IEHP_ADD): case(UR_IEHP_DEL):
			//OnSelchangeInelasticHingePropCmb();
			UpdateIehpCombo();
			break;
		case(UR_IEHP_MFD):
			// 여기서는 저장만 하고 UR_IEHP_MFS에서 처리한다.
			bMFD = TRUE;
			break;
		case(UR_IEHP_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			UpdateIehpCombo();
			//OnSelchangeInelasticHingePropCmb();
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

void CInelasticHingeAssignDlg_MEC::OnImportHingeData()
{
	CInelasticHingeImportDlg dlg;
	dlg.DoModal();
}

void CInelasticHingeAssignDlg_MEC::OnInelasticGLinkSelRdo()
{
	DlgGlinkCtrlChanges();
}

void CInelasticHingeAssignDlg_MEC::OnInelasticGLinkPropBtn()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_BNDR_NLLINK_PROPERTY, 0));
}

bool CInelasticHingeAssignDlg_MEC::DlgGlinkCtrlChanges()
{
	//
	UpdateData();

	const bool bEnable = (m_nElemType == ET_NLNK && m_nAddDelete != 1);
	m_stcNlnkInput.EnableWindow(bEnable && m_rdoGlink2.GetCheck());
	m_edtNlnkInput.EnableWindow(bEnable && m_rdoGlink2.GetCheck());
	m_rdoGlink1.EnableWindow(bEnable && TRUE);
	m_rdoGlink2.EnableWindow(bEnable && TRUE);
	m_rdoGlink3.EnableWindow(bEnable && TRUE);
	m_cmbGlink.EnableWindow(bEnable && m_rdoGlink1.GetCheck());
	m_btnGlink.EnableWindow(bEnable && m_rdoGlink1.GetCheck());
	m_edtNlnkNumber.EnableWindow(bEnable && m_rdoGlink3.GetCheck());

	return true;
}

void CInelasticHingeAssignDlg_MEC::GetPointSpringSupportNode(CArray<UINT, UINT>& aKey)
{
	CArray<T_NSPR_K, T_NSPR_K> aKeyList;
	CArray<T_NSPR_D, T_NSPR_D&> aDataList;

	CArray<UINT, UINT> aExistPointSpringSupportNodeKey;
	int nNodeCnt = aKey.GetSize();
	for ( int i = 0; i < nNodeCnt; i++ )
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNsprNode(aKey[i], aKeyList, aDataList);
		if ( aKeyList.GetSize() > 0 )
		{
			aExistPointSpringSupportNodeKey.Add(aKey[i]);
		}
	}

	aKey.RemoveAll();
	aKey.Copy(aExistPointSpringSupportNodeKey);
}