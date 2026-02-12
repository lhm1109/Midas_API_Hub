#if !defined(__MPHI_SECT_GRID_JP_H__)
#define __MPHI_SECT_GRID_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPhiSectGrid_JP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMPhiSectGrid_JP window
#include <set>
#include "..\wg_common\wg_common_TBCommon.h"

/////////////////////////////////////////////////////////////////////////////

struct T_SECT_DATA_JP
{
	T_SECT_K SectK;
	CString strName;
	CString	strShape;
	BOOL		bRebar;
	double	dRFY;
	double	dRFZ;
	double	dRRY;
	double	dRRZ;
	BOOL		bUse;  
	BOOL		bSatisfy;
	int     nMembType; // 0:None, 1:Column, 2:Beam, 3:Brace

	T_MPST_D MpstD; 

	// check data...
	BOOL		bRC;
	BOOL		bStl;
	BOOL		bSRC;
	BOOL		bUseMiddle;	
	BOOL		bCandidate;

	// section type...
	int			nStype;

	void Initialize()
	{
		SectK				= 0;
		strName		= "";
		strShape	= "";
		bRebar		= FALSE;
		MpstD.Initialize();
		dRFY = 0.0;
		dRFZ = 0.0;
		dRRY = 0.0;
		dRRZ = 0.0;
		bUse			= FALSE;
		bSatisfy	= FALSE;
		nMembType = 0;
		bRC				= FALSE;
		bStl			= FALSE;
		bSRC			= FALSE;
		bUseMiddle = FALSE;
		bCandidate = FALSE;
		nStype		= 0;
	}
	T_SECT_DATA_JP() { Initialize(); };
	T_SECT_DATA_JP(T_SECT_DATA_JP& src) { *this = src; };
	T_SECT_DATA_JP& operator=(const T_SECT_DATA_JP& rData)
	{
		SectK				= rData.SectK;
		strName		= rData.strName;
		strShape	= rData.strShape;
		bRebar		= rData.bRebar;
		MpstD			= rData.MpstD;
		dRFY = rData.dRFY;
		dRFZ = rData.dRFZ;
		dRRY = rData.dRRY;
		dRRZ = rData.dRRZ;
		bUse			= rData.bUse;
		bSatisfy	= rData.bSatisfy;
		nMembType = rData.nMembType;
		bRC				= rData.bRC;
		bStl			= rData.bStl;
		bSRC			= rData.bSRC;
		bUseMiddle = rData.bUseMiddle;
		bCandidate = rData.bCandidate;		

		nStype		= rData.nStype;
		return *this;
	}
};

class CMPhiSectDetailGrid_JP;

class CMPhiSectGrid_JP : public CTBBrowserWnd
{

	// Construction
public:
	CMPhiSectGrid_JP(CMPhiSectDetailGrid_JP* pGrid);
	virtual ~CMPhiSectGrid_JP();

	// Operations
public:
	void Initialize(BOOL bCodeExist[6]);  
	void SetData(CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&> &aData);

	BOOL GetData(CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&> &aData);
	 
	
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);


public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL

protected:  
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return TRUE; }
	virtual BOOL CanCut() { return FALSE; }
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL CopyRange(const CGXRangeList& selList);
	virtual BOOL SetCurrentCell(ROWCOL nRow, ROWCOL nCol, UINT flags = GX_SCROLLINVIEW | GX_UPDATENOW);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
//	void GetValue(CStringArray& aValue, T_SECT_DATA_JP& data);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	BOOL IsPasteToAllRange(const CGXRange &range, CString szData); // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함;
	CString GetPosString(const T_SECT_DATA_JP& data);
	int GetPosVal(CString strPos);
	void SetShapeBitmap(ROWCOL nRow, T_SECT_DATA_JP& data);
	void SetSectionDataEdit(ROWCOL nRow, T_SECT_DATA_JP& data);

	std::set<std::pair<ROWCOL, ROWCOL>> m_setRedFont;
	BOOL m_bCodeExist[6];
	CMPhiSectDetailGrid_JP* m_pDetailGrid;
	std::vector<int> m_SelectedRow;
	// Generated message map functions

	//{{AFX_MSG(CMPhiSectGrid_JP)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_SECT_GRID_JP_H__)
