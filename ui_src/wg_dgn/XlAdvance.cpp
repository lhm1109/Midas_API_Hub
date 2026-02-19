// XlAdvance.cpp: implementation of the CXlBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_dgn.h"
#include "XlAdvance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///////////////////////////////// Class CellBase ////////////////////////////////////
CXl_ACellBase::CXl_ACellBase()
{
	m_bSetNumberFormat = FALSE;
	m_NumberFormatLocal = _T("0.000_ ");
	m_bSetVAlignment = FALSE;
	m_iVAlignment = 2;
	m_bSetHAlignment = FALSE;
	m_iHAlignment = 1;
	m_bSetCellColor = FALSE;
	m_iCellColorIndex = 1;
	m_bSetFont = FALSE;
	m_iFontHeight=9; m_lFontColor=0; m_sFontType=_T(""); m_bFontBold = FALSE;

	m_arCellLine.RemoveAll();

	m_bSetBorder = FALSE;
	m_iBorderLineStyle = 1; m_iBorderThick = 2;

	m_bSetCellOutLine = FALSE;
	m_iCellOutLineStyle=1; m_iCellOutLineThick=2; m_iCellOutLineColor=1;

}

CXl_ACellBase::~CXl_ACellBase()
{

}

CXl_ACellBase::CXl_ACellBase(CXl_ACellBase& src)
{
	*this = src;
}

CXl_ACellBase& CXl_ACellBase::operator=(CXl_ACellBase& src)
{
	m_bSetNumberFormat=src.m_bSetNumberFormat;
	m_NumberFormatLocal=src.m_NumberFormatLocal;
	m_bSetVAlignment=src.m_bSetVAlignment;
	m_iVAlignment=src.m_iVAlignment;
	m_bSetHAlignment=src.m_bSetHAlignment;
	m_iHAlignment=src.m_iHAlignment;
	m_bSetCellColor=src.m_bSetCellColor;
	m_iCellColorIndex=src.m_iCellColorIndex;
	m_bSetFont=src.m_bSetFont;
	m_iFontHeight=src.m_iFontHeight;
	m_lFontColor=src.m_lFontColor;
	m_sFontType=src.m_sFontType;
	m_bFontBold=src.m_bFontBold;

	m_bSetBorder=src.m_bSetBorder;
	m_iBorderLineStyle=src.m_iBorderLineStyle;
	m_iBorderThick=src.m_iBorderThick;
	m_bSetCellOutLine=src.m_bSetCellOutLine;
	m_iCellOutLineStyle=src.m_iCellOutLineStyle;
	m_iCellOutLineThick=src.m_iCellOutLineThick;
	m_iCellOutLineColor=src.m_iCellOutLineColor;

	m_arCellLine.RemoveAll();
	for(int i=0; i<src.m_arCellLine.GetSize(); i++)
		m_arCellLine.Add(src.m_arCellLine[i]);
	return *this;
}

///////////////////////////////// Class Cell ////////////////////////////////////

CXl_ACell::CXl_ACell()
{
	m_iRow = 0;
	m_iCol = 0;
	m_iValueType = -1;
	m_lValue=0;
	m_dValue=0.0;
	m_sValue=0;
	m_strValue = _T("");
}

CXl_ACell::~CXl_ACell()
{
}

CXl_ACell::CXl_ACell(CXl_ACell& src)
{
	*this = src;
}

CXl_ACell& CXl_ACell::operator=(CXl_ACell& src)
{
	(CXl_ACellBase&)(*this) = (CXl_ACellBase&)src;
	m_iRow=src.m_iRow;
	m_iCol=src.m_iCol;
	m_dValue=src.m_dValue;
	m_lValue=src.m_lValue;
	m_sValue=src.m_sValue;
	m_strValue=src.m_strValue;
	m_iValueType=src.m_iValueType;
	
	return *this;
}

//0:String, 1:double, 2:long, 3:short
void CXl_ACell::SetValue(double dValue)
{
	m_dValue = dValue;
	m_iValueType = 1;
}

void CXl_ACell::SetValue(long lValue)
{
	m_lValue = lValue;
	m_iValueType = 2;
}

void CXl_ACell::SetValue(short sValue)
{
	m_sValue = sValue;
	m_iValueType = 3;
}

void CXl_ACell::SetValue(CString strValue)
{
	m_strValue = strValue;
	m_iValueType = 0;
}

