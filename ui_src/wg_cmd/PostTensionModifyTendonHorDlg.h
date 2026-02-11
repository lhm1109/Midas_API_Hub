#if !defined(__POSTTENSIONMODIFYTENDONHORDLG_H__)
#define __POSTTENSIONMODIFYTENDONHORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBBrowserWndEx.h"
#include "GdiplusDrawerStatic\GdiplusDrawerStatic.h"

class CDBDoc;
class CPostTensionModifyTendonDlg;
class CPostTensionModifyTendonHorDlg;

#include "HeaderPre.h"

// Grid 클래스 - CTBBrowserWndEx를 상속
class __MY_EXT_CLASS__ CPostTensionModifyTendonHorProfileGrid : public CTBBrowserWndEx
{
public:
	CPostTensionModifyTendonHorProfileGrid();
	virtual ~CPostTensionModifyTendonHorProfileGrid();

	void SetParentDlg(CPostTensionModifyTendonHorDlg* pParent) { m_pParentDlg = pParent; }

	void Initialize(CDBDoc* pDoc);
	void SetHeaderTitle();
	void SetColumnStyles();
	void ClearGrid();

protected:
	// Grid 값 변경 감지를 위한 오버라이드
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol) override;

private:
	CPostTensionModifyTendonHorDlg* m_pParentDlg;
	CDBDoc* m_pDoc;

	DECLARE_MESSAGE_MAP()
};

class __MY_EXT_CLASS__ CPostTensionModifyTendonHorDlg : public CChildDialog
{
// Construction
public:
	CPostTensionModifyTendonHorDlg(CDBDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CPostTensionModifyTendonHorDlg();

// Dialog Data
	enum { IDD = IDD_CMD_PT_MODIFY_TENDON_HOR };

// Attributes
protected:
	CDBDoc* m_pDoc;
	CPostTensionModifyTendonDlg* m_pParent;

	gdiplus_draw::CGdiplusDrawerStatic m_wndPic;
	CStatic m_wndProfileTable;

	CPostTensionModifyTendonHorProfileGrid* m_pProfileGrid;

	T_PTTN_K m_PttnKey;           // 현재 선택된 PTTN Key
	T_PTTN_D m_OriginalPttnData;  // 원본 텐던 데이터

// Operations
public:
	void SetPttnKey(T_PTTN_K key);    // 부모에서 PTTN Key 설정 시 호출
	void UpdateGridData();            // Grid 데이터 업데이트
	void OnProfileDataChanged();      // Grid 값 변경 시 호출
	void DrawProfile();               // 프로파일 그리기

protected:
	void CreateProfileGrid();
	void InitializeProfileGrid();
	void DrawDefaultGuide();          // 텐던 미선택 시 기본 안내
	void DrawNoDataMessage();         // 데이터 없음 메시지

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

#endif // !defined(__POSTTENSIONMODIFYTENDONHORDLG_H__)
