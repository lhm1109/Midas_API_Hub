#include "StdAfx.h"
#include "DgnStl_SP16CombinedSectParams_Dlg.h"

#include "../wg_base/TreeMenuBarBase.h"
#include "../wg_base/TreeMenuMode.h"
#include "../wg_base/I_GENModelBase.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/DataCtrl.h"
#include "../wg_db/UnitCtrl.h"
#include "../wg_db/ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnStl_SP16CombinedSectParams_Dlg::CDgnStl_SP16CombinedSectParams_Dlg(CDBDoc* new_doc, CWnd* parent)
	: CMenuBarChildDlg(IDD, parent), doc{ new_doc } {
	ASSERT(doc);
}

void CDgnStl_SP16CombinedSectParams_Dlg::DoDataExchange(CDataExchange* dex) {
	ASSERT(dex);
	CMenuBarChildDlg::DoDataExchange(dex);
	DDX_Radio(dex,   IDC_DGN_STL_SP16CombinedSectParams_AR_RDO, option);
	DDX_Radio(dex,   IDC_DGN_STL_SP16CombinedSectParams_WithPlanks_RDO, plank_connection_option);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_PlankHeight_EDT1, m_EditPlankHeight);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_PlankHeight_EDT1_UNIT, m_EditPlankHeightUnit);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_PlankThickness_EDT1, m_EditPlankThickness);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_PlankThickness_EDT1_UNIT, m_EditPlankThicknessUnit);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_CrossbarArea_EDT1, m_EditCrossbarArea);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_CrossbarArea_EDT1_UNIT, m_EditCrossbarAreaUnit);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_LatticeSpacing_EDT1, m_EditLatticeSpacing);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_LatticeSpacing_EDT1_UNIT, m_EditLatticeSpacingUnit);
	DDX_Radio(dex,   IDC_DGN_STL_SP16CombinedSectParams_LengthLyWholeEl_RDO, length_in_lattice_plane_ly);
	DDX_Radio(dex,	 IDC_DGN_STL_SP16CombinedSectParams_LengthLzWholeEl_RDO, length_in_lattice_plane_lz);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_LengthLySpacingCoeff_EDT1, m_EditLyLengthCoeff);
	DDX_Control(dex, IDC_DGN_STL_SP16CombinedSectParams_LengthLzSpacingCoeff_EDT1, m_EditLzLengthCoeff);
}

BEGIN_MESSAGE_MAP(CDgnStl_SP16CombinedSectParams_Dlg, CDialog)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_STL_SP16CombinedSectParams_WithPlanks_RDO, OnConnTypeChange)
	ON_BN_CLICKED(IDC_DGN_STL_SP16CombinedSectParams_WithLattice_RDO, OnConnTypeChange)
	ON_BN_CLICKED(IDC_DGN_STL_SP16CombinedSectParams_LengthLySpacingCoeff_RDO, OnConnTypeChange)
	ON_BN_CLICKED(IDC_DGN_STL_SP16CombinedSectParams_LengthLyWholeEl_RDO, OnConnTypeChange)
	ON_BN_CLICKED(IDC_DGN_STL_SP16CombinedSectParams_LengthLzSpacingCoeff_RDO, OnConnTypeChange)
	ON_BN_CLICKED(IDC_DGN_STL_SP16CombinedSectParams_LengthLzWholeEl_RDO, OnConnTypeChange)
END_MESSAGE_MAP()

void CDgnStl_SP16CombinedSectParams_Dlg::OnDgnClose() {
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStl_SP16CombinedSectParams_Dlg::OnConnTypeChange() {
	UpdateData(TRUE);
	UpdateControls();
}

void CDgnStl_SP16CombinedSectParams_Dlg::OnDgnExecute() {
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
				data.branch_connection_type = plank_connection_option;
				data.plank_height = m_EditPlankHeight.GetEditValue();
				data.plank_thickness = m_EditPlankThickness.GetEditValue();
				data.lattice_crossbar_area = m_EditCrossbarArea.GetEditValue();
				data.lattice_spacing = m_EditLatticeSpacing.GetEditValue();
				data.ly_branch_type = length_in_lattice_plane_ly;
				data.ly_lattice_spacing_coeff = m_EditLyLengthCoeff.GetEditValue();
				data.lz_branch_type = length_in_lattice_plane_lz;
				data.lz_lattice_spacing_coeff = m_EditLzLengthCoeff.GetEditValue();
				doc->m_pDataCtrl->Add_SP16CombinedSectParams(steel_keys, data);
			} else {
				AfxMessageBox(_LSX(No selected steel elements!));
			}
		} else if (option == 1) { // delete
			doc->m_pDataCtrl->Del_SP16CombinedSectParams(steel_keys);
		} else {
			ASSERT(FALSE);
		}
	} else {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	doc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}

