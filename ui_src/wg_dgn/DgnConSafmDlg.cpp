#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConSafmDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDgnConSafmDlg::CDgnConSafmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConSafmDlg::IDD, pParent)
{
	m_dMtr1 = 1.2;
	m_dMtr2 = 1.0;
	m_nOption = 0;
}

void CDgnConSafmDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_CON_SAFM_AR_RDO, m_nOption);
	DDX_Text(pDX, IDC_DGN_CON_SAFM_EDT1, m_dMtr1);
	DDX_Text(pDX, IDC_DGN_CON_SAFM_EDT2, m_dMtr2);
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CDgnConSafmDlg, CDialog)
#undef CDialog
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
END_MESSAGE_MAP()


BOOL CDgnConSafmDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();

	return TRUE;
}


void CDgnConSafmDlg::OnDgnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConSafmDlg::OnDgnExecute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;  aSelKey.RemoveAll();
	CArray<T_ELEM_K, T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	// if Selected element is existed.
	if (aSelKey.GetSize() > 0)
	{
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			// Check Material.
			T_ELEM_D ElemD; ElemD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))
				continue;
			T_MATD_D MatlD; MatlD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD))
				continue;

			if (MatlD.Type != _T("C"))
				continue;

			if (m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp))
				rSelKey.Add(key);
		}

		T_SAFM_D SafmData;
		SafmData.Initialize();
		if (m_nOption == 0)	// add/replace
		{
			SafmData.dM_tr1 = m_dMtr1;
			SafmData.dM_tr2 = m_dMtr2;

			if (rSelKey.GetSize() > 0)
			{
				m_pDoc->m_pDataCtrl->AddSafm(rSelKey, SafmData);
			}
			else
			{
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK);
			}
		}
		else
		{
			m_pDoc->m_pDataCtrl->DelSafm(rSelKey);
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}