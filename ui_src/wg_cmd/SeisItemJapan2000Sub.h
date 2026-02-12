#if !defined(__SEISITEMJAPAN2000SUB_H__)
#define __SEISITEMJAPAN2000SUB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemJapan2000Sub.h : header file
//

#include "CMGXGridWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemJapan2000Sub dialog

class CSeisItemJapan2000Sub : public CDialogMove
{
// Construction
public:
	CSeisItemJapan2000Sub(CWnd* pParent = NULL);   // standard constructor
	void SetData(double dTX, double dTY,
		CArray<T_SEIS_JIS_AI_BASE, T_SEIS_JIS_AI_BASE&>* pData, BOOL bInit);

// Dialog Data
	//{{AFX_DATA(CSeisItemJapan2000Sub)
	enum { IDD = IDD_ETC_SEIS_ITEM_JP2000_SUB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisItemJapan2000Sub)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitGrid();
	void SetGridTitle();

	void CalcDefaultData();
	void Data2Grid();
	BOOL Grid2Data();

protected:
	BOOL m_bInit;
	CCMGXGridWnd2 m_wndGrid;
	double m_dTX, m_dTY; 
	CArray<T_SEIS_JIS_AI_BASE, T_SEIS_JIS_AI_BASE&>* m_pData;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisItemJapan2000Sub)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISITEMJAPAN2000SUB_H__)
