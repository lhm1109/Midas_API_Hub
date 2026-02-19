
#pragma once

#include "wg_dgnRes2.h"
#include "../wg_base/wg_base_MenuBarChildDlg.h"
#include "../wg_db/DB_ST_DT_SP14SeisMembParams.h"

class CDBDoc;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnStl_SP14SeisMembParams_Dlg : public CMenuBarChildDlg {
private:
	static constexpr auto IDD = IDD_DGN_STL_SP14SeisMembParams_DLG;
	CDBDoc* doc = nullptr;
	T_SP14SeisMembParams_D data = {};
	int	option = 0;

public:
	CDgnStl_SP14SeisMembParams_Dlg(CDBDoc* new_doc, CWnd* parent = nullptr);

protected:
	virtual void DoDataExchange(CDataExchange* dex) override;

	DECLARE_MESSAGE_MAP();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
};

#include "HeaderPost.h"
