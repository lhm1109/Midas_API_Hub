#if !defined(__POSTTENSIONMODIFYTENDONVERDLG_H__)
#define __POSTTENSIONMODIFYTENDONVERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_ChildDialog.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_common\wg_common_TBBrowserWndEx.h"

#include "GdiplusDrawerStatic\GdiplusDrawerStatic.h"

#include "BCGPSliderCtrl.h"

class CDBDoc;
class CPostTensionModifyTendonDlg;
class CPostTensionModifyTendonVerDlg;

#include "HeaderPre.h"

// MStatusSlider 스타일 둥근 배경을 가진 슬라이더 (다이얼로그용)
class __MY_EXT_CLASS__ CPTScaleSliderCtrl : public CBCGPSliderCtrl
{
	DECLARE_DYNAMIC(CPTScaleSliderCtrl)
public:
	CPTScaleSliderCtrl() : m_bHighlighted(FALSE) {}

protected:
	BOOL m_bHighlighted;

	virtual void OnDraw(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

// Grid 클래스 - CTBBrowserWndEx를 상속
class __MY_EXT_CLASS__ CPostTensionModifyTendonVerProfileGrid : public CTBBrowserWndEx
{
public:
	CPostTensionModifyTendonVerProfileGrid();
	virtual ~CPostTensionModifyTendonVerProfileGrid();

	void SetParentDlg(CPostTensionModifyTendonVerDlg* pParent) { m_pParentDlg = pParent; }

	// 초기화 및 설정 함수들
	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void MakeDefaultItem();
	void SetDefaultBeamItem();

	// 그리드 데이터 관리
	void UpdateProfileGridData(T_TDPF_D& TdpfD);
	void InsertProfileData(T_TDPF_D& Data);
	void ClearGrid();

	// 컬럼 정보 관리
	void AddColumnInfo(const CString& strTitle, int nType, int nUnit = 0);
	void SetColumnStyles();

	void GetShapeTypeList(CString& strList);
	CString GetProfileShape(const CString& strBoundaryCondition, int nType);

	// 시각적 읽기 전용 모드 (m_bLockEdit 플래그만 설정, 스타일 저장/복원 없음)
	void SetVisualReadOnly(BOOL bReadOnly);

protected:
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol) override;

private:
	CPostTensionModifyTendonVerDlg* m_pParentDlg;
	CDBDoc* m_pDoc;

	// 컬럼 정보 저장
	CStringArray m_aColumnTitles;
	CArray<int, int> m_aColumnTypes;
	CArray<int, int> m_aColumnUnits;

	void SetDefaultValues();

	DECLARE_MESSAGE_MAP()
};

class __MY_EXT_CLASS__ CPostTensionModifyTendonVerDlg : public CChildDialog
{
// Construction
public:
	CPostTensionModifyTendonVerDlg(CDBDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CPostTensionModifyTendonVerDlg();

// Dialog Data
	enum { IDD = IDD_CMD_PT_MODIFY_TENDON_VER };

// Attributes
protected:
	CDBDoc* m_pDoc;
	CPostTensionModifyTendonDlg* m_pParent;

	gdiplus_draw::CGdiplusDrawerStatic m_wndPic;
	CStatic m_wndProfileTable;
	mit::frx::MComboBox m_cmbProfile;
	mit::frx::MButton m_chkUser;
	BOOL m_bUser;

	CPTScaleSliderCtrl m_wndVertScaleSlider;   // 수직 스케일 슬라이더
	double m_dVertScale;                 // 수직 스케일 배율 (1.0 = 기본)

	CPostTensionModifyTendonVerProfileGrid* m_pProfileGrid;

	T_PTTN_K m_PttnKey;           // 현재 선택된 PTTN Key
	T_PTTN_D m_OriginalPttnData;  // 원본 텐던 데이터 (변경 감지용)
	BOOL m_bUpdatingGrid;         // Grid 업데이트 중 플래그 (OnGridDataChanged 무시용)

// Operations
public:
	void UpdateTendonProfileList();
	void UpdateProfileGridData();
	void DrawProfile();
	void OnProfileDataChanged();
	void SetPttnKey(T_PTTN_K key);    // 부모에서 PTTN Key 설정 시 호출
	void OnGridDataChanged();         // Grid 데이터 변경 시 호출
	void UpdateUserDefinedState();    // ComboBox 활성화/비활성화 업데이트
	void GetPttnData(T_PTTN_D& Data); // 현재 탭 데이터를 T_PTTN_D에 설정
	BOOL IsGridDataChanged();                    // 원본과 Grid 데이터 비교

protected:
	void CreateProfileGrid();
	void InitializeProfileGrid();
	void GetCurrentGridData(T_PTTN_D& Data);     // Grid에서 현재 데이터 추출
	void DrawDefaultGuide();                     // 텐던 미선택 시 기본 안내 표시
	void DrawNoDataMessage();                    // 프로필 데이터 없음 메시지 표시

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Generated message map functions
protected:
	afx_msg void OnBnClickedUserChk();  // User Defined 체크박스 클릭 핸들러
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif // !defined(__POSTTENSIONMODIFYTENDONVERDLG_H__)
