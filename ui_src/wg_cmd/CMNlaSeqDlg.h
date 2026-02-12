#if !defined(AFX_CMNLASEQDLG_H__8655DBC2_7086_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMNLASEQDLG_H__8655DBC2_7086_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNlaSeqDlg.h : header file
//

#include "CMDlgBase.h"

#include "..\wg_db\wg_db_DBDoc.h"

struct seqdata
{
	T_STLD_K	stldKey;
	CString		stldName;
};

/////////////////////////////////////////////////////////////////////////////
// CCMNlaSeqDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMNlaSeqDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMNlaSeqDlg(CWnd* pParent = NULL);   // standard constructor

	void	InitSeqDataList();

	void	SetLeftHeaderTitle();
	void	MakeLeftItemEx();
	int		FindInsertionPosLeft(T_STLD_K Key);
	BOOL	InsertItemLeft(T_STLD_K Key, CString strLcName);
	void	InitLeftItem();

	void	SetRightHeaderTitle();
	void	MakeRightItemEx();
	CString	DataToStr(int i, int nNum);
	void	InitRightItem();


	BOOL	CheckLDSQData(int key);
// Dialog Data
	//{{AFX_DATA(CCMNlaSeqDlg)
	enum { IDD = IDD_ETC_NLASEQ };
	CListCtrl	m_RightList;
	CListCtrl	m_LeftList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNlaSeqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*		m_pDoc;
	CArray<seqdata, seqdata>	m_aSeqData;
	CArray<seqdata, seqdata>	m_aLcData;

	// Generated message map functions
	//{{AFX_MSG(CCMNlaSeqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdNlaseqNlacBtn();
	afx_msg void OnCmdNlaseqLeftBt();
	afx_msg void OnCmdNlaseqRightBt();
	virtual void OnOK();
	afx_msg void OnCmdNlaseqRlistUp();
	afx_msg void OnCmdNlaseqRlistDn();
	afx_msg void OnDblclkCmdNlaseqLlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdNlaseqRlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMNLASEQDLG_H__8655DBC2_7086_11D5_880F_00010263A1F4__INCLUDED_)
