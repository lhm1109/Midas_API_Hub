#pragma once

#include "IDgnPerformDataBase.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ IDgnPerformDataBaseSTL : public IDgnPerformDataBase
{
public:
    IDgnPerformDataBaseSTL();
    virtual ~IDgnPerformDataBaseSTL();

protected:
    virtual dgn::def::enDgn GetDgnType() override;
    virtual bool IsDesigning() override;
	virtual BOOL IsStl() { return TRUE; }
};

#include "HeaderPost.h"
