// CMThrdHnge.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdHnge.h"
#include "CMThMainDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StoryData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThrdHnge dialog


CCMThrdHnge::CCMThrdHnge(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdHnge::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdHnge)
	m_sName = _T("");
	m_nType = 0;
	m_sElemK = _T("");
	m_nElemType = 0;
	//}}AFX_DATA_INIT

	m_aElement.Add(IDC_CMD_THRD_ELEM);
	m_aElement.Add(IDC_CMD_BAR_THKYNDEL);
	m_aElement.Add(IDC_ELEM_ENBUTTON);

	m_aLink.Add(IDC_CMD_THRD_GLLINK);
	m_aLink.Add(IDC_CMD_BAR_THNLLK_CBX);
}


void CCMThrdHnge::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdHnge)
	DDX_Control(pDX, IDC_CMD_BAR_THNLLK_CBX, m_cbxGL);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_cbxComp);
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cbxLComb);
	DDX_Control(pDX, IDC_CMD_BAR_THLOCATION, m_cbxLocation);
	DDX_Control(pDX, IDC_CMD_BAR_STORY		 , m_cbxStory);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_edElemK);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_sName);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_DEFORM, m_nType);
	DDX_Text(pDX, IDC_CMD_BAR_THKYNDEL, m_sElemK);
	DDX_Radio(pDX, IDC_CMD_ELEMTYPE_RDO1, m_nElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdHnge, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdHnge)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_DEFORM, OnGpsBarThtypeDeform)
	ON_BN_CLICKED(IDC_ELEM_ENBUTTON, OnElemEnbutton)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_FORCE, OnGpsBarThtypeDeform)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_THCOMP, OnSelchangeGpsBarThcomp)
	ON_CBN_SELCHANGE(IDC_CMD_BAR_STORY, OnChangeStory)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_ELEMTYPE_RDO1, OnCmdElemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ELEMTYPE_RDO2, OnCmdElemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ELEMTYPE_RDO3, OnCmdElemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ELEMTYPE_RDO4, OnCmdElemTypeRdo)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdHnge message handlers

BOOL CCMThrdHnge::OnExecute()
{
	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifyThrd(strOldName, m_pDlgData))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddThrd(m_pDlgData))
			return FALSE;
	}
	return TRUE;
}

void CCMThrdHnge::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 8);
}

void CCMThrdHnge::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(0, 8);
	}
}

void CCMThrdHnge::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
		//ChangeComponent(m_nType);
		if(SetInelstHngeInfo(m_pDlgData.nEntity))
		{
			if(m_bModify) UpdateLocation(m_pDlgData.nPosition-1);
			else          UpdateLocation(1);
		}
	}
}

void CCMThrdHnge::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nPosition = 1;
	m_pDlgData.nComponent = 1;
	m_pDlgData.nOption1 = 1;
	m_pDlgData.bAllMode = TRUE;

	m_bModify = FALSE;
	strOldName = _T("");
}

