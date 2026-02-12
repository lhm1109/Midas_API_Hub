#if !defined(AFX_TEMPERATURERISEPROPLISTDLG_H__)
#define      AFX_CMTEMPERATURERISEPROPLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TemperatureRisePropListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTemperatureRisePropListDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"
#define COLCOUNT 2
class __MY_EXT_CLASS__ CTemperatureRisePropListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CTemperatureRisePropListDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CCMTimeDefMatTypeDlg)
	enum { IDD = IDD_TEMPERATURE_RISE_PROP_LIST };
	CListCtrl	m_wndList;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTimeDefMatTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_MHTP_K Key, T_MHTP_D &Data);
	BOOL DeleteItem(T_MHTP_K Key, T_MHTP_D &Data);
	BOOL ModifyItem(T_MHTP_K KeyOld, T_MHTP_K Key, T_MHTP_D &Data);
	CString DataToStr(int i, T_MHTP_K Key, T_MHTP_D &Data);
	CString GetCopyName(CString strOrigin);

// Implementation
protected:
	CDBDoc *m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CCMTimeDefMatTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTEMPERATURERISEPROPDLG_H__52F6A038_B8DB_4792_BFF6_45C937C88B71__INCLUDED_)