BOOL CXl_ACell::WriteCell(CXlBase* pXl)
{
	if(m_bSetNumberFormat)	
		pXl->SetNumberFormat(m_iRow,m_iCol,m_iRow,m_iCol, m_NumberFormatLocal);
	if(m_bSetCellColor)
		pXl->SetBackColor(m_iRow,m_iCol,m_iRow,m_iCol,m_iCellColorIndex);
	if(m_bSetVAlignment)
		pXl->SetVerAlign(m_iRow,m_iCol,m_iRow,m_iCol, m_iVAlignment);
	if(m_bSetHAlignment)
		pXl->SetHorAlign(m_iRow,m_iCol,m_iRow,m_iCol, m_iHAlignment);
	if(m_bSetFont)
	{
		if(m_sFontType!=_T(""))
			pXl->SetFont(m_iRow,m_iCol,m_iRow,m_iCol,m_iFontHeight,m_lFontColor,m_sFontType,m_bFontBold);
		else
			pXl->SetFont(m_iRow,m_iCol,m_iRow,m_iCol,m_iFontHeight,m_lFontColor,m_bFontBold);
	}
	if(m_bSetBorder)
		pXl->SetBorder(m_iRow,m_iCol,m_iRow,m_iCol, m_iBorderLineStyle, m_iBorderThick);

	for(int iCellLine=0; iCellLine<m_arCellLine.GetSize(); iCellLine++)
	{
		T_CellLine CellLine; CellLine.Init();
		CellLine = m_arCellLine.GetAt(iCellLine);
		pXl->CellLine(m_iRow,m_iCol,m_iRow,m_iCol, CellLine.iDirection, CellLine.iCellLineStyle, CellLine.iCellLineThick, CellLine.iCellLineColor);
	}

	switch(m_iValueType)
	{
		case 0:
			pXl->SetValue(m_iRow, m_iCol, m_strValue);
			break;
		case 1:
			pXl->SetValue(m_iRow, m_iCol, m_dValue);
			break;
		case 2:
			pXl->SetValue(m_iRow, m_iCol, m_lValue);
			break;
		case 3:
			pXl->SetValue(m_iRow, m_iCol, m_sValue);
			break;
		default:
			ASSERT(0);
			break;
	}

	if(m_bSetCellOutLine)
		pXl->CellOutLine(m_iRow,m_iCol,m_iRow,m_iCol,m_iCellOutLineStyle, m_iCellOutLineThick, m_iCellOutLineColor);

	return TRUE;
}

///////////////////////////////// Class Cells ////////////////////////////////////
CXl_ACells::CXl_ACells()
{
	m_iValueType = -1;
	m_lValue=0;
	m_dValue=0.0;
	m_sValue=0;
	m_strValue = _T("");
	m_bMerge = FALSE;
	m_iSRow = m_iERow = 0;
	m_iSCol = m_iECol = 0;
	m_arCell.RemoveAll();
}

CXl_ACells::~CXl_ACells()
{

}

CXl_ACells::CXl_ACells(CXl_ACells& src)
{
	*this = src;
}

CXl_ACells& CXl_ACells::operator=(CXl_ACells& src)
{
	(CXl_ACellBase&)(*this) = (CXl_ACellBase&)src;
	m_iSRow=src.m_iSRow;
	m_iSCol=src.m_iSCol;
	m_iERow=src.m_iERow;
	m_iECol=src.m_iECol;

	m_dValue=src.m_dValue;
	m_lValue=src.m_lValue;
	m_sValue=src.m_sValue;
	m_strValue=src.m_strValue;
	m_iValueType=src.m_iValueType;

	m_bMerge=src.m_bMerge;
	m_arCell.RemoveAll();
	for(int i=0; i<src.m_arCell.GetSize(); i++)
		m_arCell.Add(src.m_arCell[i]);
	return *this;
}

//0:String, 1:double, 2:long, 3:short
void CXl_ACells::SetValue(double dValue)
{
	m_dValue = dValue;
	m_iValueType = 1;
}

void CXl_ACells::SetValue(long lValue)
{
	m_lValue = lValue;
	m_iValueType = 2;
}

void CXl_ACells::SetValue(short sValue)
{
	m_sValue = sValue;
	m_iValueType = 3;
}

void CXl_ACells::SetValue(CString strValue)
{
	m_strValue = strValue;
	m_iValueType = 0;
}

