#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"

class CCmdReinforceInfoDlg : public CDialogMove
{
	// Construction
public:
	CCmdReinforceInfoDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCmdReinforceInfoDlg)
	enum
	{
		IDD = IDD_CMD_REINFORCE_ASSESS_INFO_DLG
	};
private:
	CComboBox			m_AssessCombox;
	CComboBox			m_DesignCombox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_RCTI_D m_Data;
	int m_nBuildingType;
	void Data2Dlg();
	BOOL Dlg2Data();
public:
	virtual BOOL OnInitDialog();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	afx_msg void OnOK();
	afx_msg void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

