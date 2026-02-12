// CSeisCvlJpSetTargetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetTargetDlg.h"
#include "SeisCvlJpSetTargetUserDlg.h"
#include "SeisCvlJpSetTargetUpperDlg.h"
#include "SeisCvlJpSetTargetLowerDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetDlg

IMPLEMENT_DYNCREATE(CSeisCvlJpSetTargetDlg, MBaseDockingView)

CSeisCvlJpSetTargetDlg::CSeisCvlJpSetTargetDlg()
	: MBaseDockingView(CSeisCvlJpSetTargetDlg::IDD)
{
	// 클래스 만들때 Menu 추가 해야함
	AddChildDlgs();
	m_pDoc = CDBDoc::GetDocPoint();
	m_nCurChildDlg = -1;
}

CSeisCvlJpSetTargetDlg::~CSeisCvlJpSetTargetDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetTargetDlg::DoDataExchange(CDataExchange* pDX)
{
	MBaseDockingView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlJpSetTargetDlg)
	DDX_Control(pDX, IDC_TM_SIGR_MENU_CMB, m_cmbMenu);
	DDX_Control(pDX, IDC_TM_SIGR_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TM_SIGR_TABLE_BTN, m_btnTable);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetTargetDlg, MBaseDockingView)
	//{{AFX_MSG_MAP(CSeisCvlJpSetTargetDlg)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_TM_SIGR_MENU_CMB, OnSelChangeMenuCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetDlg diagnostics

void CSeisCvlJpSetTargetDlg::AddChildDlgs()
{
	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_TARGET_UPPER__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_TARGET_UPPER));
	m_aChildDlg.Add(new CSeisCvlJpSetTargetUpperDlg(this));
	m_aTableID.Add(0);

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_TARGET_LOWER__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_TARGET_LOWER));
	m_aChildDlg.Add(new CSeisCvlJpSetTargetLowerDlg(this));
	m_aTableID.Add(0);

	m_aDlgID.Add(__TMMODE_SEIS_CVL_JP_SETTING_TARGET_USER__);
	m_aMenyStr.Add(_LS(IDS_WG_TREEMENU_SETTING_TARGET_USER));
	m_aChildDlg.Add(new CSeisCvlJpSetTargetUserDlg(this));
	m_aTableID.Add(0);
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
BOOL CSeisCvlJpSetTargetDlg::ShowChildDlgByID(int nID)
{
	if (nID == __TMMODE_SEIS_CVL_JP_SETTING_TARGET_UPPER__)
	{
		ShowCurChildDlg(0);
		m_cmbMenu.SetCurSel(0);
		m_btnTable.EnableWindow(FALSE);
		return TRUE;
	}
	else if (nID == __TMMODE_SEIS_CVL_JP_SETTING_TARGET_LOWER__)
	{
		ShowCurChildDlg(1);
		m_cmbMenu.SetCurSel(1);
		m_btnTable.EnableWindow(FALSE);
		return TRUE;
	}
	else if (nID == __TMMODE_SEIS_CVL_JP_SETTING_TARGET_USER__)
	{
		ShowCurChildDlg(2);
		m_cmbMenu.SetCurSel(2);
		m_btnTable.EnableWindow(FALSE);
		return TRUE;
	}

	return FALSE;
}

void CSeisCvlJpSetTargetDlg::OnInitialUpdate()
{
	MBaseDockingView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetDlg message handlers

int CSeisCvlJpSetTargetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MBaseDockingView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}

BOOL CSeisCvlJpSetTargetDlg::ExistElementAtSigrUser(const T_SIGR_K& sigrK, const T_KEY_LIST& elemKeys)
{
	int nSize = elemKeys.GetSize();
	if (nSize < 1) return FALSE;

	T_SIGR_USER_K key;
	T_SIGR_USER_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSigrUser()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSigrUser()->GetNext(pos, key, data);

		//T_SIGR_K 와 동일한 key 는 같은 데이터이기 때문에 증복체크 하지않음(변경일 때)
		if (sigrK == key)
			continue;

		for (int i = 0; i < data.aInvestigatedElem.GetCount(); i++)
		{
			T_KEY elemKey = data.aInvestigatedElem[i];
			const auto itr = std::find(&elemKeys[0], &elemKeys[0] + nSize, elemKey);

			if (itr != &elemKeys[0] + nSize)
			{
				CString strDuplName = m_pDoc->m_pAttrCtrl2->GetQSigrUser()->GetSigrName(data.kSigr);
				T_KEY nDuplElemkey = elemKey;

				CString msg; msg.Format(_LS(IDS_IDD_TM_SIGR_MSG_DUPLI_ELEMENT), strDuplName, nDuplElemkey);
				AfxMessageBox(msg);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CSeisCvlJpSetTargetDlg::ExistElementAtSigrUpper(const T_SIGR_K& sigrK, const T_KEY_LIST& elemKeys)
{
	int nSize = elemKeys.GetSize();
	if (nSize < 1) return FALSE;

	T_SIGR_UPPER_K key;
	T_SIGR_UPPER_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetNext(pos, key, data);

		//T_SIGR_K 와 동일한 key 는 같은 데이터이기 때문에 증복체크 하지않음(변경일 때)
		if(sigrK == key)
			continue;

		for (int i = 0; i < data.aElements.GetCount(); i++)
		{
			T_KEY elemKey = data.aElements[i];
			const auto itr = std::find(&elemKeys[0], &elemKeys[0] + nSize, elemKey);

			if (itr != &elemKeys[0] + nSize)
			{
				CString strDuplName = m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetSigrName(data.kSigr);
				T_KEY nDuplElemkey = elemKey;

				CString msg; msg.Format(_LS(IDS_IDD_TM_SIGR_MSG_DUPLI_ELEMENT), strDuplName, nDuplElemkey);
				AfxMessageBox(msg);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CSeisCvlJpSetTargetDlg::ExistElementAtSigrLower(const T_SIGR_K& sigrK, const T_KEY_LIST& elemKeys)
{
	int nSize = elemKeys.GetSize();
	if (nSize < 1) return FALSE;

	T_SIGR_LOWER_K key;
	T_SIGR_LOWER_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetNext(pos, key, data);

		//T_SIGR_K 와 동일한 key 는 같은 데이터이기 때문에 증복체크 하지않음(변경일 때)
		if (sigrK == key)
			continue;

		CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;
		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetInvestigatedElem(key, aInvestigatedElem);

		for (int i = 0; i < aInvestigatedElem.GetCount(); i++)
		{
			T_KEY elemKey = aInvestigatedElem[i];
			const auto itr = std::find(&elemKeys[0], &elemKeys[0] + nSize, elemKey);

			if (itr != &elemKeys[0] + nSize)
			{
				CString strDuplName = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(data.kSigr);
				T_KEY nDuplElemkey = elemKey;

				CString msg; msg.Format(_LS(IDS_IDD_TM_SIGR_MSG_DUPLI_ELEMENT), strDuplName, nDuplElemkey);
				AfxMessageBox(msg);
				return TRUE;
			}
		}
	}

	return FALSE;
}
