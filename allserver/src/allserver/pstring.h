#ifndef __PSTRING__
#define __PSTRING__
class PString
{
	private:
		pj_str_t m_str;
		pj_pool_t *m_pool;
	public:
		PString();
		PString(const char*);
		virtual ~PString();
		operator const char *() const;
		long GetLength() const;
};

#endif
