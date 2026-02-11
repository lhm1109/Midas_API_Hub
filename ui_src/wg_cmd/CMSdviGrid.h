#if !defined(__CMSDVIGRID_H__)
#define __CMSDVIGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdnaItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

class CCMSdviDlg;

/////////////////////////////////////////////////////////////////////////////
// CCMSdviGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSdviGrid : public CTBBrowserWndEx
{
// Construction
public:
	CCMSdviGrid();

// Attributes
public:
	static CString m_aCurDefVal[];

// Operations
public:
	void Initialize(CCMSdviDlg* pParent);
	void MakeItem();
	void SetData(T_SDVI_D data) { m_data=data; }
	void GetData(T_SDVI_D& data);
	void UpdateType(int nDamperType, int nDashpotType, int nInputType);
	void EnableDisableCells();
	virtual void SetHeaderTitle(BOOL bInit);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL ValidateField(CString& value, int nColID);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType);
	//virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	virtual BOOL SetRowHeight(ROWCOL nFromRow, ROWCOL nToRow, int nHeight, int* anHeightArray = NULL, UINT flags = GX_UPDATENOW, GXCmdType ctCmd = gxDo);

protected:
	BOOL GetValue(ROWCOL nRow, T_SDVI_COMP &data);
	BOOL SetValue(ROWCOL nRow, const T_SDVI_COMP &data);
	void ConvStrToData(int i, CString& value, T_SDVI_COMP& data);
	void ConvDataToStr(int i, const T_SDVI_COMP& data, CString& value);
	void SetAutoCalcValue();
	int  GetActiveDOFInt(CString value);
	CString GetActiveDOFStr(int idx);
	void GetPropHeaderTitle(int nComp, CString aTitle[5]);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdviGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSdviGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSdviGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nHeaderRow;   // m_nHeaderRow번째 줄까지 Header로 표현
	int m_nColCount;
	CCMSdviDlg* m_pParent;
	T_SDVI_D m_data;

	int	m_nDamperType;
	int	m_nDashpotType;
	int	m_nInputType;
	BOOL m_bOnInit;
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSDVIGRID_H__)
