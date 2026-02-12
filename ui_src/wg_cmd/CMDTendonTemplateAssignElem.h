#pragma once

#include "CMDlgBase.h"
#include "MyListCtrl.h"

#include "..\wg_db\SelectCtrl.h"

class CCMDTendonTemplateDlg;
class CCMDTendonTemplateAssignElem : public CCMDlgBase
{

public:
	CCMDTendonTemplateAssignElem(CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateAssignElem();
	
	enum { IDD = IDD_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL};
	
protected:	

	CCMDTendonTemplateDlg* m_pParentWnd;

	CSelectEdit	  m_wndAssignElem;
	CMyListCtrl		m_listElems;
	CStringArray* m_pAssignElems;
	CStringArray  m_caAssignElems;

	BOOL m_bMsgIgnore;

protected:
	
	void RefreshList();
	void SetControl();
	void SetElemsCbxInit(BOOL bLast = FALSE);

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnCmdSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdDel();
	afx_msg void OnCmdDellAll();
	afx_msg void OnCmdAssignElemList();

	DECLARE_MESSAGE_MAP()
};
