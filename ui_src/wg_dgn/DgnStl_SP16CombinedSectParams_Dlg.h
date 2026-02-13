#pragma once

#include "wg_dgnRes2.h"
#include "../wg_base/wg_base_MenuBarChildDlg.h"
#include "../wg_db/DB_ST_DT_SP16CombinedSectParams.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CDBDoc;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnStl_SP16CombinedSectParams_Dlg : public CMenuBarChildDlg {
private:
	static constexpr auto IDD = IDD_DGN_STL_SP16CombinedSectParams_DLG;
	CDBDoc* doc = nullptr;
	T_SP16CombinedSectParams_D data = {};
	int	option = 0;
	int plank_connection_option = SP16CSP_RDO_BRANCH_CONN_LATTICE;
	int length_in_lattice_plane_ly = SP16CSP_RDO_BRANCH_LENGTH_LATTICE_SPACING_COEFF;
	int length_in_lattice_plane_lz = SP16CSP_RDO_BRANCH_LENGTH_SAME;
	void UpdateControls();
	void OnConnTypeChange();

public:
	CDgnStl_SP16CombinedSectParams_Dlg(CDBDoc* new_doc, CWnd* parent = nullptr);
	void Initial_Unit();
	void Initial_Values();
	CEditUnit m_EditPlankHeight;
	CTextUnit m_EditPlankHeightUnit;
	CEditUnit m_EditPlankThickness;
	CTextUnit m_EditPlankThicknessUnit;
	CEditUnit m_EditCrossbarArea;
	CTextUnit m_EditCrossbarAreaUnit;
	CEditUnit m_EditLatticeSpacing;
	CTextUnit m_EditLatticeSpacingUnit;
	CEditUnit m_EditLyLengthCoeff;
	CEditUnit m_EditLzLengthCoeff;

protected:
	virtual void DoDataExchange(CDataExchange* dex) override;

	DECLARE_MESSAGE_MAP();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
};

#include "HeaderPost.h"
