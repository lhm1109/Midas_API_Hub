#if !defined(AFX_TREEREPORTCTRL_H__)
#define AFX_TREEREPORTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeReportCtrl.h : header file
//
#include "TreeWorkDefine.h"
#include "ReportDefine.h"
#include "ReportAutoRegeneration.h"
#include "..\wg_base\wg_base_ColorTreeCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "ReportTreeText.h"
#include "..\MIT_frx\MBaseTreeCtrl.h"

#include <map>
#include <vector>
using namespace std;
class AutoReport_Gen_Base;
class CTBAMRUDTData;

/////////////////////////////////////////////////////////////////////////////////////////
// Word Dll Communicate Structure
//
/////////////////////////////////////////////////////////////////////////////////////////

#define D_DATATYPE_1ST_FLD  1
#define D_DATATYPE_2ND_FLD  2
#define D_DATATYPE_LST      3

//#define D_REPORT_TEXT_CNT   23

/////////////////////////////////////////////////////////////////////////////////////////
// CTreeReportCtrl window
//
/////////////////////////////////////////////////////////////////////////////////////////

class CTreeReportCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
// Construction
public:
		friend class AutoReport_Gen_Base;
	DECLARE_DYNAMIC(CTreeReportCtrl);
	CTreeReportCtrl();
	virtual ~CTreeReportCtrl();

// Attributes
protected:
	//////////////////////////////////////////////////////////////////////////////////////////
	// Member Data
	BOOL  m_bSetUchtRes;
	CArray<int, int>  m_aUchtRes; // workstree에 insert 가능여부에 따른 색깔 표현하기 위해.

	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Member Var.
	BOOL      m_bDrag;     // Drag 여부 판단, TRUE==드래깅 중 
	BOOL      m_bDragPoss; // Drag 가능여부 판단, TRUE==드래깅 가능
	//HTREEITEM m_hDragItem; // Drag한 tree Item의 handle 값
	CArray<HTREEITEM, HTREEITEM> m_ahDragItems; // Drag한 tree Item의 handle 값
	int       m_nDataType;  // Drag한 tree Item의 Type (1: 1st FLD, 2: 2nd FLD, 3: LST)

	HTREEITEM                     m_hRootTi;
	CArray<HTREEITEM, HTREEITEM>  m_arrSub1List;

	// CReportAutoRegeneration도 같이 수정할 것!
	CStringArray                  m_aReportTextCaption;  // Workstree에 보여질 Text List 이름
	CStringArray                  m_aReportTextStrName;  // 실제 Word에 보내질 Text List 추출정보이름
	/** Add By JUHONGHUA(MITC) 2012-2-14 16:56:35**/
	std::vector<pair<HTREEITEM,vector<CString> > > m_mapCHReportTextCaptions;
	std::vector<pair<HTREEITEM,vector<CString> > > m_mapCHReportTextStrNames;

	std::map<CString,HTREEITEM> m_mapCHReportStrName_New;
	CReportAutoRegeneration       m_AutoRegen;
	//////////////////////////////////////////////////////////////////////////////////////////

// Operations
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	// Create / Destroy
	void CreateTree();
	void DestroyTree();
	void FreeWorkTreeBuffer();
	void FreeBoundaryInfo();

	void GetReportTextCaptions(CStringArray& aReportTextCaption);
	void GetReportTextStrNames(CStringArray& aReportTextStrName);
	int FindIndexOfReportTexts(HTREEITEM htree,const std::vector<pair<HTREEITEM,vector<CString> > >& strs);
	void InsertChinaReportItemAll();
	void GetAllGenSegments(std::vector<GenSegmentElements>& segs);
	void SetInsertState(BOOL bInsert); // bInsert = true; update = false
	
