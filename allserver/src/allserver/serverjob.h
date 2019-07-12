#include <list>
class serverjob: public RegularJob
{
private:
	pj_ioqueue_t *m_ioqueue;
	pj_pool_t *m_pool;
	std::list<pj_ioqueue_key_t *> m_closekeys;	
public:
	serverjob();
	virtual ~serverjob();
	void addsock(pj_sock_t sock);
	virtual void Exec();
	void init();
	void addclose(pj_ioqueue_key_t *key);
};
