#if !defined(AFX_DGNSTLKRPARAMTABLEDLG_H__)
#define AFX_DGNSTLKRPARAMTABLEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlKRParamTableDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlKRParamTableDlg dialog
class CDgnSteelKRGrid;

class __MY_EXT_CLASS__ CDgnStlKRParamTableDlg : public CDialogMove
{
// Construction
public:
	CDgnStlKRParamTableDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CDgnStlKRParamTableDlg)
	enum { IDD = IDD_DGN_STL_KR_PARAM_TABLE_DLG };	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlKRParamTableDlg)
	protected:
	virtual BOOL DestroyWindow();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*						m_pDoc;

	int               m_nElemCount;
	
	CDgnSteelKRGrid*  m_Grid;  
	 
	void DataToGrid();
	void GridToData();
	BOOL CheckInputData();	

	CArray<T_STDP_K, T_STDP_K> m_arElemk;
	CArray<T_STDP_D, T_STDP_D&> m_arStdpD;
	CArray<T_LENG_D, T_LENG_D&> m_arLengD;
	CArray<T_KFAC_D, T_KFAC_D&> m_arKfacD;
	//T_KEY_LIST m_aBngrK;
	//CArray<T_EWSF_D, T_EWSF_D&> m_arEwsdD;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnStlKRParamTableDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLKRPARAMTABLEDLG_H__)
