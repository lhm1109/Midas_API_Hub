// CPGFatigueParamDlg.cpp: implementation of the CCPGFatigueParamDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGFatigueParamDlg.h"
#include "CPGFatigueParamTabIDlg.h"
#include "CPGFatigueParamTabJDlg.h"
#include "CPGFatigueParamTabIDlg_CSA.h"
#include "CPGFatigueParamTabJDlg_CSA.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CCPGFatigueParamDlg::CCPGFatigueParamDlg(CWnd* pParent /*=NULL*/)
: CMenuBarChildDlg(CCPGFatigueParamDlg::IDD, pParent)
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_pDoc = CDBDoc::GetDocPoint();	

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bBothIJ = FALSE;
}

void CCPGFatigueParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Control(pDX, IDC_DGN_CPG_CPFP_TAB,         m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_CPFP_IJBOTH_CHK,  m_Ctrl_BothIJ);
}


BEGIN_MESSAGE_MAP(CCPGFatigueParamDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,      OnDgnCPGLoadAppAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,      OnDgnCPGLoadAppAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_CPFP_IJBOTH_CHK,  OnBothIJChk)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CPG_CPFP_TAB, OnSelchangeTab)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGFatigueParamDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL) return TRUE;

	m_Ctrl_BothIJ.SetCheck(TRUE);

	m_Tab.DeleteAllItems();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_CPG_CPFP_PLACEHOLDER);
	ASSERT(pPlaceHolder);
 	m_Tab.SetPlaceHolder(pPlaceHolder, FALSE);
	
	// CTBCpfp::ShowHideColsByCode과 함께 검토
	switch (m_nDgnCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			// I
			m_pSubDlg_I_CSA = new CCPGFatigueParamTabIDlg_CSA(this);
			m_Tab.AddTab(m_pSubDlg_I_CSA, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg_CSA::IDD, TRUE);

			// J
			m_pSubDlg_J_CSA = new CCPGFatigueParamTabJDlg_CSA(this);
			m_Tab.AddTab(m_pSubDlg_J_CSA, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabJDlg_CSA::IDD, TRUE);
		}
		break;
	case KDS_24_14_31_2018_CSG:
	case KSCE_LSD15_CSG:
		{
			// I
			m_pSubDlg_I = new CCPGFatigueParamTabIDlg(this);
			m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg::IDD, TRUE);

			// J
			m_pSubDlg_J = new CCPGFatigueParamTabJDlg(this);
			m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabJDlg::IDD, TRUE);
		}
		break;
	case STL_AASHTO_LRFD19: // Rating - Steel
		{
			// I
			m_pSubDlg_I_AASHTO19 = new CCPGFatigueParamTabIDlg_AASHTO19(this, 0);
			m_Tab.AddTab(m_pSubDlg_I_AASHTO19, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg_AASHTO19::IDD, TRUE);

			// J
			m_pSubDlg_J_AASHTO19 = new CCPGFatigueParamTabIDlg_AASHTO19(this, 1);
			m_Tab.AddTab(m_pSubDlg_J_AASHTO19, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabIDlg_AASHTO19::IDD, TRUE);
		}
		break;
	case STL_AASHTO_LRFD12: // Rating - Steel
		{
			// I
			m_pSubDlg_I = new CCPGFatigueParamTabIDlg(this);
			m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg::IDD, TRUE);

			// J
			m_pSubDlg_J = new CCPGFatigueParamTabJDlg(this);
			m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabJDlg::IDD, TRUE);
		}
		break;
	default: // 
		{
			// I
			int* pElemType= (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode))? NULL : &m_nElemType;
			m_pSubDlg_I = new CCPGFatigueParamTabIDlg(this, pElemType);
			m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg::IDD, TRUE);

			// J
			m_pSubDlg_J = new CCPGFatigueParamTabJDlg(this, pElemType);
			m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabJDlg::IDD, TRUE);
		}
		break;
	}	
	
	m_Tab.ShowTab(0);

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGFatigueParamDlg::InitCtrl()
{
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPFP_IJBOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPFP_TAB);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPFP_PLACEHOLDER);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPFP_APPLY);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPFP_CLOSE);
}

void CCPGFatigueParamDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX;
	int nDistY;

	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CPG_CPFP_IJBOTH_CHK)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode))
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CCPGFatigueParamDlg::ShowHideControls()
{
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
	GetDlgItem(IDC_DGN_CPG_CPFP_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CPG_CPFP_APPLY)->ShowWindow(SW_HIDE);
}

void CCPGFatigueParamDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

void CCPGFatigueParamDlg::Execute() 
{
	auto SetCommonData = [this](auto& rData) {
		T_CPFP_TYPE arType[2];
		Dlg2Data(arType);
		memcpy(&rData.TfType[0], &arType[0], sizeof(T_CPFP_TYPE));
		memcpy(&rData.TfType[1], &arType[1], sizeof(T_CPFP_TYPE));
		rData.bBothIJ = m_bBothIJ;
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_CPFP_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddCpfp(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_CPFV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddCpfv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelCpfp(selKeys) :
			m_pDoc->m_pDataCtrl->DelCpfv(selKeys);
		if (success) Initial_SelectItem();
	};

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}
		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, true);
		else
			ExcuteDelete(aSelKey, true);

	}
	else if (m_nElemType == ELEM_TYPE_VBEAM)
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, false);
		else
			ExcuteDelete(aSelKey, false);
	}
}

BOOL CCPGFatigueParamDlg::Dlg2Data(T_CPFP_TYPE* pTfType)
{
	UpdateData(TRUE);
	m_bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if (m_Tab.GetCurSel() == 0)
	{
		switch (m_nDgnCode)
		{
		case CSA_S6_14_CSG:
		case CSA_S6_19_CSG:
			m_pSubDlg_I_CSA->GetCpfpData();
			break;
		case STL_AASHTO_LRFD19:
			m_pSubDlg_I_AASHTO19->GetCpfpData();
			break;
		default:
			m_pSubDlg_I->GetCpfpData();
			break;
		}

		if (m_bBothIJ) // I, J 가 같을 경우
		{
			pTfType[0] = m_I_Data;
			pTfType[1] = pTfType[0];
		}
		else
		{
			pTfType[0] = m_I_Data;
			pTfType[1] = m_J_Data;
		}
	}
	else
	{
		if (m_bBothIJ)
		{
			pTfType[0] = m_I_Data;
			pTfType[1] = pTfType[0];
		}
		else
		{
			switch (m_nDgnCode)
			{
			case CSA_S6_14_CSG:
			case CSA_S6_19_CSG:
				m_pSubDlg_J_CSA->GetCpfpData();
				break;
			case STL_AASHTO_LRFD19:
				m_pSubDlg_J_AASHTO19->GetCpfpData();
				break;
			default:
				m_pSubDlg_J->GetCpfpData();
				break;
			}
			pTfType[0] = m_I_Data;
			pTfType[1] = m_J_Data;
		}
	}

	return TRUE;
}

void CCPGFatigueParamDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGFatigueParamDlg::OnBothIJChk() 
{
	m_bBothIJ = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
	{
		ControlsEnableDisableSubDlgJ(m_nDgnCode);		
	}
}

void CCPGFatigueParamDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bBothIJ = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 0) 
	{
		ResizeWindowSubDlgI(m_nDgnCode);		
	}
	else
	{
		ResizeWindowSubDlgJ(m_nDgnCode);		
	}
}

void CCPGFatigueParamDlg::OnDgnCPGLoadAppAddDel() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CPG_CPFP_IJBOTH_CHK)->EnableWindow(m_nOption==OPTION_ADD_REPLACE);

	EnableDisableControls();
	if(m_Tab.GetCurSel() == 0) 
	{
		ControlsEnableDisableSubDlgI(m_nDgnCode);		
	}
	else                       
	{
		ControlsEnableDisableSubDlgJ(m_nDgnCode);
	}
}

void CCPGFatigueParamDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	OnDgnCPGLoadAppAddDel();
}

void CCPGFatigueParamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		case D_UPDATE_BUFFER_BEFORE:
		case D_UPDATE_UNIT:
		case D_UPDATE_SEL_ADD:
		case D_UPDATE_SEL_DEL:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

