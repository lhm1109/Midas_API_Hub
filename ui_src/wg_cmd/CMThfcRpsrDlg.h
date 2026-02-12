#if !defined(AFX_CMTHFCRPSRDLG_H__C78AB722_D611_4E08_A942_607A13222233__INCLUDED_)
#define AFX_CMTHFCRPSRDLG_H__C78AB722_D611_4E08_A942_607A13222233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThfcRpsrDlg.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThfcRpsrDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMThfcRpsrDlg : public CDialogMove
{
// Construction
public:
	CCMThfcRpsrDlg(CWnd* pParent = NULL);   // standard constructor
	void SetEQData(CArray<T_THFC_BASE, T_THFC_BASE&>& arData, int nDataType, double dGravity, UINT Key);
	
// Dialog Data
	//{{AFX_DATA(CCMThfcRpsrDlg)
	enum { IDD = IDD_ETC_THFC_RPSR };
	MComboBox	m_cmbSpecType;
	BOOL	m_bLogX;
	BOOL	m_bLogY;
	float	m_fAmpScale;
	float	m_fTimeScale;
	float	m_fFrom;
	float	m_fTo;
	int 	m_nStep;
	MButton	m_btnDyGen;
	//}}AFX_DATA
	float m_fDamping[6];
	int m_nDataType;
	double m_dGravity;
	UINT m_nKey;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThfcRpsrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<T_THFC_BASE, T_THFC_BASE&> m_arData;
	

	CArray <UINT,UINT> m_ctlDamping;

	int m_nNumOfDamping;
	int m_nNumOfData;
	double Grav;

	SREGraphView m_GraphView;
	CMySRGraph m_Graph;

	void MakeCombo();
	void InitChart();
	void OnXScale();
	void SetLegend();
	void SetValue(int nIndex, int nGroup, double x, double y);
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	void OnETCSPFCCHARTXLogDraw();
	
	// MNET:2476-GSJI-20060929
	// Random주기의 지진파를 일정간격의 주기를 갖도록 수정하는 함수 - 불필요하다고 판단되어 수행하지 않음.
	//    int  OptimizeTHFCBASE(CArray<T_THFC_BASE, T_THFC_BASE&> &arOptData);  

	// Generated message map functions
	//{{AFX_MSG(CCMThfcRpsrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDampAddBtn();
	afx_msg void OnCmdGenerateBtn();
	afx_msg void OnETCSPFCCHARTXLog();
	afx_msg void OnETCSPFCCHARTYLog();
	afx_msg void OnSelchangeCmdSpectrumType();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHFCRPSRDLG_H__C78AB722_D611_4E08_A942_607A13222233__INCLUDED_)
