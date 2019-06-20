#ifndef __SYNC__
#define __SYNC__

class pool
{
	public:
		pool();
		~pool();
	static pj_caching_pool g_cp;
};

class PMutex
{
	private:
		pj_mutex_t *m_mutex;
		pj_event_t *m_event;
		pj_pool_t *m_pool;
	public:
		PMutex();
		virtual ~PMutex();
		void init();
		void Lock();
		void UnLock();
		void Wait();
		void Signal();
};

class PSyncPoint
{
	private:
		pj_event_t *m_event;
		pj_pool_t *m_pool;
	public:
		PSyncPoint();
		virtual ~PSyncPoint();
		void Wait();
		bool Wait(const long&);
		void Signal();
};

class PWaitAndSignal
{
	PMutex &m_mutex;
	public:
		PWaitAndSignal(PMutex &mutex);
		~PWaitAndSignal();
};
class PReadWriteMutex
{
	private:
		PMutex m_read,m_write;
	public:
		PReadWriteMutex();
		~PReadWriteMutex();
		void StartWrite();
		void EndWrite();
		void StartRead();
		void EndRead();
};
#endif