void CCPGFatigueParamDlg::UpdateBuffer()
{
	switch (m_nDgnCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			m_Tab.DeleteAllItems();
			// I
			m_pSubDlg_I_CSA = new CCPGFatigueParamTabIDlg_CSA(this);
			m_Tab.AddTab(m_pSubDlg_I_CSA, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg_CSA::IDD, TRUE);

			// J
			m_pSubDlg_J_CSA = new CCPGFatigueParamTabJDlg_CSA(this);
			m_Tab.AddTab(m_pSubDlg_J_CSA, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabJDlg_CSA::IDD, TRUE);
		}
		break;
	case STL_AASHTO_LRFD19:
		{
			m_Tab.DeleteAllItems();
			// I
			m_pSubDlg_I_AASHTO19 = new CCPGFatigueParamTabIDlg_AASHTO19(this, 0);
			m_Tab.AddTab(m_pSubDlg_I_AASHTO19, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg_AASHTO19::IDD, TRUE);

			// J
			m_pSubDlg_J_AASHTO19 = new CCPGFatigueParamTabIDlg_AASHTO19(this, 1);
			m_Tab.AddTab(m_pSubDlg_J_AASHTO19, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabIDlg_AASHTO19::IDD, TRUE);
		}
		break;
	default:
		{
			m_Tab.DeleteAllItems();
			// I
			int* pElemType = (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode)) ? NULL : &m_nElemType;
			m_pSubDlg_I = new CCPGFatigueParamTabIDlg(this, pElemType);
			m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGFatigueParamTabIDlg::IDD, TRUE);

			// J
			m_pSubDlg_J = new CCPGFatigueParamTabJDlg(this, pElemType);
			m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGFatigueParamTabJDlg::IDD, TRUE);
		}
		break;
	}
	
	m_Tab.ShowTab(0);
	
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bMFD = FALSE;
	BOOL bOther=FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_CPGD_ADD):
		case(UR_CPGD_DEL):
			{
				BOOL bAllowCode = [](int nCode)
				{
					switch (nCode)											
					{
					case AASHTO_LRFD07_CSG:
					case AASHTO_LRFD12_CSG:
					case AASHTO_LRFD16_CSG:
					case AASHTO_LRFD17_CSG:
					case AASHTO_LRFD20_CSG:
					case KDS_24_14_31_2018_CSG:
					case KSCE_LSD15_CSG:
					case KSSC_2014_CSG:
					case CSA_S6_14_CSG:
					case CSA_S6_19_CSG:
					case STL_AASHTO_LRFD19:
						return TRUE;
					default:
						return FALSE;
					}					
				}(m_nDgnCode);

				GetDlgItem(IDC_DGN_CPG_OPT_GRP)->EnableWindow(bAllowCode);
				GetDlgItem(IDC_DGN_CPG_OPT_ADD_RDO)->EnableWindow(bAllowCode);
				GetDlgItem(IDC_DGN_CPG_OPT_DEL_RDO)->EnableWindow(bAllowCode);

				GetDlgItem(IDC_DGN_CPG_CPFP_IJBOTH_CHK)->EnableWindow(bAllowCode && m_nOption == 0);
				GetDlgItem(IDC_DGN_CPG_CPFP_TAB)->EnableWindow(bAllowCode && m_nOption == 0);

				if (m_Tab.GetCurSel() == 0)
				{
					ControlsEnableDisableSubDlgI(m_nDgnCode);
					ResizeWindowSubDlgI(m_nDgnCode);
				}
				else
				{
					ControlsEnableDisableSubDlgJ(m_nDgnCode);
					ResizeWindowSubDlgJ(m_nDgnCode);
				}
			}
			break; 
		}
	}  
}

