#include "mythread.h"
#if PLATFORM == PLATFORM_WIN32
	#include <windows.h>
#else
	#include <pthread.h>
	#include "signal.h"
#endif
#include "algorithm"

namespace thread{
/**
 * ͷ�ļ�����
 **/

#if PLATFORM == PLATFORM_WIN32
	// ʵ��window �汾
	/**
	* \brief ���캯��������һ�����������
	*
	*/
	Mutex::Mutex() 
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief ��������������һ�����������
	*
	*/
	Mutex::~Mutex()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief ����һ��������
	*
	*/
	void Mutex::lock()
	{
		if( WaitForSingleObject(m_hMutex,10000) == WAIT_TIMEOUT )
		{
			//TODO error
		}
	}

	/**
	* \brief ����һ��������
	*
	*/
	void Mutex::unlock()
	{
		ReleaseMutex(m_hMutex);
	}
	
	/**
	* \brief ���캯�������ڴ���һ����д��
	*
	*/
	RWLock::RWLock():rd_count(0), wr_count(0)
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief ������������������һ����д��
	*
	*/
	RWLock::~RWLock()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief �Զ�д�����ж���������
	*
	*/
	void RWLock::rdlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	};

	/**
	* \brief �Զ�д������д��������
	*
	*/
	void RWLock::wrlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	}

	/**
	* \brief �Զ�д�����н�������
	*
	*/
	void RWLock::unlock()
	{
		ReleaseMutex(m_hMutex);
	}
	
	/**
	* \brief ���캯�������ڴ���һ����������
	*
	*/
	Cond::Cond()
	{
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	/**
	* \brief ������������������һ����������
	*
	*/
	Cond::~Cond()
	{
		CloseHandle(m_hEvent);
	}

	/**
	* \brief �����еȴ���������������̹߳㲥�����źţ�ʹ��Щ�߳��ܹ���������ִ��
	*
	*/
	void Cond::broadcast()
	{
		SetEvent(m_hEvent);
	}

	/**
	* \brief �����еȴ���������������̷߳����źţ�ʹ��Щ�߳��ܹ���������ִ��
	*
	*/
	void Cond::signal()
	{
		SetEvent(m_hEvent);
	}
	/**
	* \param m_hMutex ��Ҫ�ȴ��Ļ�����
	*/
	void Cond::wait(Mutex &mutex)
	{
		WaitForSingleObject(m_hEvent,INFINITE);
	}
	/**
	* \brief ���캯��������һ������
	*
	* \param name �߳�����
	* \param joinable ��������߳��˳���ʱ���Ƿ񱣴�״̬�����Ϊtrue��ʾ�߳��˳�����״̬�����򽫲������˳�״̬
	*/
	Thread::Thread(const std::string &name,const bool joinable) 
		: threadName(name),alive(false),complete(false),joinable(joinable) { m_hThread = NULL; };

	/**
	* \brief ������������������һ�����󣬻��ն���ռ�
	*
	*/
	Thread::~Thread()
	{
		if (NULL != m_hThread)
		{
			CloseHandle(m_hThread);
		}
	};

	/**
	* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬��
	*
	*
	* \param sec ָ����ʱ�䣬��
	*/
	void Thread::sleep(const long sec)
	{
		::Sleep(1000 * sec);
	}

	/**
	* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬����
	*
	*
	* \param msec ָ����ʱ�䣬����
	*/
	void Thread::msleep(const long msec)
	{
		::Sleep(msec);
	}

	/**
	* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬΢��
	*
	*
	* \param usec ָ����ʱ�䣬΢��
	*/
	void Thread::usleep(const long usec)
	{
		::Sleep(usec / 1000);
	}

	/**
	* \brief �߳��Ƿ���joinable��
	*
	*
	* \return joinable
	*/
	const bool Thread::isJoinable() const
	{
		return joinable;
	}

	/**
	* \brief ����߳��Ƿ�������״̬
	*
	* \return �߳��Ƿ�������״̬
	*/
	const bool Thread::isAlive() const
	{
		return alive;
	}

	/**
	* \brief ���������߳�
	*
	* ��ʵֻ�����ñ�ǣ���ô�̵߳�run���ص�ѭ���ؼ�������ǣ�����������Ѿ����ã����˳�ѭ��
	*
	*/
	void Thread::final()
	{
		complete = true;
	}
	 
	/**
	* \brief �ж��߳��Ƿ����������ȥ
	*
	* ��Ҫ����run()����ѭ���У��ж�ѭ���Ƿ����ִ����ȥ
	*
	* \return �߳����ص��Ƿ����ִ��
	*/
	const bool Thread::isFinal() const 
	{
		return complete;
	}

	/**
	* \brief �����߳�����
	*
	* \return �߳�����
	*/
	const std::string &Thread::getThreadName() const
	{
		return threadName;
	}
	
	/**
	 * \brief �̺߳���
	 *
	 * �ں��������������߳������ʵ�ֵĻص�����
	 *
	 * \param arg �����̵߳Ĳ���
	 * \return �����߳̽�����Ϣ
	 */
	unsigned long WINAPI Thread::threadFunc(void *arg)
	{
		Thread *thread = (Thread *)arg;

		thread->mlock.lock();
		thread->alive = true;
		thread->mlock.unlock();

		//�����̵߳����ص�����
		thread->run();

		thread->mlock.lock();
		thread->alive = false;
		thread->mlock.unlock();

		//�������joinable,��Ҫ�����߳���Դ
		if (!thread->isJoinable())
		{
		delete thread;
		}
		else
		{
		CloseHandle(thread->m_hThread);
		thread->m_hThread = NULL;
		}

		return 0;
	}

	/**
	 * \brief �����߳�,�����߳�
	 *
	 * \return �����߳��Ƿ�ɹ�
	 */
	bool Thread::start()
	{
		unsigned long dwThread;

		//�߳��Ѿ���������,ֱ�ӷ���
		if (alive)
		{
		return true;
		}

		if (NULL == (m_hThread=CreateThread(NULL,0,Thread::threadFunc,this,0,&dwThread))) 
		{
		return false;
		}
		return true;
	}

	/**
	 * \brief �ȴ�һ���߳̽���
	 *
	 */
	void Thread::join()
	{
		WaitForSingleObject(m_hThread,INFINITE);
	}

#else
	// ʵ��linux �� mac �汾
	/**
	 * Mutex
	 **/
	Mutex::Mutex() 
	{
		pthread_mutex_init(&m_hMutex, NULL);
	}
	Mutex::~Mutex()
	{
		pthread_mutex_destroy(&m_hMutex);
	}
	inline void Mutex::lock()
	{
		pthread_mutex_lock(&m_hMutex);
	}
	inline void Mutex::unlock()
	{
		pthread_mutex_unlock(&m_hMutex);
	}
	/**
	 * RWLock
	 */
	RWLock::RWLock() : rd_count(0), wr_count(0)
	{
		pthread_rwlock_init(&m_hMutex, NULL);
	}
	
	RWLock::~RWLock()
	{
		pthread_rwlock_destroy(&m_hMutex);
	}

	inline void RWLock::rdlock()
	{
		pthread_rwlock_rdlock(&m_hMutex);
		++rd_count;
	}

	inline void RWLock::wrlock()
	{
		pthread_rwlock_wrlock(&m_hMutex);
		++wr_count;
		++rd_count;
	}

	inline void RWLock::unlock()
	{
		pthread_rwlock_unlock(&m_hMutex);
		--rd_count;
	}
	/**
	 * Con
	 */
	Cond::Cond()
	{
		::pthread_cond_init(&m_hEvent, NULL);
	}
	Cond::~Cond()
	{
		::pthread_cond_destroy(&m_hEvent);
	}
	void Cond::broadcast()
	{
		::pthread_cond_broadcast(&m_hEvent);
	}
	void Cond::signal()
	{
		::pthread_cond_signal(&m_hEvent);
	}
	void Cond::wait(Mutex &mutex)
	{
		::pthread_cond_wait(&m_hEvent, &mutex.m_hMutex);
	}
	/**
	* \brief ���캯��������һ������
	* \param name �߳�����
	* \param joinable ��������߳��˳���ʱ���Ƿ񱣴�״̬�����Ϊtrue��ʾ�߳��˳�����״̬�����򽫲������˳�״̬
	*/
	Thread::Thread(const std::string &name,const bool joinable) 
		: threadName(name),alive(false),complete(false),joinable(joinable)
    {
	       	m_hThread = 0; 
	};

	Thread::~Thread()
	{
	}

	/**
	* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬��
	* \param sec ָ����ʱ�䣬��
	*/
	void Thread::sleep(const long sec)
	{
		::sleep(1000 * sec);
	}

	/**
	* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬����
	* \param msec ָ����ʱ�䣬����
	*/
	void Thread::msleep(const long msec)
	{
		::sleep(msec);
	}

	/**
	* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬΢��
	* \param usec ָ����ʱ�䣬΢��
	*/
	void Thread::usleep(const long usec)
	{
		::sleep(usec / 1000);
	}

	/**
	* \brief �߳��Ƿ���joinable��
	* \return joinable
	*/
	const bool Thread::isJoinable() const
	{
		return joinable;
	}

	/**
	* \brief ����߳��Ƿ�������״̬
	* \return �߳��Ƿ�������״̬
	*/
	const bool Thread::isAlive() const
	{
		return alive;
	}
	/**
	* \brief ���������߳�
	* ��ʵֻ�����ñ�ǣ���ô�̵߳�run���ص�ѭ���ؼ�������ǣ�����������Ѿ����ã����˳�ѭ��
	*/
	void Thread::final()
	{
		complete = true;
	}

	/**
	* \brief �ж��߳��Ƿ����������ȥ
	* ��Ҫ����run()����ѭ���У��ж�ѭ���Ƿ����ִ����ȥ
	* \return �߳����ص��Ƿ����ִ��
	*/
	const bool Thread::isFinal() const 
	{
		return complete;
	}
	/**
	* \brief �ж������߳��Ƿ���ͬһ���߳�
	* \param other ���Ƚϵ��߳�
	* \return �Ƿ���ͬһ���߳�
	*/
	/*
	const bool Thread::operator==(const Thread& other) const
	{
		return pthread_equal(m_hThread,other.m_hThread) != 0;
	}
	*/
	/**
	* \brief �����߳�����
	*
	* \return �߳�����
	*/
	const std::string &Thread::getThreadName() const
	{
		return threadName;
	}
	/**
	 * \brief �̺߳���
	 * �ں��������������߳������ʵ�ֵĻص�����
	 */
	void* Thread::threadFunc(void *arg)
	{
		Thread *thread = (Thread *)arg;
		
		thread->mlock.lock();
		thread->alive = true;
		thread->cond.broadcast();
		thread->mlock.unlock();

		//�����߳��źŴ�����
		sigset_t sig_mask;
		sigfillset(&sig_mask);
		pthread_sigmask(SIG_SETMASK, &sig_mask, NULL);

		//mysql_thread_init();

		//�����̵߳����ص�����
		thread->run();

		//mysql_thread_end();

		thread->mlock.lock();
		thread->alive = false;
		thread->cond.broadcast();
		thread->mlock.unlock();

		//�������joinable,��Ҫ�����߳���Դ
		if (!thread->isJoinable())
		{
			thread->mlock.lock();
			while (thread->alive)
				thread->cond.wait(thread->mlock);
			thread->mlock.unlock();
			delete (thread);
		}
		
		pthread_exit(NULL);
	}

	/**
	 * \brief �����߳�,�����߳�
	 * \return �����߳��Ƿ�ɹ�
	 */
	bool Thread::start()
	{
		//�߳��Ѿ���������,ֱ�ӷ���
		if (alive)
		{
			return true;
		}

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		if (!joinable) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		if (0 != pthread_create(&m_hThread, &attr, threadFunc, this))
		{
			return false;
		}

		pthread_attr_destroy(&attr);
		mlock.lock();
		while (!alive)
			cond.wait(mlock);
		mlock.unlock();
		return true;
	}

	/**
	 * \brief �ȴ�һ���߳̽���
	 *
	 */
	void Thread::join()
	{
		if (0 != m_hThread && joinable)
		{
			::pthread_join(m_hThread, NULL);
			m_hThread = 0;
			mlock.lock();
			while (alive)
				cond.wait(mlock);
			mlock.unlock();
		}
	}
#endif

	/**
	 * Mutex_scope_lock
	 **/
	Mutex_scope_lock::Mutex_scope_lock(Mutex &m) : mlock(m)
	{
		mlock.lock();
	}
	/**
	 *��������unlock����
	 */
	Mutex_scope_lock::~Mutex_scope_lock()
	{
		mlock.unlock();
	}

	RWLock_scope_rdlock::RWLock_scope_rdlock(RWLock &m) : rwlock(m)
	{
		rwlock.rdlock();
	}

	RWLock_scope_rdlock::~RWLock_scope_rdlock()
	{
		rwlock.unlock();
	}

	RWLock_scope_wrlock::RWLock_scope_wrlock(RWLock &m) : rwlock(m)
	{
		rwlock.wrlock();
	}

	RWLock_scope_wrlock::~RWLock_scope_wrlock()
	{
		rwlock.unlock();
	}

	ThreadGroup::ThreadGroup() : vts(), rwlock()
	{
	}

	ThreadGroup::~ThreadGroup()
	{
		joinAll();
	}

	/**
	 * \brief ���һ���̵߳�������
	 * \param thread ����ӵ��߳�
	 */
	void ThreadGroup::add(Thread *thread)
	{
		RWLock_scope_wrlock scope_wrlock(rwlock);
		Container::iterator it = std::find(vts.begin(),vts.end(),thread);
		if (it == vts.end())
			vts.push_back(thread);
	}

	/**
	 * \brief ����index�±��ȡ�߳�
	 * \param index �±���
	 */
	Thread *ThreadGroup::getByIndex(const Container::size_type index)
	{
		RWLock_scope_rdlock scope_rdlock(rwlock);
		if (index >= vts.size())
			return NULL;
		else
			return vts[index];
	}

	/**
	 * \brief ����[]�����,����index�±��ȡ�߳�
	 * \param index �±���
	 * \return �߳�
	 */
	Thread *ThreadGroup::operator[] (const Container::size_type index)
	{
		RWLock_scope_rdlock scope_rdlock(rwlock);
		if (index >= vts.size())
			return NULL;
		else
			return vts[index];
	}

	/**
	 * \brief �ȴ������е������߳̽���
	 */
	void ThreadGroup::joinAll()
	{
		RWLock_scope_rdlock scope_wrlock(rwlock);
		while(!vts.empty())
		{
			Thread *pThread = vts.back();
			vts.pop_back();
			if (pThread)
			{
				pThread->final();
				pThread->join();
				delete (pThread);
			}
		}
	}

	/**
	 * \brief �������е�����Ԫ�ص��ûص�����
	 * \param cb �ص�����ʵ��
	 */
	void ThreadGroup::execAll(Callback &cb)
	{
		RWLock_scope_rdlock scope_rdlock(rwlock);
		for(Container::iterator it = vts.begin(); it != vts.end(); ++it)
		{
			cb.exec(*it);
		}
	}
}

