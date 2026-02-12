#pragma once

#include <functional>

class CBaseTreeCtrl : public SECTreeCtrl
{
	DECLARE_DYNAMIC(CBaseTreeCtrl);
public:
	CBaseTreeCtrl(void);
	virtual ~CBaseTreeCtrl(void);

protected:
	virtual void DoPaint(CDC* pDC);
};
