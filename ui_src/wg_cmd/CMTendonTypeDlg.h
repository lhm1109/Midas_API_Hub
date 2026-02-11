#if !defined(__CMTENDONTYPEDLG_H__)
#define __CMTENDONTYPEDLG_H__

#include "HeaderPre.h"
#include "wg_cmdRes2.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTendonTypeDlg dialog
class __MY_EXT_CLASS__ CCMTendonTypeDlg : public CChildDialogMove ,  public CDBUpdateConnector
{
public:
	CCMTendonTypeDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CMD_DEF_TENDON_TYPE_DLG };

	CListCtrl	m_wndTendonTypes;

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	void SetListHeader();
	void MakeListData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdModify();
	afx_msg void OnDblclkCmdTendonTypeList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(__CMTENDONTYPEDLG_H__)
