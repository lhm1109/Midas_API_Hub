#if !defined(__WIND_AMP_FACTOR_H__INCLUDE)
#define __WIND_AMP_FACTOR_H__INCLUDE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_base\DialogMove.h"
#include "CMGXGridWnd.h"


class CWindAmpFactorGrid : public CCMGXGridWnd2
{
public:
	CWindAmpFactorGrid();
	//	virtual ~CDgnConColmRebarGrid();

	// Interface
public:
	void Initialize(CWnd* pParent);
	void SetGridWndSize(int nWidth, int nHeight) { m_nWndWidth = nWidth;  m_nWndHeight = nHeight; }

	void Data2Grid(const T_WIND_KDS2021* pKDS2021);
	BOOL Grid2Data(T_WIND_KDS2021* pData);

	void UpdateUnit();

// 	void ChangeEnable();
	double GetGridValue(ROWCOL nRow, ROWCOL nCol);

	// override 
public:
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);

	BOOL PasteFromClipboard(const CGXRange &range, int nKeyCol, CGXGridWnd &x, int &nPasteMode, int &nPastedCols);
	virtual BOOL ValidateField(CString& value, int nColID);

	// Operation
protected:
	void SetSizeColumn();
	void SetDataColumn();
	void SetHeaderTitle();

private:

	// Member Var.
protected:
	int m_nWndWidth;
	int m_nWndHeight;

// 	CMasonryMaterialDlg* m_pParent;
};

class CWindAmpFactorDlg : public CDialogMove
{
public:
	CWindAmpFactorDlg(T_WIND_KDS2021* pKDS2021, CWnd* pParent = NULL);   // 표준 생성자입니다.

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ETC_WIND_KDS2021_AMPFACTOR_DLG	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void InitCtrl();
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	
protected:
	T_WIND_KDS2021* m_pKDS2021;
	CWindAmpFactorGrid m_AmpFactorGrid;

};

#endif  //!defined(__WIND_AMP_FACTOR_H__INCLUDE)