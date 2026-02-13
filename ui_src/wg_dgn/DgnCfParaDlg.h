#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

class CDBDoc;

class CDgnCfParaDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnCfParaDlg(UINT nIDD, CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
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
	CDBDoc* m_pDoc;
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDgnCfStabCoefDlg dialog
class CDgnCfStabCoefDlg : public CDgnCfParaDlg
{
public:
	CDgnCfStabCoefDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst);
};

/////////////////////////////////////////////////////////////////////////////
// CDgnCfOpenSectCoefDlg dialog
class CDgnCfOpenSectCoefDlg : public CDgnCfParaDlg
{
public:
	CDgnCfOpenSectCoefDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc,  T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc,  T_KEY_LIST &lst);
};


/////////////////////////////////////////////////////////////////////////////
// CDgnCfPurlinDlg dialog
class CDgnCfPurlinDlg : public CDgnCfParaDlg
{
public:
	CDgnCfPurlinDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc, T_KEY_LIST& lst);
	virtual BOOL OnDelData(CDBDoc* pDoc, T_KEY_LIST& lst);
};


/////////////////////////////////////////////////////////////////////////////
//CDgnCfPhiCoefDlg dialog
class CDgnCfPhiCoefDlg : public CDgnCfParaDlg
{
public:
	CDgnCfPhiCoefDlg(CWnd* pParent = NULL);   // standard constructor
protected:
	virtual void Initial_Data();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnAddData(CDBDoc* pDoc, T_KEY_LIST &lst);
	virtual BOOL OnDelData(CDBDoc* pDoc, T_KEY_LIST &lst);
};