BOOL CCMThrdHnge::Dlg2Data()
{
	UpdateData(TRUE);

	CDBDoc *pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	T_ELEM_D DataElem;

	if (m_nElemType==2) // general link
	{
		int nSel = m_cbxGL.GetCurSel();
		if (nSel != CB_ERR) m_pDlgData.nEntity = m_cbxGL.GetItemData(nSel);
		m_pDlgData.nElemType4Iehg = 1;
	}
	else if(m_nElemType==3)
	{
		int nWallID = 0;
		_stscanf(m_sElemK.operator LPCTSTR(), _T("%d"), &nWallID);

		int nStorNameSelect = m_cbxStory.GetCurSel();
		if(nStorNameSelect == -1) return FALSE;
		m_cbxStory.GetLBText(nStorNameSelect, m_sStoryData);
		T_STOR_K StorK = pDoc->m_pAttrCtrl->GetStorKey(m_sStoryData);
		m_pDlgData.nEntity = pDoc->m_pAttrCtrl->GetElemKByWallIDStorK(nWallID, StorK);
		m_pDlgData.nPosition   = GetPosition();
		m_pDlgData.nElemType4Iehg = 3;
	}
	else
	{
		_stscanf(m_sElemK.operator LPCTSTR(), _T("%d"), &m_pDlgData.nEntity);
		m_pDlgData.nPosition   = GetPosition();

		if (m_nElemType==0) // beam
		{
			pDoc->m_pAttrCtrl->GetElem(m_pDlgData.nEntity, DataElem);
			if (DataElem.eltyp != BEAM_EL)
			{
				GSaveHistoryFormat(_LS(IDS_WG_CMD_THE_SELECTED_OBJECT_IS_NOT_A_BEAM_));
				return FALSE;
			}
			m_pDlgData.nElemType4Iehg = 0;
		}
		else if(m_nElemType==1) // truss
		{
			pDoc->m_pAttrCtrl->GetElem(m_pDlgData.nEntity, DataElem);
			// if (DataElem.eltyp != TRUSS_EL)
			if (!pDoc->m_pAttrCtrl->IsTruss(DataElem.eltyp))   // MNET:XXXX-HSSHIM-20091206. comp-only, tens-only Truss도 Truss임...^^
			{
				GSaveHistoryFormat(_LS(IDS_WG_CMD_THE_SELECTED_OBJECT_IS_NOT_A_TRUSS_));
				return FALSE;
			}
			m_pDlgData.nElemType4Iehg = 2;
		}
		else ASSERT(0);
	}

	m_pDlgData.nGraphStep = 0;
	m_pDlgData.nFunctionType = 9;
	m_pDlgData.Name = m_sName;
	m_pDlgData.nSubType    = m_nType + 1;
	m_pDlgData.nComponent  = m_cbxComp.GetCurSel() + 1;
	m_pDlgData.nOption1    = m_InelstHngeInfoBase.nHingeType + 1;
							
	UINT nLType,nLKey,nMnMxType;
	if(!m_cbxLComb.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{ 
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis = nLKey; 
	return TRUE;
}

void CCMThrdHnge::Data2Dlg()
{
	m_sName = m_pDlgData.Name;
	m_nType = m_pDlgData.nSubType - 1;

	if     (m_pDlgData.nElemType4Iehg==0) m_nElemType=0; // beam
	else if(m_pDlgData.nElemType4Iehg==1) m_nElemType=2; // general link
	else if(m_pDlgData.nElemType4Iehg==2) m_nElemType=1; // truss
	else if(m_pDlgData.nElemType4Iehg==3) m_nElemType=3; // wall
	else ASSERT(0);

	ChangeComponent(m_nType, m_nElemType);

	m_cbxComp.SetCurSel(m_pDlgData.nComponent-1);

	MakeNllkKeyList();
	if (m_nElemType==2) // general link
	{
		int nSize = m_cbxGL.GetCount();
		for (int i=0; i<nSize; i++)
		{
			if (m_pDlgData.nEntity==m_cbxGL.GetItemData(i))
			{
				m_cbxGL.SetCurSel(i);
				break;
			}
		}
	}
	else
	{
		UINT nEntity = m_pDlgData.nEntity;
		if(m_nElemType==3)
		{
			CDBDoc *pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
			nEntity = pDoc->m_pPostCtrl->GetUserWallId(m_pDlgData.nEntity);
		}
		if (nEntity == 0)
			m_sElemK.Format(_T(""));
		else
			m_sElemK.Format(_T("%d"), nEntity);
	}

	if(SetInelstHngeInfo(m_pDlgData.nEntity))
		UpdateLocation(m_pDlgData.nPosition-1);

	UpdateData(FALSE);
	OnCmdElemTypeRdo();
	m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	if(m_nElemType == 3)
	{
		T_STOR_K StorK;
		T_STOR_D StorD;
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStorKeyByElemKey(StorK,m_pDlgData.nEntity);
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStor(StorK,StorD);

		for(int i=0; i < m_cbxStory.GetCount(); i++)
		{						
			m_cbxStory.GetLBText(i,m_sStoryData);
			if(m_sStoryData.CompareNoCase(StorD.StoryName) == 0)
			{
				m_cbxStory.SetCurSel(i);
			}
		}
	}
}

BOOL CCMThrdHnge::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	m_bModify=FALSE;
	m_nComboType = -1;
	AlignCtrl();
	//MakeNllkKeyList();

	if (!m_bModify)
		DataInit();

	m_edElemK.SetAttElemList();
	m_edElemK.SetLButtonDownNotifyWindow(this);
	m_edElemK.SetEnterNotifyWindow(this);
	m_edElemK.SetModeToUse(MOUSEEDIT_USE_GET_GPSELEM_LIST);
	m_edElemK.SetMaxElemKeyNum(1);

	GotoDlgCtrl(&m_edElemK);
	m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY ); 
	Data2Dlg();

	if(SetInelstHngeInfo(m_pDlgData.nEntity))
	{
		if(m_bModify) UpdateLocation(m_pDlgData.nPosition-1);
		else          UpdateLocation(1);
	}
	
	// 위치 이동 Data2Dlg
	//m_cbxLComb.SetLoadType(D_SELECTLOAD_HISTORY ); 
	//m_cbxLComb.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThrdHnge::AlignCtrl()
{
	CRect src, target;
	GetDlgItem(IDC_CMD_BAR_THKYNDEL)->GetWindowRect(target);
	GetDlgItem(IDC_CMD_BAR_THNLLK_CBX)->GetWindowRect(src);

	int x, y;
	x = target.left - src.left;
	y = target.top - src.top;

	CDlgUtil::CtrlMoveDistXY(this, m_aLink, x, y);
	CDlgUtil::CtrlShowHide(this, m_aLink, FALSE);
}

BOOL CCMThrdHnge::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if(pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_RETURN && pWnd->GetDlgCtrlID() == IDC_CMD_BAR_THKYNDEL)
		{
			OnElemEnbutton();
			return TRUE;
		}
	}
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

