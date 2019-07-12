#include <map>
#include <string>
class httphandle;
typedef std::string (httphandle::*handle)();
class httphandle
{
private:
	pj_pool_t *m_pool;
	std::map<std::string,std::string> m_head; 
	std::map<std::string,std::string> m_param; 
	std::map<std::string,handle> m_mtoh;
	std::string m_response;
private:
	std::string getprocess();
	std::string postprocess();
	std::string putprocess();
	std::string patchprocess();
	std::string deleteprocess();
public:
	httphandle();
	httphandle(pj_pool_t *pool);
	virtual ~httphandle();
	bool parse(char * buf);
	std::string process();
	void operator() (std::pair<std::string,std::string> item);
	template <class item, class func>
	void for_each(item b,item e, func &_f)
	{
		while(b!=e)
		{
			_f(*b);
			++b;
		}
	}
};
