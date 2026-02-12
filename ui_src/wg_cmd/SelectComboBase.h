#if !defined(AFX_SELECTCOMBOBASE_H__8BADD462_5F94_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SELECTCOMBOBASE_H__8BADD462_5F94_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectComboBase.h : header file
//

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSelectComboNode window

class CSelectComboNode : public CComboBox
{
// Construction
public:
	CSelectComboNode();
	virtual ~CSelectComboNode();
	friend class CDBDoc;

// Static Members
protected:
	static CString m_sText;
	static CArray<CSelectComboNode*, CSelectComboNode*> m_aInstances; // 생성된 인스턴스 배열
	static void Initialize();       // Document가 생성될 경우 호출
	static void UpdateSelection();  // Selection이 변경될 경우 호출
	static CDBDoc* GetDocument();
	static CString GetOptimizedString();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectComboNode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateSelectionText();
	void OnEnterKeyPressed();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSelectComboNode)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCOMBOBASE_H__8BADD462_5F94_11D3_92DE_0000C0B0E6B3__INCLUDED_)
