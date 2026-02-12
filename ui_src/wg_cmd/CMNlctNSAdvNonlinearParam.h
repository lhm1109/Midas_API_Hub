#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
using namespace mit::frx;
#include <list>
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CMNlctNSAdvNonlinearParam : public CDialogMove
{
	DECLARE_DYNAMIC(CMNlctNSAdvNonlinearParam)

public:
	CMNlctNSAdvNonlinearParam(CWnd* pParent = NULL );
	virtual ~CMNlctNSAdvNonlinearParam();

	enum { IDD = IDD_CMD_NONL_PARAM };

	void SetData(T_NLCT_NLCTRL_PARM& data);
	void GetData(T_NLCT_NLCTRL_PARM& data);

	BOOL m_bDefaultSetting;
	BOOL m_bTerminateAnal;

	CEditUnit m_edtNumofIterStiff;
	CEditUnit m_edtMaxNumofQuasi;
	CEditUnit m_edtMaxNumofIter;
	CEditUnit m_edtMaxBisectLevel;
	MButton m_chkSmartBisection;
	CEditUnit m_edtDivThreshold;

	MComboBox m_cbxStiffScheme;
	MButton m_chkEnableLineSearch;
	MComboBox m_cbxLineSearchOpt;
	CEditUnit m_edtMaxLineSearch;
	CEditUnit m_edtLineSearchTol;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	void Data2Dlg();
	BOOL DoApply();


	void SetStiffUpdateSchemeCbx();
	void SetLineSearchOptCmb();
	void EnableLineSearch(bool bEnable);
	afx_msg void OnCmdCheckDefault();
	afx_msg void OnCmdCheckLineSearch();
	afx_msg void OnCmdChangeSchemeCbx();
	afx_msg void OnCmdChangeLineSearchOptCbx();

	DECLARE_MESSAGE_MAP()
	
	T_NLCT_NLCTRL_PARM	m_Data;
};

#include "HeaderPost.h"