void CCMThrdHnge::OnGpsBarThtypeDeform() 
{
	UpdateData(TRUE); 
	ChangeComponent(m_nType, m_nElemType);
	m_cbxComp.SetCurSel(0);
	OnSelchangeGpsBarThcomp();
}

void CCMThrdHnge::ChangeComponent(int nType, int nElemType)
{
	if(nType == 0) 
	{
		if (m_nComboType == 0)
			return;
		m_nComboType = 0;
		m_cbxComp.ResetContent(); 
		
		m_cbxComp.AddString(_LS(IDS_WG_CMD_DX));
		if(nElemType!=1) // truss type이 아니면
		{
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_DY));
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_DZ));
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_RX));
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_RY));
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_RZ));
		}
	}
	else if(nElemType==2 && nType==1)
	{
		if (m_nComboType == 1)
			return;
		m_nComboType = 1;
		m_cbxComp.ResetContent(); 

		m_cbxComp.AddString(_LSX(Fx));
		m_cbxComp.AddString(_LSX(Fy));
		m_cbxComp.AddString(_LSX(Fz));
		m_cbxComp.AddString(_LSX(Mx));
		m_cbxComp.AddString(_LSX(My));
		m_cbxComp.AddString(_LSX(Mz));
	}
	else
	{
		if (m_nComboType == 2)
			return;
		m_nComboType = 2;
		m_cbxComp.ResetContent(); 

		m_cbxComp.AddString(_LS(IDS_WG_CMD_AXIAL));
		if(nElemType!=1) // truss type이 아니면
		{
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_SHEAR_Y));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_SHEAR_Z));
			m_cbxComp.AddString(_LS(IDS_WG_CMD_TORSION));
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_MOMENT_Y));
		  m_cbxComp.AddString(_LS(IDS_WG_CMD_MOMENT_Z));
		}
	}
	m_cbxComp.SetCurSel(0);

	
}

