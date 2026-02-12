#if !defined(__MPHI_SECT_DETAIL_GRID_JP_H__)
#define __MPHI_SECT_DETAIL_GRID_JP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPhiSectDetailGrid_JP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMPhiSectDetailGrid_JP window
#include "MPhiSectGrid_JP.h"

using namespace mit::frx;

class CMPhiSectGrid_JP;
/////////////////////////////////////////////////////////////////////////////
class CMPhiSectDetailGrid_JP : public CTBBrowserWnd
{

	// Construction
public:
	CMPhiSectDetailGrid_JP();
	virtual ~CMPhiSectDetailGrid_JP();

	enum eMatlCode
	{
		kMPHI_CONC_CODE_BOOK  = 0,
		kMPHI_V,
		kMPHI_III_SIG_CK,
		kMPHI_III_85_SIG_CK,
		kMPHI_V24_DEFAULT,
		kMPHI_IV_14_24_PIPE,
		kMPHI_III_SIG_CK_H29,
	};

	// Operations
public:
	void Initialize(const CArray<T_MPHG_K, T_MPHG_K>& aMphgK, const CArray<T_MPHG_D, T_MPHG_D&>& aMphgD);
	void SetAllData(CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&> &aData);
	void InitCursor(ROWCOL nRow);
	void setInitRow();

	void setInitUnit(const int& row);
	void SetData(const T_SECT_DATA_JP& rData);
	void SetRC(const T_SECT_DATA_JP& rData);
	void SetSteel(const T_SECT_DATA_JP& rData);
	void EnableMatlYear(const  BOOL& bStl);
	BOOL GetData(CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&>& aData);
	void GetRc(const T_SECT_DATA_JP& newData, T_SECT_DATA_JP& rData);
	void GetSteel(const T_SECT_DATA_JP& newData, T_SECT_DATA_JP& rData);
	BOOL GetCheckCircleSolidPipe(const T_SECT_DATA_JP& rData);
	BOOL GetCheckTrackSolidTrack(const T_SECT_DATA_JP& rData);
	void EditCheckBox(ROWCOL nRow, ROWCOL nCol, T_SECT_DATA_JP& rData);
	void EditData(ROWCOL nRow, ROWCOL nCol, T_SECT_DATA_JP& rData);
	void SetMatlYear(const T_SECT_DATA_JP& rData);

	int m_SectCurrentRow;
protected:  
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ProcessKeys(CWnd* pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	//virtual BOOL CanPaste() { return TRUE; }
	//virtual BOOL CanCut() { return FALSE; }
	//virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	//virtual BOOL CopyRange(const CGXRangeList& selList);

protected:
	CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&> m_SectData;
	CDBDoc* m_pDoc;
	CMPhiSectGrid_JP* m_pParentGird;
	BOOL m_bChange;
	int  m_MatlCode;
	CArray<T_MPHG_K, T_MPHG_K> m_aMphgK;
	CArray<T_MPHG_D, T_MPHG_D&> m_aMphgD;
	//{{AFX_MSG(CMPhiSectDetailGrid_JP)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MPHI_SECT_DETAIL_GRID_JP_H__)
