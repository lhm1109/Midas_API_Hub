#pragma once
#include "..\wg_base\DialogMove.h"
#include "LoadCaseForceControlledActionGrid.h"

class CLoadCaseForceControlledActionDlg : public CDialogMove
{
public:
	CLoadCaseForceControlledActionDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CMD_LFCA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	void Data2Grid();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
	CLoadCaseForceControlledActionGrid m_grid;
};