#if !defined(__DgnMergeExcelDlg_H__)
#define __DgnMergeExcelDlg_H__

#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"

using namespace std;

class CMergeGrid;
class __MY_EXT_CLASS__ CDgnMergeExcelDlg : public CDialogMove
{
public:
	CDgnMergeExcelDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnMergeExcelDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnSeisSetParamDlg)
	enum {
		IDD = IDD_SEIS_MERGE_EXCEL_DLG_JP
	};

protected:
	afx_msg void OnCancel();
	afx_msg void OnOk();
	afx_msg void CheckBoxCheck(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:
	CMergeGrid* m_pGridCtrl;
};

class CMergeGrid : public CBCGPGridCtrl
{
public:
	CMergeGrid();   // standard constructor
	~CMergeGrid();

	enum eCol
	{
		kDirection,
		kType,
		kBtn,
		kFile
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	std::map<int, std::map<int, CString>> m_mapFilePath;
protected:
	void InitRow();
	void FileOpen(const int& nRow);
	void GetMapIndexFromRow(const int& nRow, int& nSeisDesc, int& nSeisType);
};

#include "HeaderPost.h"

#endif // !defined(__DgnMergeExcelDlg_H__)