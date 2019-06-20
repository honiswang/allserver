#include "pjlib.h"
#include "sync.h"
#include "pstring.h"
PString::PString():m_pool(NULL)
{
	m_pool =pj_pool_create(&pool::g_cp.factory, // the factory
			"poolstring", // pool's name
			4000, // initial size
			4000, // increment size
			NULL);
}
PString::PString(const char* str)
{
	m_str = pj_str((char *)str);
}

PString::~PString()
{

}

PString::operator const char *() const
{
	return pj_strbuf(&m_str);
}
long PString::GetLength() const
{
	return pj_strtol(&m_str);
}
