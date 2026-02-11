#if !defined(__SEISMIC_LOAD_LISTDLG_H__)
#define __SEISMIC_LOAD_LISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"      /////////
// CSeismicLoadListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSeismicLoadListDlg dialog

class __MY_EXT_CLASS__ CSeismicLoadListDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CSeismicLoadListDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListHeader();
	void MakeListData();

	enum { IDD = IDD_ETC_EP_SEISMIC_LIST };

protected:
	CListCtrl	m_wndSeismicLoadList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	virtual BOOL OnInitDialog();

	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdModify();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"      /////////

#endif // !defined(__SEISMIC_LOAD_LISTDLG_H__)