LRESULT CCMThrdHnge::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_edElemK.IsKeyPressed())return 0L;
	if(m_edElemK.GetSafeHwnd()==(HWND)lParam) 
	{
		int	otype,ekey;
		UpdateData(TRUE);
		if(m_edElemK.GetCurPickedGPSObjectDataST(otype, ekey)) 
		{
			if(m_pDlgData.nEntity != ekey) 
			{
				T_ELEM_D DataElem;
				CDBDoc *pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
				pDoc->m_pAttrCtrl->GetElem(ekey, DataElem);
				if(m_nElemType==0 && !pDoc->m_pAttrCtrl->IsBeam(DataElem.eltyp))
				{
					GSaveHistoryFormat(_LS(IDS_WG_CMD_THE_SELECTED_OBJECT_IS_NOT_A_BEAM_));
					return 0L;
				}
				else if(m_nElemType==1 && !pDoc->m_pAttrCtrl->IsTruss(DataElem.eltyp))
				{
					GSaveHistoryFormat(_LS(IDS_WG_CMD_THE_SELECTED_OBJECT_IS_NOT_A_TRUSS_));
					return 0L;
				}
				else if(m_nElemType==3 && !pDoc->m_pAttrCtrl->IsWall(DataElem.eltyp))
				{
					GSaveHistoryFormat(_LS(IDS_WG_CMD_THE_SELECTED_OBJECT_IS_NOT_A_WALL_));
					return 0L;
				}

				if(m_nElemType == 3)
				{
					T_STOR_K nStorKey;
					T_STOR_D nStorD;
					CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStorKeyByElemKey(nStorKey,ekey);
					CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStor(nStorKey,nStorD);

					for(int i=0; i < m_cbxStory.GetCount(); i++)
					{						
						m_cbxStory.GetLBText(i,m_sStoryData);
						if(m_sStoryData.CompareNoCase(nStorD.StoryName) == 0)
						{
							m_cbxStory.SetCurSel(i);
						}
					}
				}

				m_pDlgData.nEntity = ekey;
				if(!SetInelstHngeInfo(ekey))return 0L;
				UpdateLocation(1);				
			}
		}
	}

	UINT nEntity = m_pDlgData.nEntity;
	if(m_nElemType==3)
	{
		CDBDoc *pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
		nEntity = pDoc->m_pPostCtrl->GetUserWallId(m_pDlgData.nEntity);
	}
	m_sElemK.Format(_T("%d"),nEntity);
	UpdateData(FALSE);
	return 0L;
}

BOOL  CCMThrdHnge::SetInelstHngeInfo(UINT ElemK)
{
	m_InelstHngeInfoBase.Initialize(); 

	CDBDoc *pDoc = CDBDoc::GetDocPoint(); 
	T_IEHG_D IehgD;
	T_IEHP_D IehpD;
	if(!pDoc->m_pAttrCtrl->GetIehgByElemK(ElemK, IehgD)) return FALSE;
	if(!pDoc->m_pAttrCtrl->CheckHingeResultExists(ElemK)) return FALSE;
	if(!pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) return FALSE;

	for(int i=0; i<6; i++)
	{
		if(IehpD.cDir[i] != '1')continue;
		if(m_cbxComp.GetCurSel()==i)
		{
			m_InelstHngeInfoBase.nHingeType = IehpD.nHingeType;
			if(m_InelstHngeInfoBase.nHingeType==4 && IehpD.nDefinition==0) m_InelstHngeInfoBase.nHingeType = 0; //sjhuh- 임시로 0으로 처리되게끔 함.
			if (m_InelstHngeInfoBase.nHingeType == 0)
			{
				m_InelstHngeInfoBase.nHingeLocation = IehpD.nHingeLocation[i];

				if(m_InelstHngeInfoBase.nHingeLocation == 2)
					m_InelstHngeInfoBase.nNumHinge=2;
				else
					m_InelstHngeInfoBase.nNumHinge=1;
			}
			else
			{
				m_InelstHngeInfoBase.nHingeLocation = 0;
				m_InelstHngeInfoBase.nNumHinge=IehpD.nSectionNum[i];
			}
			return TRUE;
		}
	}
	m_cbxLocation.ResetContent();
	return FALSE; 
}