BOOL CXl_ACells::WriteCells(CXlBase* pXl)
{
	if(m_bSetNumberFormat)	
		pXl->SetNumberFormat(m_iSRow, m_iSCol, m_iERow, m_iECol, m_NumberFormatLocal);
	if(m_bSetCellColor)
		pXl->SetBackColor(m_iSRow,m_iSCol,m_iERow,m_iECol,m_iCellColorIndex);
	if(m_bSetVAlignment)
		pXl->SetVerAlign(m_iSRow,m_iSCol,m_iERow,m_iECol, m_iVAlignment);
	if(m_bSetHAlignment)
		pXl->SetHorAlign(m_iSRow,m_iSCol,m_iERow,m_iECol, m_iHAlignment);
	if(m_bSetFont)
	{
		if(m_sFontType!=_T(""))
			pXl->SetFont(m_iSRow,m_iSCol,m_iERow,m_iECol,m_iFontHeight,m_lFontColor,m_sFontType,m_bFontBold);
		else
			pXl->SetFont(m_iSRow,m_iSCol,m_iERow,m_iECol,m_iFontHeight,m_lFontColor,m_bFontBold);
	}
	if(m_bSetBorder)
		pXl->SetBorder(m_iSRow,m_iSCol,m_iERow,m_iECol, m_iBorderLineStyle, m_iBorderThick);

	for(int iCellLine=0; iCellLine<m_arCellLine.GetSize(); iCellLine++)
	{
		T_CellLine CellLine; CellLine.Init();
		CellLine = m_arCellLine.GetAt(iCellLine);
		pXl->CellLine(m_iSRow,m_iSCol,m_iERow,m_iECol, CellLine.iDirection, CellLine.iCellLineStyle, CellLine.iCellLineThick, CellLine.iCellLineColor);
	}

	for(int i=0; i<m_arCell.GetSize(); i++)
	{
		m_arCell[i].WriteCell(pXl);
	}

	if(m_iValueType >= 0)
	{
		for(int i=m_iSRow; i<=m_iERow; i++)
		{
			for(int j=m_iSCol; j<=m_iECol; j++)
			{
				switch(m_iValueType)
				{
					case 0:
						pXl->SetValue(i, j, m_strValue);
						break;
					case 1:
						pXl->SetValue(i, j, m_dValue);
						break;
					case 2:
						pXl->SetValue(i, j, m_lValue);
						break;
					case 3:
						pXl->SetValue(i, j, m_sValue);
						break;
					default:
						ASSERT(0);
						break;
				}
			}
		}
	}

	if(m_bMerge)
	{
		pXl->SetMergeCell(m_iSRow, m_iSCol, m_iERow, m_iECol);
	}

	if(m_bSetCellOutLine)
		pXl->CellOutLine(m_iSRow,m_iSCol,m_iERow,m_iECol,m_iCellOutLineStyle, m_iCellOutLineThick, m_iCellOutLineColor);

	return TRUE;
}


///////////////////////////////// Class Table ////////////////////////////////////

CXl_ATable::CXl_ATable()
{
	m_iTableSRow = m_iTableERow = 0;
	m_iTableSCol = m_iTableECol = 0;
	m_arHeader.RemoveAll();
	m_arData.RemoveAll();
}

CXl_ATable::~CXl_ATable()
{

}

CXl_ATable::CXl_ATable(CXl_ATable& src)
{
	*this = src;
}

CXl_ATable& CXl_ATable::operator=(CXl_ATable& src)
{
	(CXl_ACellBase&)(*this) = (CXl_ACellBase&)src;
	m_iTableSRow=src.m_iTableSRow;
	m_iTableERow=src.m_iTableERow;
	m_iTableSCol=src.m_iTableSCol;
	m_iTableECol=src.m_iTableECol;
	m_arHeader.RemoveAll();
	for(int i=0; i<src.m_arHeader.GetSize(); i++)
	{
		m_arHeader.Add(src.m_arHeader[i]);
	}
	m_arData.RemoveAll();
	for(int i=0; i<src.m_arData.GetSize(); i++)
	{
		m_arData.Add(src.m_arData[i]);
	}
	return *this;
}

BOOL CXl_ATable::WriteTable(CXlBase* pXl)
{
	if(m_bSetNumberFormat)	
		pXl->SetNumberFormat(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol, m_NumberFormatLocal);
	if(m_bSetCellColor)
		pXl->SetBackColor(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol,m_iCellColorIndex);
	if(m_bSetVAlignment)
		pXl->SetVerAlign(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol, m_iVAlignment);
	if(m_bSetHAlignment)
		pXl->SetHorAlign(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol, m_iHAlignment);
	if(m_bSetFont)
	{
		if(m_sFontType!=_T(""))
			pXl->SetFont(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol,m_iFontHeight,m_lFontColor,m_sFontType,m_bFontBold);
		else
			pXl->SetFont(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol,m_iFontHeight,m_lFontColor,m_bFontBold);
	}
	if(m_bSetBorder)
		pXl->SetBorder(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol, m_iBorderLineStyle, m_iBorderThick);

	for(int iCellLine=0; iCellLine<m_arCellLine.GetSize(); iCellLine++)
	{
		T_CellLine CellLine; CellLine.Init();
		CellLine = m_arCellLine.GetAt(iCellLine);
		pXl->CellLine(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol, CellLine.iDirection, CellLine.iCellLineStyle, CellLine.iCellLineThick, CellLine.iCellLineColor);
	}

	//Write Row Header
	for(int iH=0; iH<m_arHeader.GetSize(); iH++)
	{
		m_arHeader[iH].WriteCells(pXl);
	}

	for(int iD=0; iD<m_arData.GetSize(); iD++)
	{
		m_arData[iD].WriteCells(pXl);
	}

	if(m_bSetCellOutLine)
		pXl->CellOutLine(m_iTableSRow,m_iTableSCol,m_iTableERow,m_iTableECol,m_iCellOutLineStyle, m_iCellOutLineThick, m_iCellOutLineColor);

	return TRUE;
}