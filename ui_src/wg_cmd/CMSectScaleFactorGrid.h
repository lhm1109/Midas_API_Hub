#if !defined(__CMSECTSCALEFACTORGRID_H__)
#define __CMSECTSCALEFACTORGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdnaItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"

class CCMSectScaleFactorDlgNew;

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSectScaleFactorGrid : public CTBCommon
{
// Construction
public:
	CCMSectScaleFactorGrid();

// Attributes
public:
	static CString m_aCurDefVal[];

// Operations
public:
	void Initialize(CCMSectScaleFactorDlgNew* pParent);
	BOOL DeleteRecordByKey(const T_SECF_K &key, const T_SECF_D &data);
	BOOL InsertRecordByKey(const T_SECF_K &key, const T_SECF_D &data);

	virtual int GetKeyColID() { return 0; }
	virtual void OnChangedSelectionPublic(long *keys, int nSize);
	virtual long GetSelectedKeys(CArray<long, long> &caKey);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);

	BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, T_SECF_K &key, T_SECF_D &data);
	BOOL ValidateField(CString value, int nColID);
	BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols);

	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData);
	virtual BOOL DeleteFromDB(ROWCOL nRow);
	virtual BOOL DeleteFromDB(CRowColArray& awRows);
	virtual BOOL ModifyToDB(ROWCOL nRow);
	virtual BOOL ModifyToDB(CStringArray& raOldKey, 
									CArray<void*, void*>& raKey, CArray<void*, void*>& raData);

	virtual CString GetNewKey();
	virtual CString GetDefValue(int nIndex);
	virtual CString* GetDefValueArray();
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData);
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData);

	virtual long GetCountData();
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols);
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols);
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord);

	virtual void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
//  virtual void OnChangeCurrentRecord(ROWCOL nRow);

	virtual CString GetKeyString(ROWCOL nRow);            // 2개 이상의 column이 key를 형성할 경우 override
	virtual CString GetKeyStringBeforeEdit(ROWCOL nRow);  // 2개 이상의 column이 key를 형성할 경우 override

	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

public:
//  BOOL      CheckCurrentRecordChanged();
	BOOL      GetCurrentSecf(T_SECF_K& Key);
//  BOOL      SetCurrentSecf(T_SECF_K key, T_SECF_D& data);

	void      SetBngrColumn(ROWCOL nCol);
	void      SetCompositeColumn(ROWCOL nCol);
	T_BNGR_K  GetBngrKey(CString& BngrName);
	CString   GetBngrName(T_BNGR_K BngrKey);
	CString   GetComposite(BOOL bBefore, BOOL bAfter);

protected:
	BOOL GetValue(ROWCOL nRow, T_SECF_K &key, T_SECF_D &data);
	BOOL SetValue(ROWCOL nRow, const T_SECF_K &key, const T_SECF_D &data);
	BOOL SetValue(CGXData* pData, ROWCOL nRow, const T_SECF_K &key, const T_SECF_D &data, CRowColArray &awCols);
	void ConvStrToData(int i, CString& value, T_SECF_K& key, T_SECF_D& data);
	void ConvDataToStr(int i, const T_SECF_K& key, const T_SECF_D& data, CString& value);

	void MakeSearchKey(const T_SECF_K &key, const T_SECF_D &data, CStringArray &aKey);
	void GetAllSelectedSecf(CArray<T_SECF_K, T_SECF_K>& aSelKey);

	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void ChangeSectionID(ROWCOL nRow, int nID);

	void OnOffBeforeAfter(ROWCOL nRow); // added by mylee
	void CoverByIJ(ROWCOL nRow);        // added by mylee

	BOOL IsCmpTapBoxITub(T_SECT_D& Sect) { return (Sect.nStype==D_SECT_TYPE_TAPERED && (Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_B || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_I || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_TUB || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_B || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_I || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_TUB));}
	BOOL IsCmpTapCICT(T_SECT_D& Sect) { return (Sect.nStype==D_SECT_TYPE_TAPERED && (Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_CI || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_CT));}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectScaleFactorGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSectScaleFactorGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectScaleFactorGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CArray<T_SECF_K, T_SECF_K> m_aSelectedKeys;
	ROWCOL m_nCurrentRow;
	CCMSectScaleFactorDlgNew* m_pParent;
public:
// 탭 바뀔시 호출하여 OnChangeCurrentRecord(m_nCurrentRow) 호출 -> LBUTTONDOWN 에서 처리하는 것으로 수정
//  void OnChangeCurrentRecord(); 

};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTSCALEFACTORGRID_H__)
