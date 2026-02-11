#if !defined(AFX_CMSECTITEMGRID4PSC_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_)
#define AFX_CMSECTITEMGRID4PSC_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemGrid4Psc.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

class CCMSecViewWnd;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Psc window

class CCMSectItemGrid4Psc : public CGXGridWnd
{
// Construction
public:
	CCMSectItemGrid4Psc();
	virtual ~CCMSectItemGrid4Psc();

//----------------------------------------------------------------------
// Attributes
public:

//----------------------------------------------------------------------
// Operations
public:
	void InitGrid(CCMSecViewWnd* pPreview, CCMSectItemGrid4Psc* pOtherEnd);
	void SetDataSource(T_SECT_SECTBASE_D* pSectBase, int nSectType);
	void ChangeShape(int nNewShapeIndex);
	void ShowData();
	BOOL SaveData();
	BOOL SaveSize();
	BOOL SaveFlag();


//----------------------------------------------------------------------
// Implementation
protected:
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	void OnFlagChange();
	void OnSizeChange();
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void SetColumnStatus();

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL ConvertFormulaToValue(CString& value);

protected:
	CCMSecViewWnd* m_pPreview;
	T_SECT_SECTBASE_D* m_pSectBase;
	CCMSectItemGrid4Psc* m_pOtherEnd;
	int m_nSectType;
	int m_nShapeIndex;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemGrid4Psc)
	//}}AFX_VIRTUAL
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemGrid4Psc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMGRID4PSC_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_)