void CCMThrdHnge::UpdateLocation(int nPos)
{
	int i; 
	CString sStr;
	m_cbxLocation.ResetContent(); 
	switch(m_InelstHngeInfoBase.nHingeType)
	{
	// Spring
	case 0:
		switch(m_InelstHngeInfoBase.nHingeLocation)
		{
		case 0: 
			//nPos = 1; 
			m_cbxLocation.AddString(_T("I-Pos")); 
			m_cbxLocation.SetCurSel(0);
			break; 
		case 1: 
			//nPos = 2; 
			m_cbxLocation.AddString(_T("J-Pos")); 
			m_cbxLocation.SetCurSel(0);
			break; 
		case 2: 
			//nPos = 1,2; 
			m_cbxLocation.AddString(_T("I-Pos")); 
			m_cbxLocation.AddString(_T("J-Pos"));
			m_cbxLocation.SetCurSel(nPos-1);
			break; 
		case 3: 
			m_cbxLocation.AddString(_T("Center"));  
			m_cbxLocation.SetCurSel(0);
			break; 
		}
		break; 
	// Sectional
	case 1:
	case 4: // Fiber Wall
		for(i=0; i<m_InelstHngeInfoBase.nNumHinge; i++)
		{
			sStr.Format(_T("%d-Pos"),i+1);
			m_cbxLocation.AddString(sStr);
		}
		m_cbxLocation.SetCurSel(nPos);
		break; 
	default:
		break; 
	}
}

void CCMThrdHnge::OnElemEnbutton() 
{
	UpdateData(TRUE);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); if(pDoc==NULL) { ASSERT(0); return;}
	int nKey = _ttoi(m_sElemK);
	int nEntity = 0;

	if(m_nElemType == 3) //Wall Type이면 nKey 값은 WallID이다.
	{
		BOOL bWallIsCollect = FALSE;
		CArray<T_ELEM_K,T_ELEM_K> aElemKeyList;
		CArray<int,int> aWallIdList;
		aWallIdList.Add(nKey);
		pDoc->m_pAttrCtrl->GetKeyListWallUser(&aWallIdList, NULL, &aElemKeyList);

		if(aElemKeyList.GetSize() == 0)
		{
			GSaveHistoryFormat(_LS(IDS_WG_GPS_The_InputWallID_is_not_Collectly));
			return;
		}

		int nStorNameSelect = -1;		
		nStorNameSelect = m_cbxStory.GetCurSel();
		if(nStorNameSelect == -1)
		{
			GSaveHistoryFormat(_LS(IDS_WG_GPS_The_StoryValue_is_Need));
			return;
		}
		m_cbxStory.GetLBText(nStorNameSelect, m_sStoryData);
						
		CStoryData* pStoryData = pDoc->GetStoryData();

		T_STOR_K StorK;
		T_STOR_D StorD;
		for(int i =0; i < aElemKeyList.GetSize(); i++)
		{
			T_ELEM_K nTempRootKey = -1;
			CArray<T_ELEM_K,T_ELEM_K> TempKeyList;
			pDoc->m_pAttrCtrl->GetKeyListBy_WallID_ByStor(TempKeyList, aElemKeyList[i], NULL, pStoryData);
			pDoc->m_pAttrCtrl->GetRootWallElemKey(TempKeyList, nTempRootKey);
			pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, nTempRootKey, pStoryData);
			pDoc->m_pAttrCtrl->GetStor(StorK,StorD);
			if(m_sStoryData.CompareNoCase(StorD.StoryName) == 0) 
			{
				nEntity = nTempRootKey;
				bWallIsCollect = TRUE;
				break;
			}
		}

		if(!bWallIsCollect)
		{
			int nStorNameSelect = -1;
			CString StorName;
			nStorNameSelect = m_cbxStory.GetCurSel();
			if(nStorNameSelect == -1)
			{
				GSaveHistoryFormat(_LS(IDS_WG_GPS_The_StoryValue_is_Need));
				return;
			}
			m_cbxStory.GetLBText(nStorNameSelect, StorName);

			GSaveHistoryFormat(_LS(IDS_WG_GPS_The_NotExist_Wall_Element_By_Floor_Wall_),StorName, nKey);
			return;
		}
	}
	else
	{
		nEntity = nKey;
		T_ELEM_D DataElem;
		pDoc->m_pAttrCtrl->GetElem(nKey,DataElem);

		if(m_nElemType==0 && !pDoc->m_pAttrCtrl->IsBeam(DataElem.eltyp))
		{
			GSaveHistoryFormat(_LS(IDS_WG_GPS_The_Selected_object_is_not_a_beam_));
			return;
		}
		else if(m_nElemType==1 && !pDoc->m_pAttrCtrl->IsTruss(DataElem.eltyp))
		{
			GSaveHistoryFormat(_LS(IDS_WG_GPS_The_Selected_object_is_not_a_truss_));
			return;
		}
	}

	if(nEntity==m_pDlgData.nEntity)return; 
	m_pDlgData.nEntity = nEntity;  
	if(!SetInelstHngeInfo(nEntity))return;
	UpdateLocation(0);
}

