#pragma once

struct DGN_PAGE_BREAK
{
	bool bPageBreak;
	long nLineCount;
	std::vector<CString> vPageBreak;

	void Init()
	{
		bPageBreak = true;
		nLineCount = 0;
		vPageBreak.clear();
	}
	DGN_PAGE_BREAK() { Init(); }
	DGN_PAGE_BREAK(const DGN_PAGE_BREAK& src) { *this = src; }
	DGN_PAGE_BREAK& operator = (const DGN_PAGE_BREAK& src)
	{
		bPageBreak = src.bPageBreak;
		nLineCount = src.nLineCount;
		vPageBreak = src.vPageBreak;
		return *this;
	}
};

struct DGN_FRACTION_DATA
{
	CString csUpper;
	CString csLower;

	void Init()
	{
		csUpper.Empty();
		csLower.Empty();
	}
	DGN_FRACTION_DATA(const CString& Upper, const CString& Lower)
	{
		csUpper = Upper;
		csLower = Lower;
	}
	DGN_FRACTION_DATA() { Init(); }
	DGN_FRACTION_DATA(const DGN_FRACTION_DATA& src) { *this = src; }
	DGN_FRACTION_DATA& operator = (const DGN_FRACTION_DATA& src)
	{
		csUpper = src.csUpper;
		csLower = src.csLower;
		return *this;
	}
};

typedef CArray<DGN_FRACTION_DATA, DGN_FRACTION_DATA&> DGN_FRACTION_DATA_LIST;

struct D_TEXT_TABLE_CELL
{
	UINT nLine;  // Number of Text Line.
	UINT nWidth; // Width of content.
	UINT nAlign; // Horizontal Alignment
	CStringArray aText;

	void Init()
	{
		nLine = 0;
		aText.RemoveAll();
	}
	D_TEXT_TABLE_CELL() { Init(); }
	D_TEXT_TABLE_CELL(const D_TEXT_TABLE_CELL& src) { *this = src; }
	D_TEXT_TABLE_CELL& operator = (const D_TEXT_TABLE_CELL& src)
	{
		nLine = src.nLine;
		aText.Copy(src.aText);
		return *this;
	}
};

typedef CArray<D_TEXT_TABLE_CELL, D_TEXT_TABLE_CELL&> D_TEXT_TABLE_CELL_LIST;

struct D_TEXT_TABLE_ROW
{
	D_TEXT_TABLE_CELL_LIST aCol;

	void Init()
	{
		aCol.RemoveAll();
	}
	D_TEXT_TABLE_ROW() { Init(); }
	D_TEXT_TABLE_ROW(const D_TEXT_TABLE_ROW& src) { *this = src; }
	D_TEXT_TABLE_ROW& operator = (const D_TEXT_TABLE_ROW& src)
	{
		aCol.Copy(src.aCol);
		return *this;
	}
};

typedef CArray<D_TEXT_TABLE_ROW, D_TEXT_TABLE_ROW&> D_TEXT_TABLE_ROW_LIST;

struct D_TEXT_TABLE
{
	D_TEXT_TABLE_ROW_LIST aRow;

	void Init()
	{
		aRow.RemoveAll();
	}
	void SetTableSize(UINT nRow, UINT nCol)
	{
		aRow.SetSize(nRow);
		for ( UINT i = 0; i < nRow; ++i )
		{
			aRow[i].aCol.SetSize(nCol);
		}
	}
	D_TEXT_TABLE() { Init(); }
	D_TEXT_TABLE(const D_TEXT_TABLE& src) { *this = src; }
	D_TEXT_TABLE& operator = (const D_TEXT_TABLE& src)
	{
		aRow.Copy(src.aRow);
		return *this;
	}
};

struct D_UNICODE_TEXT_WIDTH
{
	long nFrom;
	long nTo;
	CString csProp; // East Asian Width Property ( A, F, H, N, Na, W )
	CString csDesc;

	void Init()
	{
		nFrom = 0;
		nTo = 0;
		csProp.Empty();
		csDesc.Empty();
	}
	D_UNICODE_TEXT_WIDTH() { Init(); }
	D_UNICODE_TEXT_WIDTH(const D_UNICODE_TEXT_WIDTH& src) { *this = src; }
	D_UNICODE_TEXT_WIDTH& operator= (const D_UNICODE_TEXT_WIDTH& src)
	{
		nFrom = src.nFrom;
		nTo = src.nTo;
		csProp = src.csProp;
		csDesc = src.csDesc;
		return *this;
	}
};

typedef CMapEx<UINT, UINT, D_UNICODE_TEXT_WIDTH, D_UNICODE_TEXT_WIDTH&> D_UNICODE_TEXT_WIDTH_MAPEX;

struct D_EQUAL_POS_INFO
{
	long nPosi;
	long nSpac;

	void Init()
	{
		nPosi = 0;
		nSpac = 0;
	}
	D_EQUAL_POS_INFO() { Init(); }
	D_EQUAL_POS_INFO(const D_EQUAL_POS_INFO& src) { *this = src; }
	D_EQUAL_POS_INFO& operator= (const D_EQUAL_POS_INFO& src)
	{
		nPosi = src.nPosi;
		nSpac = src.nSpac;
		return *this;
	}
};
