#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "HeaderPre.h"


class __MY_EXT_CLASS__ CCmdDampCheckInfoDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCmdDampCheckInfoDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCmdDampCheckInfoDlg)
	enum {IDD = IDD_CMD_DAMP_CH_INFO_DLG};
private:
	CFormulaEdit m_wndRxCoeEdit;
	CFormulaEdit m_wndRzCoeEdit;

	MComboBox			m_LcomCombox;
	CMyCheckComboBox	m_rxCombox;
	CMyCheckComboBox	m_rzCombox;
	int m_nLmtType;

	//Buffer the index and type of combox
	CArray<UINT,UINT>	m_aRx[2];
	CArray<UINT, UINT>	m_aRz[2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_DPCI_D m_Data;
	int m_nSeisStor;
	int m_nUpStor;
	int m_nLcom;
	BOOL m_bInit;
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetCodeCmb();
	void SetLcomGCmb();
	void SetRsCmb();
public:
	virtual BOOL OnInitDialog();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	afx_msg void OnGeneralLcomBtn();
	afx_msg void OnOK();
	afx_msg void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

