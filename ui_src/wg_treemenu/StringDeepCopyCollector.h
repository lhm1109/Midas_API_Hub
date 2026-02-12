#pragma once

#include <vector>

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CStringDeepCopyCollector
{
public:
	CStringDeepCopyCollector();
	~CStringDeepCopyCollector();

public:
	TCHAR* StringDeepCopy(CString& str);

protected:
	std::vector<TCHAR*> m_aDelete;
};
#include "HeaderPost.h"