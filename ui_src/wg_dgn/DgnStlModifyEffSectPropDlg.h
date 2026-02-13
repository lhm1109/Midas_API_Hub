#if !defined(__CDgnStlModifyEffSectPropDlg_H__)
#define __CDgnStlModifyEffSectPropDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"      

class CDBDoc;
//////////////////////////////////////////////////////////////////////////
class CDgnStlModifyEffSectPropULSGrid;  
class CDgnStlModifyEffSectPropSLSGrid;  


class __MY_EXT_CLASS__ CDgnStlModifyEffSectPropDlg : public CDialogMove
{
	// Construction
public:
	CDgnStlModifyEffSectPropDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDgnStlModifyEffSectPropDlg)
	enum { IDD = IDD_DGN_STL_MODIFY_EFFSECTPROP_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA
	
protected:
	void InitGridData();
	void Initial_ListCtrl();
	void Initial_Data();

	CString DataToStr(int i, T_SECT_K Key, T_SECT_D &Data);
	void SetData();
	BOOL Dlg2Data();
	BOOL DataCheck(T_SECT_STIFFNESS_CF_BASE& CFStiffULS);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlModifyEffSectPropDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;
	T_SECT_K m_Key;
	T_SECT_D m_Data;

	CDgnStlModifyEffSectPropULSGrid* m_pSectULSGrid;
	//CDgnStlModifyEffSectPropSLSGrid* m_pSectSLSGrid;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlModifyEffSectPropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModify();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

#endif // !defined(__CDgnStlModifyEffSectPropDlg_H__)