int CCMThrdHnge::GetPosition()
{
	int nPosition=0;

	if (!SetInelstHngeInfo(m_pDlgData.nEntity))
		return nPosition;       // Location Combo가 없는 경우이므로.. 0을 return;

	switch(m_InelstHngeInfoBase.nHingeType)
	{  
	case 0:  // Lumped
		switch(m_InelstHngeInfoBase.nHingeLocation)
		{
		case 0:   nPosition=2;  break; // I
		case 1:   nPosition=3;  break; // J
		case 2:   nPosition=m_cbxLocation.GetCurSel()+2; break; 
		case 3:   nPosition=1;  break; // Center
		}
		break; 
	// Sectional
	case 1:  // Distributed
	case 4:  // Fiber Wall
		nPosition = m_cbxLocation.GetCurSel()+1; // Index
		break; 
	case 2: case 3: // Spring, Truss
		break;
	default:
		ASSERT(0);
		break; 
	}
	return nPosition;
}

void CCMThrdHnge::OnChangeStory()
{
	UpdateData(TRUE);
	BOOL bWallIsCollect = FALSE;
	int nStorNameSelect = m_cbxStory.GetCurSel();
	if(nStorNameSelect == -1) return;

	m_cbxStory.GetLBText(nStorNameSelect, m_sStoryData);
	int nWallID = _ttoi(m_sElemK);

	CArray<T_ELEM_K,T_ELEM_K> aElemKeyList;
	CArray<int,int> aWallIdList;
	aWallIdList.Add(nWallID);
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetKeyListWallUser(&aWallIdList, NULL, &aElemKeyList);

	if(aElemKeyList.GetSize() == 0)
	{
		GSaveHistoryFormat(_LS(IDS_WG_GPS_The_InputWallID_is_not_Collectly));
		return;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//CStoryData StoryData(pDoc);
	//StoryData.MakeStoryData(1);
	CStoryData* pStoryData = pDoc->GetStoryData();

	T_STOR_K StorK;
	T_STOR_D StorD;
	UINT nElemK=NULL;
	for(int i =0; i < aElemKeyList.GetSize(); i++)
	{
		T_ELEM_K nTempRootKey = -1;
		CArray<T_ELEM_K,T_ELEM_K> TempKeyList;
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetKeyListBy_WallID_ByStor(TempKeyList, aElemKeyList[i], NULL, pStoryData);
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetRootWallElemKey(TempKeyList, nTempRootKey);
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStorKeyByElemKey(StorK, nTempRootKey, pStoryData);
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStor(StorK,StorD);
		if(m_sStoryData.CompareNoCase(StorD.StoryName) == 0) 
		{
			nElemK = nTempRootKey;
			bWallIsCollect = TRUE;
			break;
		}
	}

	if(!bWallIsCollect)
	{
		int nStorNameSelect = -1;
		CString StorName;
		nStorNameSelect = m_cbxStory.GetCurSel();
		if(nStorNameSelect == -1)
		{
			GSaveHistoryFormat(_LS(IDS_WG_GPS_The_StoryValue_is_Need));
			return;
		}
		m_cbxStory.GetLBText(nStorNameSelect, StorName);

		GSaveHistoryFormat(_LS(IDS_WG_GPS_The_NotExist_Wall_Element_By_Floor_Wall_),StorName, nWallID);
		return;
	}

	if(nElemK==m_pDlgData.nEntity)return; 
	m_pDlgData.nEntity = nElemK;  
	if(!SetInelstHngeInfo(nElemK))return; 
	UpdateLocation(1);
}

void CCMThrdHnge::OnSelchangeGpsBarThcomp() 
{
	if(!SetInelstHngeInfo(m_pDlgData.nEntity))return; 
	UpdateLocation(1);
}

BOOL CCMThrdHnge::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_THRD_D ThrdD;
	pDoc->m_pAttrCtrl->GetThrd(key, ThrdD);
	m_pDlgData = ThrdD;
	strOldName = ThrdD.Name;
	m_bModify = TRUE;
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
	Data2Dlg();

	m_edElemK.SetModeToUse(MOUSEEDIT_USE_GET_GPSELEM_LIST);
	m_edElemK.SetMaxElemKeyNum(1);
	GotoDlgCtrl(&m_edElemK);

	return TRUE;
}

