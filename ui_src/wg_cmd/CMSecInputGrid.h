#if !defined(AFX_CMSECINPUTGRID_H__3027D3E1_32F1_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECINPUTGRID_H__3027D3E1_32F1_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSecInputGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSecInputGrid window

class CCMSecInputGrid : public CGXGridWnd
{
// Construction
public:
	CCMSecInputGrid();
	virtual ~CCMSecInputGrid();

// Attributes
public:
	CDBDoc*     m_pDoc;

// Operations
public:
	void InitGrid(CDBDoc* pDoc);
	void InitSectionByData(T_SECT_D* pDataSrc, BOOL bAfter);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSecInputGrid)
	//}}AFX_VIRTUAL
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);

// Implementation
public:
	void InitGridParam();
	void InitGridForRegular(T_SECT_D* pDataSrc);
	void InitGridForSrc(T_SECT_D* pDataSrc);
	void InitGridForUser(T_SECT_D* pDataSrc);
	void InitGridForCombined(T_SECT_D* pDataSrc);
	void InitGridForCombinedA(T_SECT_D* pDataSrc);
	void InitGridForCombinedB(T_SECT_D* pDataSrc);
	void InitGridForTapered(T_SECT_D* pDataSrc);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSecInputGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CGXGridParam* m_pParam;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECINPUTGRID_H__3027D3E1_32F1_11D3_92DE_0000C0B0E6B3__INCLUDED_)
