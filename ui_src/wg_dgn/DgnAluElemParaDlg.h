#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

class CDBDoc;

class CDgnAluParaDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnAluParaDlg(UINT nIDD, CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst) = 0;
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst) = 0;
public:
	void Initial_SelectItem();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
protected:
	//}}AFX_VIRTUAL
	// Generated message map functions
	afx_msg void OnDgnClose();
	afx_msg void Execute();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
class CDgnAluAribDlg : public CDgnAluParaDlg
{
public:
	CDgnAluAribDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst);
private:
	afx_msg void OnCheckMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
class CDgnAluAhtrDlg : public CDgnAluParaDlg
{
public:
	CDgnAluAhtrDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst);
private:
	CEditUnit m_Val;
	CTextUnit m_Unit;
};

//////////////////////////////////////////////////////////////////////////
class CDgnAluAwedDlg : public CDgnAluParaDlg
{
public:
	CDgnAluAwedDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst);
};

//////////////////////////////////////////////////////////////////////////
class CDgnAluAPhiDlg : public CDgnAluParaDlg
{
public:
	CDgnAluAPhiDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst);
};
