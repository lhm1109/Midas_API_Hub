#pragma once
#include "..\wg_base\DialogMove.h"
#include "RSModifyFactorGrid.h"

class CRSModifyFactorDlg : public CDialogMove
{
public:
	CRSModifyFactorDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CMD_RSMF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	void Data2Grid();
	BOOL Grid2Data();

	afx_msg void OnUseFactorChk();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
	CRSModifyFactorGrid m_grid;
	BOOL m_bUseModFactor;
};