#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenAssignReinforceSectDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\ViewBuff.h"

#include "..\wg_db\DBCodeDef.h"
#include "..\wg_cmd\ExportFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnGenAssignReinforceSectDlg::CDgnGenAssignReinforceSectDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenAssignReinforceSectDlg::IDD, pParent)
{
	m_nOption = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenAssignReinforceSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_DAMP_DSST_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_1_COMBO, m_RfstCombox);
}


BEGIN_MESSAGE_MAP(CDgnGenAssignReinforceSectDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenAssignReinforceSectDlg)
	ON_BN_CLICKED(IDC_DGN_BUTTON1, OnArst)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnGenAssignReinforceSectDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	m_RfstCombox.ResetContent();
	CDC* pDC = m_RfstCombox.GetDC();
	CArray<T_RFST_K, T_RFST_K> aRfstKeyList;
	m_pDoc->m_pAttrCtrl->GetRfstKeyList(aRfstKeyList);
	T_RFST_D RfstD;
	int maxTextLength = 0;
	CString str;
	for(int i = 0; i < aRfstKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetRfst(aRfstKeyList[i], RfstD);
		str.Format(_T("%d:%s"), aRfstKeyList[i], RfstD.strReinforceSectName);
		int nIndex = m_RfstCombox.AddString(str);
		m_RfstCombox.SetItemData(nIndex, aRfstKeyList[i]);
		maxTextLength = max(maxTextLength, pDC->GetTextExtent(RfstD.strReinforceSectName).cx);
	}
	m_RfstCombox.SetCurSel(0);
	m_RfstCombox.SetDroppedWidth(maxTextLength + 10);

	return TRUE;
}


void CDgnGenAssignReinforceSectDlg::OnDgnExecute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<T_ELEM_K, T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		T_RFST_K rfstK = m_RfstCombox.GetItemData(m_RfstCombox.GetCurSel());
		T_ELEM_D ElemD;
		T_MATL_D MatlD;
		if (m_nOption == 0)
		{
			for (int i = aSelKey.GetSize() - 1; i >= 0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;			
				// Beam, Truss, Wall.
				if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) continue;
				rSelKey.Add(ElemK);
			}
		}
		else
		{
			for (int i = aSelKey.GetSize() - 1; i >= 0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
				// Beam, Truss, Wall.
				if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) continue;
				rSelKey.Add(ElemK);
			}
		}

		T_ARST_D ArstD;
		ArstD.Initialize();
		ArstD.ReinforceSectKey = rfstK;;

		if(m_nOption == 0)
			m_pDoc->m_pDataCtrl->AddArst(rSelKey, ArstD);
		else
			m_pDoc->m_pDataCtrl->DelArst(rSelKey);

		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else	
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

void CDgnGenAssignReinforceSectDlg::OnArst()
{
	CMD_DoModal1(D_CMD_REINFORCE_DEFINE_SECT_DLG);
}

void CDgnGenAssignReinforceSectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_BUFFER_AFTER:
		UpdateRfst();
	break;
	default:
		ASSERT(TRUE);
	}
}

void CDgnGenAssignReinforceSectDlg::UpdateRfst()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bRFST = FALSE;
	while (pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case UR_RFST_ADD:
		case UR_RFST_DEL:
		case UR_RFST_MFD:
		case UR_RFST_MFS:
			bRFST = TRUE;
			break;
		default:
			break;
		}
	}
	if (bRFST)
	{
		int nCurSel = m_RfstCombox.GetCurSel();
		m_RfstCombox.ResetContent();
		CDC* pDC = m_RfstCombox.GetDC();
		CArray<T_RFST_K, T_RFST_K> aRfstKeyList;
		m_pDoc->m_pAttrCtrl->GetRfstKeyList(aRfstKeyList);
		T_RFST_D RfstD;
		int maxTextLength = 0;
		CString str;
		for (int i = 0; i < aRfstKeyList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetRfst(aRfstKeyList[i], RfstD);
			str.Format(_T("%d:%s"), aRfstKeyList[i], RfstD.strReinforceSectName);
			int nIndex = m_RfstCombox.AddString(str);
			m_RfstCombox.SetItemData(nIndex, aRfstKeyList[i]);
			maxTextLength = max(maxTextLength, pDC->GetTextExtent(RfstD.strReinforceSectName).cx);
		}
		m_RfstCombox.SetCurSel(nCurSel);
		m_RfstCombox.SetDroppedWidth(maxTextLength + 10);
	}
}

