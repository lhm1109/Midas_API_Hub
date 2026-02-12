#pragma once
#include "../wg_base/DialogMove.h"
struct DlgNumDet
{
	DlgNumDet()
	{
		num_ExponentFlag = 0;
		num_DecimalPoint = 2;
	}
	BOOL	num_ExponentFlag;
	int		num_DecimalPoint;
};

class CTreeNumDetTargetDlg : public CDialogMove
{
public:
	CTreeNumDetTargetDlg(DlgNumDet* pDlgData,CWnd* pParent = NULL);   // standard constructor
	CTreeNumDetTargetDlg(UINT nIDTemplate,CWnd* pParent = NULL);   // standard constructor

	DlgNumDet			*m_pDlgData;

	// Dialog Data
	//{{AFX_DATA(CTreeNumDetTargetDlg)
	enum { IDD = IDD_TM_NUMDET_TARGET };
	CString	m_DpEd;
	BOOL	m_bOkApply;
	BOOL	m_bExponent;
//public:
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTreeNumDetTargetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	////}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

