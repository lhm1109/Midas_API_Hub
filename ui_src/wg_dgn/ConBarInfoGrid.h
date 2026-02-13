#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_cmd\CMGXGridWnd.h"

class CDgnConBarInfoDlg;
class CMatlDB;
class CConBarInfoGrid : public CCMGXGridWnd
{
public:
	CConBarInfoGrid(CMatlDB* pMatlDB);
	virtual ~CConBarInfoGrid();

public:
	void Initialize(CWnd* pParent);
	void SetGridWndSize(int nWidth, int nHeight) { m_nWndWidth = nWidth;  m_nWndHeight = nHeight; }

	void Data2Grid(const CStringArray& rebarNameList, const CString& strTotalRebarSize);
	void Grid2Data(CString& strTotalRebarSize);

	void UpdateUnit();

	//operation
protected:
	void SetHeaderTitle();
	void SetColumnWidth();
	void SetReadOnly(BOOL bSetReadOnly);

	void SetValue(ROWCOL row, const CString& strBarName, const CArray<CString, CString>& arBarSize);
	void ConvDataToStr(int i, const CString& strBarName, const CArray<CString, CString>& arBarSize, CString& value);
	BOOL SelectionCheck(const CString& strBarName, const CArray<CString, CString>& arBarSize);

	BOOL Cutting_RebarSizeString(CString str, CArray<CString, CString>& BarSize);

protected:
	int m_nWndWidth;
	int m_nWndHeight;

	CDgnConBarInfoDlg* m_pParent;
	CMatlDB* m_pMatlDB;
};