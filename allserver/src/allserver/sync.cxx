#include <pjlib.h>
#include "sync.h"
pool::pool()
{
}
pool::~pool()
{
}
pj_caching_pool pool::g_cp;
PMutex::PMutex():m_pool(NULL),m_mutex(NULL)
{
	init();
}
PMutex::~PMutex()
{
	pj_mutex_destroy(m_mutex);
	pj_pool_release(m_pool);
}
void PMutex::init()
{
	if(*(int *)&pool::g_cp == 0)
	{
		pj_status_t status;
		// Must init PJLIB before anything else
		status = pj_init();
		if (status != PJ_SUCCESS) {
			PJ_LOG(3,(__FILE__,"Error initializing PJLIB %d", status));
			return;
		}
		// Create the pool factory, in this case, a caching pool,
		// using default pool policy.
		pj_caching_pool_init(&pool::g_cp, NULL, 1024*1024 );
	}
	m_pool = pj_pool_create(&pool::g_cp.factory, "pool1", // pool's name
			4000, // initial size
			4000, // increment size
			NULL);
	if(m_pool != NULL)
	{
		pj_mutex_create(m_pool,"mutex",PJ_MUTEX_SIMPLE,&m_mutex);
		pj_event_create(m_pool,"sync",PJ_FALSE,PJ_TRUE,&m_event);
	}
	else
	{
		PJ_LOG(3,(__FILE__, "pmutex create fiald"));
	}
}
void PMutex::Lock()
{
	pj_mutex_lock(m_mutex);
}

void PMutex::UnLock()
{
	pj_mutex_unlock(m_mutex);
}
void PMutex::Wait()
{
	pj_event_wait(m_event);
}
void PMutex::Signal()
{
	pj_event_reset(m_event);
}
PSyncPoint::PSyncPoint():m_pool(NULL),m_event(NULL)
{
	m_pool = pj_pool_create(&pool::g_cp.factory, "pool1", // pool's name
			4000, // initial size
			4000, // increment size
			NULL);
	if(m_pool != NULL)
	{ 
		pj_event_create(m_pool,"sync",PJ_FALSE,PJ_TRUE,&m_event);
	} 
	else
	{
		PJ_LOG(3,(__FILE__, "psync create fiald"));
	}

}
PSyncPoint::~PSyncPoint()
{
	pj_event_destroy(m_event);
	pj_pool_release(m_pool);
}
void PSyncPoint::Wait()
{
	pj_event_wait(m_event);
}
bool PSyncPoint::Wait(const long &timeout)
{
	return pj_event_wait_for(m_event, timeout);
}
void PSyncPoint::Signal()
{
	pj_event_reset(m_event);
}

PWaitAndSignal::PWaitAndSignal(PMutex &mutex):m_mutex(mutex)
{
	m_mutex.Lock();
}
PWaitAndSignal::~PWaitAndSignal()
{
	m_mutex.UnLock();
}
PReadWriteMutex::PReadWriteMutex()
{
}
PReadWriteMutex::~PReadWriteMutex()
{
}
void PReadWriteMutex::StartRead()
{
	m_read.Lock();
}
void PReadWriteMutex::EndRead()
{
	m_read.UnLock();
}
void PReadWriteMutex::StartWrite()
{
	m_write.Lock();
}
void PReadWriteMutex::EndWrite()
{
	m_write.UnLock();
}
