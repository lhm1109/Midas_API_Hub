#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"

class CCmdReinforceDesignDlg : public CDialogMove
{
	// Construction
public:
	CCmdReinforceDesignDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCmdReinforceDesignDlg)
	enum
	{
		IDD = IDD_CMD_REINFORCE_DESIGN_PARA_DLG
	};
private:
	int				m_nReinforceMethod;
	int				m_nVerificationProcess;
	CComboBox		m_StructureType;
	CComboBox		m_ImportanceFactor;
	CFormulaEdit	m_AreaRatio;
	CFormulaEdit	m_ModulusRatio[2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_RDPA_D m_Data;
	void Data2Dlg();
	BOOL Dlg2Data();
public:
	virtual BOOL OnInitDialog();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnDetail();
	afx_msg void OnUpdateMethod();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

