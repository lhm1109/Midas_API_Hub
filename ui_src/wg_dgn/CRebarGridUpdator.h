// CRebarGridUpdator.h: interface for the CRebarGridUpdator class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREBARGRIDUPDATOR_H__224DA86B_9281_43BF_ACBE_7265560E9332__INCLUDED_)
#define AFX_CREBARGRIDUPDATOR_H__224DA86B_9281_43BF_ACBE_7265560E9332__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDgnCreateRebarGrid;
class CDgnCreateRebarGrid_J;

class CRebarGridUpdator  
{
protected:
	CDgnCreateRebarGrid * m_pRebarGrid;
	CDgnCreateRebarGrid_J * m_pRebarJGrid;
public:
	static CRebarGridUpdator* Instance();

	void SetGridWnd  (CDgnCreateRebarGrid* pRebarGrid, CDgnCreateRebarGrid_J* pRebarJGrid);
	void AddData2Grid(CStringArray & arNames,CArray<int,int>& arRebarNumbers,CArray<double, double>& arAreas);
	void OnGridSelChanged(CDWordArray& SelectedRows);
	
	CRebarGridUpdator();
	virtual ~CRebarGridUpdator();
};

#endif // !defined(AFX_CREBARGRIDUPDATOR_H__224DA86B_9281_43BF_ACBE_7265560E9332__INCLUDED_)