BOOL CDgnStl_SP16CombinedSectParams_Dlg::OnInitDialog() {
	CMenuBarChildDlg::OnInitDialog();

	if ( doc == NULL )	return TRUE;

	Initial_Unit();
	Initial_Values();
	UpdateControls();

	return TRUE;
}

void CDgnStl_SP16CombinedSectParams_Dlg::Initial_Unit() {
	m_EditPlankHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditPlankHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditPlankThickness.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditPlankThicknessUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditCrossbarArea.SetUnitType(D_UNITSYS_BASE_AREA);
	m_EditCrossbarAreaUnit.SetUnitType(D_UNITSYS_BASE_AREA);
	m_EditLatticeSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditLatticeSpacingUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_EditLyLengthCoeff.SetUnitType(D_UNITSYS_NONE);
	m_EditLzLengthCoeff.SetUnitType(D_UNITSYS_NONE);
}

void CDgnStl_SP16CombinedSectParams_Dlg::Initial_Values() {
	T_UNIT_INDEX CurIndex;
	doc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	auto L_GetFact = [](int nLengIndex) -> double
	{
		switch (nLengIndex)
		{
		case D_UNITSYS_LENGTH_INDEX_M: { return 100.0; }
		case D_UNITSYS_LENGTH_INDEX_CM: { return 1.0; }
		case D_UNITSYS_LENGTH_INDEX_MM: { return 1.0 / 10.0; }
		case D_UNITSYS_LENGTH_INDEX_IN: { return 2.54; }
		case D_UNITSYS_LENGTH_INDEX_FT: { return 2.54*12.0; }
		default: { ASSERT(0);  return 0.0; }
		}
	};

	const double dFlen1 = L_GetFact(CurIndex.nBase_Length);
	const double dFlen2 = L_GetFact(D_UNITSYS_LENGTH_INDEX_MM);
	auto LengFactor = dgn::SafeDiv(dFlen1, dFlen2);
	auto AreaFactor = std::pow(LengFactor, 2);
	m_EditPlankHeight.SetEditUnit(data.plank_height / LengFactor);
	m_EditPlankThickness.SetEditUnit(data.plank_thickness / LengFactor);
	m_EditCrossbarArea.SetEditUnit(data.lattice_crossbar_area / AreaFactor);
	m_EditLatticeSpacing.SetEditUnit(data.lattice_spacing / LengFactor);
	m_EditLyLengthCoeff.SetEditUnit(1.0);
	m_EditLzLengthCoeff.SetEditUnit(1.0);
}

void CDgnStl_SP16CombinedSectParams_Dlg::UpdateControls() {
	BOOL plank_connection = plank_connection_option == 0;
	if (plank_connection) {
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_PlankHeight_EDT1)->EnableWindow(true);
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_PlankThickness_EDT1)->EnableWindow(true);
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_CrossbarArea_EDT1)->EnableWindow(false);
	}
	else {
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_PlankHeight_EDT1)->EnableWindow(false);
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_PlankThickness_EDT1)->EnableWindow(false);
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_CrossbarArea_EDT1)->EnableWindow(true);
	}
	BOOL ly_same_as_element = length_in_lattice_plane_ly == 0;
	if (ly_same_as_element) {
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_LengthLySpacingCoeff_EDT1)->EnableWindow(false);
	}
	else {
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_LengthLySpacingCoeff_EDT1)->EnableWindow(true);
	}
	BOOL lz_same_as_element = length_in_lattice_plane_lz == 0;
	if (lz_same_as_element) {
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_LengthLzSpacingCoeff_EDT1)->EnableWindow(false);
	}
	else {
		GetDlgItem(IDC_DGN_STL_SP16CombinedSectParams_LengthLzSpacingCoeff_EDT1)->EnableWindow(true);
	}
}
