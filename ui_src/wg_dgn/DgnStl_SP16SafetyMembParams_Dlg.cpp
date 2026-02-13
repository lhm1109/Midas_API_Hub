
#include "StdAfx.h"
#include "DgnStl_SP16SafetyMembParams_Dlg.h"

#include "../wg_base/TreeMenuBarBase.h"
#include "../wg_base/TreeMenuMode.h"
#include "../wg_base/I_GENModelBase.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/DataCtrl.h"
#include "../wg_db/ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnStl_SP16SafetyMembParams_Dlg::CDgnStl_SP16SafetyMembParams_Dlg(CDBDoc* new_doc, CWnd* parent)
	: CMenuBarChildDlg(IDD, parent), doc{ new_doc } {
	ASSERT(doc);
}

void CDgnStl_SP16SafetyMembParams_Dlg::DoDataExchange(CDataExchange* dex) {
	ASSERT(dex);
	CMenuBarChildDlg::DoDataExchange(dex);
	DDX_Radio(dex, IDC_DGN_STL_SP16SafetyMembParams_AR_RDO, option);
	DDX_Text(dex, IDC_DGN_STL_SP16SafetyMembParams_gamma_u_EDT, data.gamma_u);
	DDX_Text(dex, IDC_DGN_STL_SP16SafetyMembParams_gamma_resp_EDT, data.gamma_resp);
	DDX_Text(dex, IDC_DGN_STL_SP16SafetyMembParams_gamma_c_str_EDT, data.gamma_c_str);
	DDX_Text(dex, IDC_DGN_STL_SP16SafetyMembParams_gamma_c_stab_EDT, data.gamma_c_stab);
}

BEGIN_MESSAGE_MAP(CDgnStl_SP16SafetyMembParams_Dlg, CDialog)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
END_MESSAGE_MAP()

void CDgnStl_SP16SafetyMembParams_Dlg::OnDgnClose() {
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStl_SP16SafetyMembParams_Dlg::OnDgnExecute() {
	const auto ok = UpdateData(TRUE);
	ASSERT(ok);
	CArray<T_ELEM_K, T_ELEM_K> selected_keys, steel_keys;
	CTreeMenuBarBase::GetI_GENModelST()->GetSelectedElemKeyListForDgn(selected_keys);
	if (selected_keys.GetSize() > 0) {
		for (auto i_key = 0; i_key < selected_keys.GetSize(); ++i_key) {
			const auto key = selected_keys[i_key];
			auto elem = T_ELEM_D{};
			if (!doc->m_pAttrCtrl->GetElem(key, elem)) {
				ASSERT(FALSE);
				continue;
			}
			auto mat = T_MATD_D{};
			if (!doc->m_pAttrCtrl->GetMatlDesign(elem.elmat, mat)) {
				ASSERT(FALSE);
				continue;
			}
			if (mat.Type == _T("S") && (doc->m_pAttrCtrl->IsBeamTypeElem(key) || doc->m_pAttrCtrl->IsColmTypeElem(key) || doc->m_pAttrCtrl->IsBraceTypeElem(key))) {
				steel_keys.Add(key);
			}
		}
		if (option == 0) { // add or replace
			if (steel_keys.GetSize() > 0) {
				doc->m_pDataCtrl->Add_SP16SafetyMembParams(steel_keys, data);
			} else {
				AfxMessageBox(_LSX(No selected steel elements!));
			}
		} else if (option == 1) { // delete
			doc->m_pDataCtrl->Del_SP16SafetyMembParams(steel_keys);
		} else {
			ASSERT(FALSE);
		}
	} else {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	doc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}
