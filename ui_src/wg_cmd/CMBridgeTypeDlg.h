#if !defined(__CMBRIDGETYPEDLG_H__)
#define __CMBRIDGETYPEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBridgeTypeDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CCMBridgeTypeDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMBridgeTypeDlg : public CDialogMove
{
// Construction
public:
	CCMBridgeTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMBridgeTypeDlg)
	enum { IDD = IDD_CMD_BRIDGE_TYPE_DLG };
	int		m_nKindBridge;
	int		m_nTypeBridge;
	int		m_nDirectLoad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBridgeTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Dlg2Data();
	void Data2Dlg();

protected:
	T_BRGT_K m_Key;
	T_BRGT_D m_Data;
	CDBDoc* m_pDoc;

protected:

	// Generated message map functions
	//{{AFX_MSG(CCMBridgeTypeDlg)
	afx_msg void OnRemoveBridgeTypeData();
	afx_msg void OnOk();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMBRIDGETYPEDLG_H__)
