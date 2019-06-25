class serverjob: public RegularJob
{
private:
	pj_ioqueue_t *m_ioqueue;
	pj_pool_t *m_pool;

public:
	serverjob();
	virtual ~serverjob();
	void addsock(pj_sock_t sock);
	virtual void Exec();
	void init();
};
