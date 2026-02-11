#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"

class CmdTotalAppraisalInformationDlg : public CDialogMove
{
	// Construction
public:
	CmdTotalAppraisalInformationDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CmdTotalAppraisalInformationDlg)
	enum
	{
		IDD = IDD_CMD_APPRAISAL_INFO_DLG
	};
private:
	int			m_nBldType;
	CComboBox	m_CodeCombox;
	CFormulaEdit m_MainAMinEdit;
	CFormulaEdit m_MainBMaxEdit;
	CFormulaEdit m_MainBMinEdit;
	CFormulaEdit m_MainCMaxEdit;
	CFormulaEdit m_MainCMinEdit;
	CFormulaEdit m_MainDMaxEdit;
	CFormulaEdit m_SecondaryAMinEdit;
	CFormulaEdit m_SecondaryBMaxEdit;
	CFormulaEdit m_SecondaryBMinEdit;
	CFormulaEdit m_SecondaryCMaxEdit;
	CFormulaEdit m_SecondaryCMinEdit;
	CFormulaEdit m_SecondaryDMaxEdit;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_TAIF_D	m_Data;

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitCode(int nBldType);
public:
	virtual BOOL OnInitDialog();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnChangeBuildingType();
	afx_msg void OnUpdateDate_MainB();
	afx_msg void OnUpdateDate_MainC();
	afx_msg void OnUpdateDate_MainD();
	afx_msg void OnUpdateDate_SecondaryB();
	afx_msg void OnUpdateDate_SecondaryC();
	afx_msg void OnUpdateDate_SecondaryD();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

