// #if !defined(__CMSTAGECOMPSECTGRID_H__)
// #define __CMSTAGECOMPSECTGRID_H__
// 
// #if _MSC_VER > 1000
// #pragma once
// #endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectGrid
class CCMStageCompSectItemDlg;
class CCMStageCompSectGrid : public CGXGridWnd
{
public:
	CCMStageCompSectGrid(CWnd* pParent);
	virtual ~CCMStageCompSectGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);  
	virtual void OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

public:
	void Initialize(CDBDoc* pDoc);
	BOOL SetRowNumberOnly(UINT nRowNumber);
	BOOL SetRowNumber(UINT nRowNumber, int nSubType=-1, BOOL nSectType=0, BOOL bTapered=FALSE);	// nSubType: (-1)Not Setting (0)1+2, 3 (1)1, 2+3 (2)User Type
	void SetCompoStageColumn(UINT nStagK);
		
	void SetData(T_CSCS_BASE& rData);
	BOOL GetData(ROWCOL nRow, T_CSCS_BASE& rData);
	BOOL GetAllData(CArray<T_CSCS_BASE, T_CSCS_BASE&>& arData);
	BOOL ReCalcStiff(int nPart);

	void SetCompositType(int nType) { m_nCompositType = nType; }
	void SetInitH();
	void SetInitCentroidForGeneral(T_SECT_D& data);
	void SetInitScaleFactorForGeneral(T_SECT_D& data);
	void SetInitStiffnessForGeneral(T_SECT_D& data);

protected:
	void SetInitRows(ROWCOL nFromRow, ROWCOL nToRow);
	void SetStyleByType();
	void SetInitData();
	void SetHeaderTitle();
	void SetDefaultData(ROWCOL nRow);
	void EnableDisableColumns(ROWCOL nRow, int nMatlType);
	void ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol);
	void SetAutoCalcH(ROWCOL nRow, ROWCOL nCol);
	ROWCOL GetRowNember(int nPart);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

public:
	static CString m_aCurDefVal[12];

protected:
	CDBDoc* m_pDoc;

	ROWCOL  m_nCurRowNumber;
	ROWCOL  m_nPreRowNumber;

	int     m_nSubType; // 2:UserType, -1:Compo_G
	BOOL    m_bTapered;
	int     m_nSectType; // 0: None 1:SRC 2 Dumbbell;

	int     m_nCompositType; // -2:Normal, -1:User, 0:Type1, 1:Type2

	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS> m_aScale;
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS> m_aStiffAuto_I;
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS> m_aStiffAuto_J;
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS> m_aStiff_I;
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS> m_aStiff_J;

private:
	CCMStageCompSectItemDlg*  m_pParent;
};

// #endif // !defined(__CMSTAGECOMPSECTGRID_H__)
