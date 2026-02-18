#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\mit_frx\MComboBox.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "ConBarInfoGrid.h"

using namespace mit::frx;
class CDgnConBarInfoDlg : public CDialogMove
{
public:
	CDgnConBarInfoDlg(CWnd* pParent = NULL);
	~CDgnConBarInfoDlg();

// Dialog Data
	enum { IDD = IDD_DGN_CON_BAR_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	// Generated message map functions
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	afx_msg void OnSelChangeStandardCombo();

	DECLARE_MESSAGE_MAP()

protected:
	void InitCmbStandard();

public:
	CString m_strTotalReBarSize;

protected:
	CMatlDB* m_pMatlDB;
	CConBarInfoGrid* m_pConBarInfoGrid;

	MComboBox m_cmbStandard;
};