#include <list>
class serverjob;
class testjob : public RegularJob
{
private:
	pj_ioqueue_t *m_ioqueue;
	pj_pool_t *m_pool;
	int m_port;
	pj_ioqueue_key_t *m_key;
	std::list<serverjob*> m_jobs;

	testjob();
public:
	testjob(int port);
	virtual ~testjob();
	virtual void Exec();
	void init(int num =4);
};
