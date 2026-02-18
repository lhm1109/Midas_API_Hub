// DgnCsgDataCtrlAS5100_6_2017.h: interface for the CDgnCsgDataCtrlAS5100_6_2017 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrlAS5100_6_2017_H__INCLUDED_)
#define AFX_DGNCsgDataCtrlAS5100_6_2017_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnCsgDataCtrl.h"

#include "HeaderPre.h"

struct CSG_MEMB_RES_AASHTO_LRFD_STR_D;

class __MY_EXT_CLASS__ CDgnCsgDataCtrlAS5100_6_2017 : public CDgnCsgDataCtrl
{
	// Member Functions.
public:
	CDgnCsgDataCtrlAS5100_6_2017();
	virtual ~CDgnCsgDataCtrlAS5100_6_2017();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);

};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrlAS5100_6_2017_H__INCLUDED_)
