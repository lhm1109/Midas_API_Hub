#if !defined(AFX_DgnSODDesignParameterBaseDlg_H_)
#define AFX_DgnSODDesignParameterBaseDlg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSODDesignParameterBaseDlg.h : header file
//
#include "DgnDlgBase.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\DlgReplaceMgr.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

/////////////////////////////////////////////////////////////////////////////
// DgnSODDesignParameterBaseDlg dialog

class CDgnSODDesignParameterRUSDlg;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSODDesignParameterBaseDlg :public CDialogMove//: public CDgnDlgBase, public CDBUpdateConnector//public CDialog
{
// Construction
public:
	CDgnSODDesignParameterBaseDlg(CWnd* pParent = NULL);   
	~CDgnSODDesignParameterBaseDlg();  

// Dialog Data
	//{{AFX_DATA(CDgnSODDesignParameterBaseDlg)
	enum { IDD = IDD_DGN_SOD_PARAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// 	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveOrUpdate();
	void Data2Dlg();
	BOOL Dlg2Data();


protected:  
	int m_nCurrPage;
	CDBDoc* m_pDoc;
	T_SDPA_D m_MainData;

	CDlgReplaceMgr m_DlgPageMgr;
 	CDgnSODDesignParameterRUSDlg* m_pRUSDlg;
	//{{AFX_MSG(CDgnSODDesignParameterBaseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG

//	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"


#endif // !defined(AFX_CDgnSODDesignParameterBaseDlg_H_)
