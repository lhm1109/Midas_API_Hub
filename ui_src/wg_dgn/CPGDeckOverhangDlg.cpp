// CPGDeckOverhangDlg.cpp: implementation of the CCPGDeckOverhangDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGDeckOverhangDlg.h"
#include "CPGCurveBridTabIDlg.h"
#include "CPGCurveBridTabJDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

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

CCPGDeckOverhangDlg::CCPGDeckOverhangDlg(CWnd* pParent /*=NULL*/)
: CMenuBarChildDlg(CCPGDeckOverhangDlg::IDD, pParent)
{
	m_nOption = 0;
	m_nElemType = 0;
	m_bBoth = TRUE;

	m_Data_I.Initialize();
	m_Data_J.Initialize();

	m_Data.Initialize();
}

void CCPGDeckOverhangDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio  (pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Radio  (pDX, IDC_DGN_CPG_ELEM_RDO,         m_nElemType);
	DDX_Check  (pDX, IDC_DGN_CPG_DOHL_BOTH_CHK,    m_bBoth);
	DDX_Control(pDX, IDC_DGN_CPG_DOHL_TAB,         m_Tab);
}

BEGIN_MESSAGE_MAP(CCPGDeckOverhangDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,      OnDgnCPGDOHLAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,      OnDgnCPGDOHLAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO,         OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO,        OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_DOHL_BOTH_CHK,    OnBothIJChk)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGDeckOverhangDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();	
	if(m_pDoc==NULL) return TRUE;

	m_Tab.DeleteAllItems();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_CPG_DOHL_PLACEHOLDER);
	ASSERT(pPlaceHolder);
 	m_Tab.SetPlaceHolder(pPlaceHolder);
	
	m_pSubDlg_I = new CCPGDeckOverhangPage(this, 0);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGDeckOverhangPage::IDD, TRUE);
	m_pSubDlg_J = new CCPGDeckOverhangPage(this, 1);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGDeckOverhangPage::IDD, TRUE);
	
	m_Tab.ShowTab(0);

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	return TRUE;
}

void CCPGDeckOverhangDlg::Execute()
{
	UpdateData(TRUE);

	auto ExcuteAddReplace = [this](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_DOHL_D rData;
			rData.Initialize();
			Dlg2Data(rData.DohlType);
			rData.bBothIJ = m_bBoth;
			if (m_pDoc->m_pDataCtrl->AddDohl(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_DOHV_D rData;
			rData.Initialize();
			T_DOHL_TYPE arType[2];
			Dlg2Data(arType);
			memcpy(&rData.DohlType[0], &arType[0], sizeof(T_DOHL_TYPE));
			memcpy(&rData.DohlType[1], &arType[1], sizeof(T_DOHL_TYPE));
			rData.bBothIJ = m_bBoth;
			if (m_pDoc->m_pDataCtrl->AddDohv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelDohl(selKeys) :
			m_pDoc->m_pDataCtrl->DelDohv(selKeys);
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

void CCPGDeckOverhangDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGDeckOverhangDlg::InitCtrl()
{
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_CPG_DOHL_BOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_CPG_DOHL_TAB);
	m_aPositionCtrl.Add(IDC_DGN_CPG_DOHL_PLACEHOLDER);
}

void CCPGDeckOverhangDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CPG_DOHL_BOTH_CHK)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

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

void CCPGDeckOverhangDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CPG_DOHL_BOTH_CHK)->EnableWindow(m_nOption == OPTION_ADD_REPLACE);
	if (m_Tab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
	else                        m_pSubDlg_J->ControlsEnableDisable();
}

void CCPGDeckOverhangDlg::ShowHideControls()
{
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
}

void CCPGDeckOverhangDlg::SetDgnCode(int nDgnCode)
{
	m_nDgnCode = nDgnCode;
}

int  CCPGDeckOverhangDlg::GetDgnCode()
{
	return m_nDgnCode;
}

void CCPGDeckOverhangDlg::SubDlgCtrlEnableDisable()
{
	OnInitDialog();
	OnDgnCPGDOHLAddDel();
}

void CCPGDeckOverhangDlg::OnBothIJChk() 
{
	UpdateData(TRUE);

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->ControlsEnableDisable();	
}

BOOL CCPGDeckOverhangDlg::Dlg2Data(T_DOHL_TYPE* pDohlType)
{
	UpdateData(TRUE);

	if(m_Tab.GetCurSel()==0) m_pSubDlg_I->GetDohlData();
	else                     m_pSubDlg_J->GetDohlData(); 

	if(m_bBoth)
	{
		pDohlType[0] = m_Data_I;
		pDohlType[1] = m_Data_I;
	}
	else
	{
		pDohlType[0] = m_Data_I;
		pDohlType[1] = m_Data_J;
	}

	return TRUE;
}

void CCPGDeckOverhangDlg::OnDgnCPGDOHLAddDel() 
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CCPGDeckOverhangDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}

void CCPGDeckOverhangDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCPGDeckOverhangDlg::UpdateBuffer()
{
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

			AlignControl();
			ShowHideControls();
			EnableDisableControls();
			T_CPGD_D CpgdD; CpgdD.Initialize();
			m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
			m_nDgnCode = CpgdD.iDgnCode;

			BOOL bAllowCode = FALSE;

			if     (m_nDgnCode == AASHTO_LRFD07_CSG) bAllowCode = TRUE;
			else if(m_nDgnCode == AASHTO_LRFD12_CSG) bAllowCode = TRUE;
			else if(m_nDgnCode == AASHTO_LRFD16_CSG) bAllowCode = TRUE;
            else if(m_nDgnCode == AASHTO_LRFD17_CSG) bAllowCode = TRUE;
            else if(m_nDgnCode == AASHTO_LRFD20_CSG) bAllowCode = TRUE;
			else if(m_nDgnCode == KSSC_2014_CSG) bAllowCode = TRUE;
			else                                     bAllowCode = FALSE; 

			GetDlgItem(IDC_DGN_CPG_DOHL_BOTH_CHK)->EnableWindow(bAllowCode && m_nOption==0);
			
			if(m_Tab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
			else                       m_pSubDlg_J->ControlsEnableDisable();
			break;
		}
	}  
}