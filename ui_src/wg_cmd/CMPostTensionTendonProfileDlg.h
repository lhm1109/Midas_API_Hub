#if !defined(__CCMPostTensionTendonProfileDlg_H__)
#define __CCMPostTensionTendonProfileDlg_H__

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\ChildDialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_common\wg_common_TBBrowserWndEx.h"

#include "HeaderPre.h"

enum PT_ELEM_TYPE : int
{
	EN_ELEM_BEAM = 0,
	EN_ELEM_SLAB,
	EN_ELEM_WALL,
};

// Forward declaration
class CCMPostTensionTendonProfileDlg;
class CCMPostTensionTendonProfileGrid;

/////////////////////////////////////////////////////////////////////////////
// CCMTendonShapeImageControl - Grid 셀에 Shape 이미지를 표시하는 Custom Control
class __MY_EXT_CLASS__ CCMTendonShapeImageControl : public CGXControl
{
public:
	CCMTendonShapeImageControl(CGXGridCore* pGrid);
	virtual ~CCMTendonShapeImageControl();

	virtual void Draw(CDC* pDC, CRect rect, ROWCOL nRow, ROWCOL nCol,
	                  const CGXStyle& style, const CGXStyle* pStandardStyle);

protected:
	void InitializeBitmaps();
	CBitmap* GetBitmapForShape(const CString& strBoundary, int nShapeType);

	// First position bitmaps
	CBitmap m_bmFirstPara1, m_bmFirstPara2, m_bmFirstPara3;
	CBitmap m_bmFirstStr1, m_bmFirstStr2;
	// Middle position bitmaps
	CBitmap m_bmMiddlePara1, m_bmMiddlePara2, m_bmMiddlePara3;
	CBitmap m_bmMiddleStr1, m_bmMiddleStr2, m_bmMiddleStr3;
	// Last position bitmaps
	CBitmap m_bmLastPara1, m_bmLastPara2, m_bmLastPara3;
	CBitmap m_bmLastStr1, m_bmLastStr2;

	BOOL m_bInitialized;
};

// Grid 클래스 - CTBBrowserWndEx를 상속
class __MY_EXT_CLASS__ CCMPostTensionTendonProfileGrid : public CTBBrowserWndEx
{
public:
	CCMPostTensionTendonProfileGrid();
	virtual ~CCMPostTensionTendonProfileGrid();

	void SetParentDlg(CCMPostTensionTendonProfileDlg* pParent) { m_pParentDlg = pParent; }

	// 초기화 및 설정 함수들
	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void MakeDefaultItem();
	void MakeDefaultItem(int nElemType);
	void SetDefaultBeamItem();
	void SetDefaultSlabItem();
	void SetDefaultWallItem();
	
	// 그리드 데이터 관리
	void UpdateProfileGridData();
	void InsertProfileData(T_TDPF_D& Data);
	void ClearGrid();
	void SetRowCountByElemType(PT_ELEM_TYPE nElemType);
	void RetrieveGridData(T_TDPF_D& TdpfD);
	int ConvertShapeStringToType(const CString& strShape);
	CString GetShapeTypeLabel(int nShapeType);  // Shape 타입을 짧은 레이블로 변환

	// 컬럼 정보 관리
	void AddColumnInfo(const CString& strTitle, int nType, int nUnit = 0);
	void SetColumnStyles();
	
	// 유효성 검사
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

	void GetShapeTypeList(CString& strList);
	CString GetShapeTypeListByBoundary(const CString& strBoundary);
	CString GetProfileShape(const int& nCurElem, const CString& strBoundaryCondition, int nType);
	void UpdateColumnHeader();
	void UpdateGridDefaultItem();

protected:
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	
private:
	CCMPostTensionTendonProfileDlg* m_pParentDlg;
	CDBDoc* m_pDoc;
	
	// 컬럼 정보 저장
	CStringArray m_aColumnTitles;
	CStringArray m_aDefValues;
	CArray<int, int> m_aColumnTypes;
	CArray<int, int> m_aColumnUnits;
	
	// 기본값 설정
	void SetDefaultValues();
	CString GetDefValue(int nCol);
	
	DECLARE_MESSAGE_MAP()
};

class __MY_EXT_CLASS__ CCMPostTensionTendonProfileDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMPostTensionTendonProfileDlg(CWnd* pParent = NULL);
	virtual ~CCMPostTensionTendonProfileDlg();

	enum { IDD = IDD_CMD_PT_TENDON_PROFILE };

	// Grid 관련 함수를 public으로 이동
	void OnGridCellChanged(int nRow, int nCol);
	void OnShapeSelectionChanged(int nRow, const CString& strShape);

	int GetElementType();
	int GetElementType(CString& strElemType);
	int GetSelectedProfileKey();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	// Dialog Data
	mit::frx::MComboBox m_comboElemType;
	CListCtrl m_listProfile;
	mit::frx::MEdit m_editName;
	CStatic	m_wndBmp;
	CBitmap m_Bitmap;
	CCMPostTensionTendonProfileGrid* m_pTableCustom;  // 포인터로 변경

	CString m_strName;

	CDBDoc* m_pDoc;

	// Generated message map functions
	afx_msg void OnSelChangeElemType();
	afx_msg void OnItemClickProfileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnDelayedResize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAddBtn();
	afx_msg void OnModifyBtn();
	afx_msg void OnDeleteBtn();

	void InitializeControls();
	void InitializeProfileGridData();
	void CreateProfileGrid();
	void UpdateProfileGridData();
	void UpdateProfileList();
	void InitTendonProfileList();
	void AddTendonProfileList(const UINT& nID, const CString& strName, const int& nType);
	void Data2Dlg();
	void Dlg2Data();

	void SetProfileTypeImage();
	void GetShapeTypeList(CString& strList);

	virtual void OnClose();

private:
	T_TDPF_D m_Data;

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif