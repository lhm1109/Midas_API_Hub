// DgnLateralCapaResultDlg.h: interface for the CDgnLateralCapaResultDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNLATERALCAPARESULTDLG_H__19B32973_7016_4082_B125_28C500F2947F__INCLUDED_)
#define AFX_DGNLATERALCAPARESULTDLG_H__19B32973_7016_4082_B125_28C500F2947F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "DgnLateralCapaResultGrid.h"

class CDgnLateralCapaResultDlg : public CDialogMove  
{
// Construction
public:
	CDgnLateralCapaResultDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnLateralCapaResultDlg();  // destructor

public:
	CDBDoc*		m_pDoc;
	CDgnLateralCapaResultGrid m_wndGrid;
	_DGN_RESERVE_LATERAL_CAPA_RES m_ResData;
	CArray<int, int> m_arRowPosi;

public:
	void OnClose();
	void SetResData(_DGN_RESERVE_LATERAL_CAPA_RES& ResData);

// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaResultDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_RESULT_DLG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
//	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
//	void UpdateBuffer(LPARAM lHint, CObject* pHint); 

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaResultDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnSelectAll();
	afx_msg void OnUnSelectAll();
	afx_msg void OnWordReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Write_ResultData();
	int GetLimStateAtElem(CArray<UINT, UINT>& aElemK);
	void Set_Title();	
	void EnableCtrls();
	CString GetStrElemList(CArray<UINT, UINT>& KeyList);	
	CString Get_FailureName(int nType);

};

#endif // !defined(AFX_DGNLATERALCAPARESULTDLG_H__19B32973_7016_4082_B125_28C500F2947F__INCLUDED_)
