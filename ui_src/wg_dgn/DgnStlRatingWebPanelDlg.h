#if !defined(AFX_DgnStlRatingWebPanelDlg_H__)
#define AFX_DgnStlRatingWebPanelDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingWebPanelDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingWebPanelDlg dialog

class CDgnStlRatingWebPanelDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{

public:
	CDgnStlRatingWebPanelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
	enum { IDD = IDD_DGN_STL_RATING_WEB_PANEL_DLG };
	
	int		m_nOption;
	int		m_nAssignType;
	int		m_nSelectType;
	CListCtrl	m_List;
	mit::frx::MButton  m_wndAllowSingleElemMember;
	int     m_nElemType;
	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	CDBDoc* m_pDoc;
	virtual ~CDgnStlRatingWebPanelDlg();
	void CtrlManager();
	void InitSelectedItem();
	void MemberDefine();
	void MemberDefineManual();  // 선택된 요소로부터 Member를 생성하는 함수(Manual의 Add/Replace 일때 사용)
	void MemberDefineAuto();    // 선택된 요소로부터 Member를 생성하는 함수(Automatic의 Add/Replace 일때 사용)  
	void DelAssignedMember();   // 선택된 요소로부터 이들이 속한 Member를 지우는 함수(Manual, Automatic의 Delete일때 사용)
//  BOOL GetFinalSurvivedMember(CArray<T_WEBP_D, T_WEBP_D&>& aDataMemb);
//  BOOL GetSuperiorMember(CArray<T_WEBP_D, T_WEBP_D&>& aDataMemb);
//  BOOL AddSingleElemMember(CArray<T_WEBP_D, T_WEBP_D&>& aDataMemb);
	BOOL DelInferiorMember();
	BOOL AddSingleElemMember();
	void AlignElemListInOrder(T_WEBP_D& rData);  // Member의 요소를 i, j순서대로 다시 배열하는 함수
	void GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz);

	//  BOOL IsContinuousMember(T_WEBP_D& rData);
	BOOL IsExistColumnWallLower(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem);
	BOOL IsExistColumnWallUpper(T_NODE_K KeyNode);
	BOOL IsStoryLevel(T_NODE_K KeyNode);
	BOOL IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew);
	BOOL IsSupportAndColumnWallUpperExist(T_ELEM_K KeyElem, T_NODE_K KeyNode);
	BOOL IsSupportExist(T_ELEM_K KeyElem, T_NODE_K KeyNode);
	
	void InitCtrl();
	void AlignControl();
	void ShowHideControls();

	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnAssignType();
	afx_msg void OnDgnOption();
	afx_msg void OnDgnMemberSelect();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnElemTypeRdo();

private:
	void ExecuteForVBeam(CArray<T_ELEM_K, T_ELEM_K>& aSelElemKey);
	void ExecuteForElement(CArray<T_ELEM_K, T_ELEM_K>& aSelElemKey);
	void ResetListByElemType();
	void MakeItemExForVbeam();
	void InsertItemForVbeam(T_WEBV_K Key);
	CString GetStrElemList(const CArray<T_ELEM_K, T_ELEM_K>& aElemList);

protected:
	CArray<T_ELEM_K, T_ELEM_K> m_aSelFrmKey;   // 선택된 element 중에서 frame type의 element list
	CArray<T_ELEM_K, T_ELEM_K> m_aNonFrmKey;   // 선택된 element 중에서 frame type 이외의 element list
	CArray<T_WEBP_D, T_WEBP_D&> m_aDataMemb;   // IsContinuousMember() 함수에서 사용할 임시적으로 설정된 멤버
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> m_mapSelFrmKey;  // 선택된 element 중에서 frame type의 element list
	CArray<UINT, UINT> m_aCtrlAll;
	NM_LISTVIEW m_TempListView;
	BOOL        m_bSentMessage;

protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	void OnDelKeyPressedInList();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void DisplaySelElemList();
	void GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void InsertItem(T_WEBP_K Key);
	void DeleteItem(T_WEBP_K Key);
	void SetListFocus(CArray<int, int>& aSelItem);
	void SetText();
	void Initial_SelectItem();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DgnStlRatingWebPanelDlg_H__)