void CCPGFatigueParamDlg::ResizeWindow()
{
	CRect rPage = 0;		
	if(m_Tab.GetCurSel() == 0) 
	{
		switch (m_nDgnCode)
		{
		case CSA_S6_14_CSG:
		case CSA_S6_19_CSG:
			{
				rPage = m_pSubDlg_I_CSA->GetResizeHeight();
			}
			break;
		case STL_AASHTO_LRFD19:
			{
				rPage = m_pSubDlg_I_AASHTO19->GetResizeHeight();
			}
			break;
		default:
			{
				rPage = m_pSubDlg_I->GetResizeHeight();
			}
			break;
		}		
	}
	else                       
	{
		switch (m_nDgnCode)
		{
		case CSA_S6_14_CSG:
		case CSA_S6_19_CSG:
			{
				rPage = m_pSubDlg_J_CSA->GetResizeHeight();
			}
			break;
		case STL_AASHTO_LRFD19:
			{
				rPage = m_pSubDlg_J_AASHTO19->GetResizeHeight();
			}
			break;
		default:
			{
				rPage = m_pSubDlg_J->GetResizeHeight();
			}
			break;
		}
	}

	int nHeight = rPage.bottom - rPage.top;
	
	CRect rTo;
	CRect rMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;
	
	// Tab Control Resize
	CWnd* pWnd = GetDlgItem(IDC_DGN_CPG_CPFP_TAB);
	pWnd->GetWindowRect(rMove);
	rMove.bottom = rMove.top + nHeight + globalUtils.ScaleByDPI(50);
	pWnd->SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width(), rMove.Height(), SWP_NOMOVE);
	
	// PlaceHolder Resize
	pWnd = GetDlgItem(IDC_DGN_CPG_CPFP_PLACEHOLDER);
	pWnd->GetWindowRect(rMove);
	rMove.bottom = rMove.top + nHeight;
	pWnd->SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width(), rMove.Height(), SWP_NOMOVE);
	
	// Move Controls
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_CPG_CPFP_APPLY);
	aControls.Add(IDC_DGN_CPG_CPFP_CLOSE);
	
	GetDlgItem(IDC_DGN_CPG_CPFP_TAB)->GetWindowRect(rTo);
	GetDlgItem(IDC_DGN_CPG_CPFP_APPLY)->GetWindowRect(rMove);
	
	nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(10);
	
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}
void CCPGFatigueParamDlg::SetDgnCode(int nDgnCode)
{
	m_nDgnCode = nDgnCode;
}

int  CCPGFatigueParamDlg::GetDgnCode()
{
	return m_nDgnCode;
}

void CCPGFatigueParamDlg::SubDlgCtrlEnableDisable()
{
	OnInitDialog();
	OnDgnCPGLoadAppAddDel();
}

void CCPGFatigueParamDlg::ControlsEnableDisableSubDlgI(int nCode)
{
	switch (nCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			if (m_pSubDlg_I_CSA->GetSafeHwnd() != 0)
			{
				m_pSubDlg_I_CSA->ControlsEnableDisable();
			}			
		}
		break;
	case STL_AASHTO_LRFD19:
		{
			if (m_pSubDlg_I_AASHTO19->GetSafeHwnd() != 0)
			{
				m_pSubDlg_I_AASHTO19->ControlsEnableDisable();
			}			
		}
		break;
	default:
		{
			if (m_pSubDlg_I->GetSafeHwnd() != 0)
			{
				m_pSubDlg_I->ControlsEnableDisable();
			}			
		}
		break;
	}
}

void CCPGFatigueParamDlg::ControlsEnableDisableSubDlgJ(int nCode)
{
	switch (nCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			if (m_pSubDlg_J_CSA->GetSafeHwnd() != 0)
			{
				m_pSubDlg_J_CSA->ControlsEnableDisable();
			}			
		}
		break;
	case STL_AASHTO_LRFD19:
		{
			if (m_pSubDlg_J_AASHTO19->GetSafeHwnd() != 0)
			{
				m_pSubDlg_J_AASHTO19->ControlsEnableDisable();
			}			
		}
		break;
	default:
		{
			if (m_pSubDlg_J->GetSafeHwnd() != 0)
			{
				m_pSubDlg_J->ControlsEnableDisable();
			}			
		}
		break;
	}
}

void CCPGFatigueParamDlg::ResizeWindowSubDlgI(int nCode)
{
	switch (nCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			m_pSubDlg_I_CSA->ResizeWindow();
		}
		break;
	case STL_AASHTO_LRFD19:
		{
			m_pSubDlg_I_AASHTO19->ResizeWindow();
		}
		break;
	default:
		{
			m_pSubDlg_I->ResizeWindow();
		}
		break;
	}		
}

void CCPGFatigueParamDlg::ResizeWindowSubDlgJ(int nCode)
{
	switch (nCode)
	{
	case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
		{
			m_pSubDlg_J_CSA->ResizeWindow();
		}
		break;
	case STL_AASHTO_LRFD19:
		{
			m_pSubDlg_J_AASHTO19->ResizeWindow();
		}
		break;
	default:
		{
			m_pSubDlg_J->ResizeWindow();
		}
		break;
	}	
}