protected:
	COLORREF GetBkColor();
	COLORREF GetBkColorTitle();
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Use / UnUse
	// 1: 검은색(활성), 2: 회색(비활성)
	int  IsUsedText(HTREEITEM hItem); // Text Item List의 Drag&Drop 사용 가능여부를 색으로 판단. 1:항목있음, 2:항목없음
	int  IsExistFile(HTREEITEM hItem);  // Image File or Table Template의 file이 존재하는지 판단. 1:파일존재, 2:파일존재하지 않음.
	int  IsPostTable(HTREEITEM hItem);  // 전처리 모드에서 Post Table은 회색처리. 1:전처리 테이블, 2:후처리 테이블
	int  IsEnableChart(HTREEITEM hItem);  // Chart의 Type에 따라 가능여부 판단. 1: 가능, 2: 가능하지 않음.
	int  IsEnableImage(HTREEITEM hItem);  // Image의 활성/비활성 처리. 1: 가능, 2: 가능하지 않음.

	void SetUchtRes();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Set Tree Item
	void SetItemTree(BOOL bInit=TRUE);
	HTREEITEM SetSub1TreeItem(UINT insItem,BOOL bInit,int nCount,UINT imageID);
	HTREEITEM GetPreviousItemToInsertAfter(UINT insItem);

	HTREEITEM SetDynReportSetupItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportUnitItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportRefDBItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportImageItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportTableItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportChartItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportTextItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDynReportFooterItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	//////////////////////////////////////////////////////////////////////////////////////////

	/////China Report Items///////////////////////////////////////////////////////////////
	//add by JUHONGHUA(MITC) 2012.02.14//
	HTREEITEM SetDynReportTextItemTree_CH(CDBDoc* pDoc,BOOL bInit=TRUE);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Active / Inactive Item
	void ActiveEntity(UINT data);
	void ActiveEntityPlus(UINT data);
	void InactiveEntity(UINT data);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Edit Property
	void EditProperty(HTREEITEM ht);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Delete Item
	void DeleteItemEntity(CArray<HTREEITEM, HTREEITEM> &aItem);
	BOOL DeleteItemImage(UINT data);  // Dynamic Report Image Delete All 하는 경우 (Ufig + Uimg), 1 Transaction 처리
	BOOL DeleteItemTable(UINT data);  // Dynamic Report Table Delete All 하는 경우 (Utbl + Utpl), 1 Transaction 처리
	BOOL EndEdit(BOOL bEnd);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Assign Attribute
	void InsertBySelectItems( CArray<HTREEITEM, HTREEITEM>& aItem );

	void EndDrag(CPoint point);
	void InsertReportItem();
	void InsertReportItemChart();
	void InsertReportItemImage();
	void InsertReportItemTable();
	void InsertReportItemSpecialTable();
	void InsertReportItemSSTable(); // Special Tables - Section Summary
	void InsertReportItemRSTable(); // Special Tables - Reinforcement of Sections
	void InsertReportItemCSTable(); // Special Tables - Composite Section for C.S.
	void InsertReportItemTableTemplate(); // Table Template
	void InsertReportItemText();
	void InsertReportItemSummary(); // Design Summary
	void InsertReportItemImageFile();
	void InsertReportItemTableAll();  // User Defined Table + Special Table + Table Template
	void InsertReportItemImageAll();  // User Defined Image + Image File
	void InsertReportItemTextAll();   // Defined Text + User Defined Text
	void InsertReportHeaderFooter();  // Header & Footer
	void MakeSectionFigure(CString strImagePath, UINT SectK, int nType, CStringArray& arImagePath);
	CString GetModelFullPathName();
	//////////////////////////////////////////////////////////////////////////////////////////
	HTREEITEM FindHTreeItemBySubName(const CString& strSubName);
	HTREEITEM FindHTreeItemByName(const CString& strName);
	void InsertReportItemText_BasicInfo();
	void InsertReportItemText_Model();
	void InsertReportItemText_LoadCapacity();
	void InsertReportItemText_ServiceAbility();
	void InsertReportItemText_Stress();
	void InsertReportItemText_Stress_ShortTerm();
	void InsertReportItemText_ForceMemb();
	void InsertReportItemText_Method(UINT data1,UINT data2,const CString& strItem);

	void InsertCoverPage();
	void InsertCatalogPage();
	void InsertReportItemText_DetailTable();

	
	//////////////////////////////////////////////////////////////////////////////////////////
	// Display / Disappear
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Select / Unselect
	BOOL GetSelectedItemKeyList(UINT data,CArray<T_NODE_K, T_NODE_K> &rNode,CArray<T_ELEM_K, T_ELEM_K> &rElem);
	BOOL SelectEntity(UINT data,BOOL bReplace=TRUE);
	void UnselectEntity(UINT data);

	int GetSelectedUfigNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedUtblNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedUchtNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedUimgNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedUtplNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedUsumNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// ETC
	//////////////////////////////////////////////////////////////////////////////////////////

	void SetDynReportFigureToView(HTREEITEM ht);
	CString ReplaceSymbolByTempChar(LPCTSTR lpStr);
	//void  TempleteImport();  // Table Template 처리 함수
	void SortCscsKey(CDBDoc* pDoc, CArray<T_CSCS_K, T_CSCS_K>& raCscsK);
	void ViewDynReportTable(HTREEITEM ht);  // double click시 해당 테이블 view를 띄움.
	void FilterFileName(CString& strFileName);  // 파일 이름에 (\, /, :, *, ?, ", <, >, |)를 사용할 수 없으므로, 이러한 문자가 있을시에 없애줌.

	void DeleteTempFiles(const std::vector<GenSegmentElements>& allsegs );
