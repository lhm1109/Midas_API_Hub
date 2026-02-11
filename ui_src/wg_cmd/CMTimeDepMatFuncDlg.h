
#include "../MIT_frx/MTabCtrl.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMTimeDepMatFuncDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMTimeDepMatFuncDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_TIME_DEP_MAT_FUNCTION };
	
	mit::frx::MTabCtrl  m_wndTab;
	CListCtrl	          m_wndFuncList;

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

public:
	void SetListCtrlHeader();
	void SetCurPage(int nIndex);
	void UpdateFunctionList();

protected:
	CString GetCopyName(CString strOrigin);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdModify();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdTimeDepMatFlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCopy();
	DECLARE_MESSAGE_MAP()

private:
	int     m_nCurSel;
};
#include "HeaderPost.h"