void CCMThrdHnge::OnCmdElemTypeRdo()
{
	UpdateData();

	m_edElemK.SetModeToUse(MOUSEEDIT_USE_GET_GPSELEM_LIST);
	m_edElemK.SetMaxElemKeyNum(1);
	GotoDlgCtrl(&m_edElemK);

	ChangeComponent(m_nType, m_nElemType);

	GetDlgItem(IDC_STATIC_STORY_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_BAR_STORY)->EnableWindow(FALSE);

	if(m_nElemType == 3)
	{
		GetDlgItem(IDC_STATIC_STORY_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_BAR_STORY)->EnableWindow(TRUE);
		CArray<CString,CString&> StorNameList;
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStorList(StorNameList);

		m_cbxStory.ResetContent();
		for(int i=0; i<StorNameList.GetSize(); i++)
			m_cbxStory.AddString(StorNameList[i]);
		GetDlgItem(IDC_CMD_THRD_ELEM)->SetWindowText(_LS(IDS_WG_GPS_WALLID_NO));
	}
	else
	{
		GetDlgItem(IDC_CMD_THRD_ELEM)->SetWindowText(_LS(IDS_WG_GPS_ELEM_NO));
	}

	CDlgUtil::CtrlShowHide(this, m_aElement, m_nElemType!=2);
	CDlgUtil::CtrlShowHide(this, m_aLink,    m_nElemType==2);

	GetDlgItem(IDC_CMD_BAR_THLOCATION)->EnableWindow(m_nElemType==0 || m_nElemType==3);
}

void CCMThrdHnge::MakeNllkKeyList()
{
	CDBDoc *pDoc=CDBDoc::GetDocPoint();

	CArray<T_NLNK_K, T_NLNK_K> m_arNllk;
	pDoc->m_pAttrCtrl->GetNlnkKeyList(m_arNllk);
	if(!m_arNllk.GetSize()) return;

	CString   sNllk;
	T_NLNK_D  Data;
	m_cbxGL.ResetContent();
	int nIndex = 1;
	for (int i=0; i<m_arNllk.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetNlnk(m_arNllk[i],Data))
			continue;

		if(!pDoc->m_pAttrCtrl->CheckNlnkHingeResultExists(m_arNllk[i]))
			continue;

		if (pDoc->m_pAttrCtrl->ExistIehp(Data.IehpKey))
		{
			sNllk.Format(_T("%d (n1:%d,n2:%d)"),m_arNllk[i],Data.Node1,Data.Node2);
			m_cbxGL.AddString(sNllk);
			m_cbxGL.SetItemData(nIndex-1, m_arNllk[i]);
			nIndex++;
		}
	}

	if (m_cbxGL.GetCount() > 0)
		m_cbxGL.SetCurSel(0);
}