protected:
	COLORREF m_BkColor;
	CString m_strBkColorName;
	CString m_strBkTitleColorName;
	int m_nRPSCNum;

	CDC*       m_pMemDC;
	CRect      m_rcMemDC;
	CRect      m_rectEraseOld;    // 스크롤에 의해서 보고 있는 영역이 변경되었는지 여부 판단용

	int m_iChapterID;
	int m_iSegmentID;
	BOOL m_bInsert;
	std::vector<GenSegmentElements> m_vctAllGenSegmentElements;
	std::vector<GenSegmentElements> m_vctTableGenSegmentElements;

public:
	void SetBkGroundColor(COLORREF BkColor);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeReportCtrl)
	protected:
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	// DB Updatae
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	//////////////////////////////////////////////////////////////////////////////////////////

	// Generated message map functions
protected:
	virtual BOOL GetCustomColor(HTREEITEM hItem);
	void ToggleStyle(DWORD lStyleMask );
	void DrawBitmapNormally(CDC* pDC);
	void DrawStripeBackground(CDC* pDC, CRect& rect);
	//{{AFX_MSG(CTreeReportCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWorktreeActive();
	afx_msg void OnWorktreeDelete();
	afx_msg void OnWorktreeInactive();
	afx_msg void OnWorktreeSelect();
	afx_msg void OnWorktreeUnselect();
	afx_msg void OnWorktreeUnselectall();
	afx_msg void OnWorktreeActiveall();
	afx_msg void OnWorktreeProperties();
	afx_msg void OnWorktreeReportUnit();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnWorktreeSelectplus();
	afx_msg void OnWorktreeDisplay();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWorktreeDisappear();
	afx_msg void OnWorktreeActivePlus();
	afx_msg void OnWorktreeExpandAll();
	afx_msg void OnWorktreeExpand1LEVEL();
	afx_msg void OnWorktreeExpand2LEVEL();
	afx_msg void OnWorktreeInsert();
	
	
	afx_msg void OnWorktreeAddImageFile();
	afx_msg void OnWorktreeSaveImageFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
class CTreeReportCtrlInstance
{
public:
	~CTreeReportCtrlInstance();
	static CTreeReportCtrl* GetTreeReportCtrlInstance();
protected:
	CTreeReportCtrlInstance(){};
private:
	
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEREPORTCTRL_H__)
