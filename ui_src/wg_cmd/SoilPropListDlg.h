#if !defined(__SOILPROPLISTDLG_H__)
#define __SOILPROPLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"      /////////
// CSoilPropListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSoilPropListDlg dialog

class __MY_EXT_CLASS__ CSoilPropListDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CSoilPropListDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListHeader();
	void MakeListData();

	enum { IDD = IDD_ETC_EP_SOIL_LIST };

protected:
	CListCtrl	m_wndSoilList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	virtual BOOL OnInitDialog();

	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdCopy();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"      /////////

#endif // !defined(__SOILPROPLISTDlg_H__)
