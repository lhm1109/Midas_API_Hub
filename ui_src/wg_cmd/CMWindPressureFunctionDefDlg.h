#if !defined(__CM_WIND_FUNC_DEF_DLG_H__)
#define __CM_WIND_FUNC_DEF_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMDlgBase.h"
#include "..\wg_base\ChildDialogMove.h"

#include "HeaderPre.h"      /////////
// CMWindPressureVelocityDefDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionDefDlg dialog

class __MY_EXT_CLASS__ CCMWindPressureFunctionDefDlg : public CChildDialogMove ,  public CDBUpdateConnector
{
// Construction
public:
	CCMWindPressureFunctionDefDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListHeader();
	void MakeListData();


	enum { IDD = IDD_CMD_DEF_WIND_FUNCTION };
	
protected:
	CListCtrl	m_wndFunctionList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	virtual BOOL OnInitDialog();

	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdModify();
	afx_msg void OnDblclkCmdTendonTypeList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"      /////////

#endif
