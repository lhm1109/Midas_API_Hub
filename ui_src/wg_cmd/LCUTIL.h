#ifndef __CLCUTIL_H__
#define __CLCUTIL_H__

class CLCUtil
{
public:
	static BOOL SetListItemDbl(CListCtrl *pList, int nIndex , CStringArray &Contents,double ItemData);
	static BOOL GetListItemDbl(CListCtrl *pList, int nIndex, CStringArray &Contents ,double& ItemData);
	static BOOL GetSelectedListItemDataDbl(CListCtrl *pList, CArray<double, double> &arRet);
	static BOOL ClearAllListItemDbl(CListCtrl* pList);
	static BOOL DeleteListItemDbl(CListCtrl *pList, int nIndex);
};

#endif
