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
 * 头文件包含
 **/

#if PLATFORM == PLATFORM_WIN32
	// 实现window 版本
	/**
	* \brief 构造函数，构造一个互斥体对象
	*
	*/
	Mutex::Mutex() 
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief 析构函数，销毁一个互斥体对象
	*
	*/
	Mutex::~Mutex()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief 加锁一个互斥体
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
	* \brief 解锁一个互斥体
	*
	*/
	void Mutex::unlock()
	{
		ReleaseMutex(m_hMutex);
	}
	
	/**
	* \brief 构造函数，用于创建一个读写锁
	*
	*/
	RWLock::RWLock():rd_count(0), wr_count(0)
	{
		m_hMutex = CreateMutex(NULL,FALSE,NULL);
	}

	/**
	* \brief 析构函数，用于销毁一个读写锁
	*
	*/
	RWLock::~RWLock()
	{
		CloseHandle(m_hMutex);
	}

	/**
	* \brief 对读写锁进行读加锁操作
	*
	*/
	void RWLock::rdlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	};

	/**
	* \brief 对读写锁进行写加锁操作
	*
	*/
	void RWLock::wrlock()
	{
		WaitForSingleObject(m_hMutex,INFINITE);
	}

	/**
	* \brief 对读写锁进行解锁操作
	*
	*/
	void RWLock::unlock()
	{
		ReleaseMutex(m_hMutex);
	}
	
	/**
	* \brief 构造函数，用于创建一个条件变量
	*
	*/
	Cond::Cond()
	{
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	/**
	* \brief 析构函数，用于销毁一个条件变量
	*
	*/
	Cond::~Cond()
	{
		CloseHandle(m_hEvent);
	}

	/**
	* \brief 对所有等待这个条件变量的线程广播发送信号，使这些线程能够继续往下执行
	*
	*/
	void Cond::broadcast()
	{
		SetEvent(m_hEvent);
	}

	/**
	* \brief 对所有等待这个条件变量的线程发送信号，使这些线程能够继续往下执行
	*
	*/
	void Cond::signal()
	{
		SetEvent(m_hEvent);
	}
	/**
	* \param m_hMutex 需要等待的互斥体
	*/
	void Cond::wait(Mutex &mutex)
	{
		WaitForSingleObject(m_hEvent,INFINITE);
	}
	/**
	* \brief 构造函数，创建一个对象
	*
	* \param name 线程名称
	* \param joinable 标明这个线程退出的时候是否保存状态，如果为true表示线程退出保存状态，否则将不保存退出状态
	*/
	Thread::Thread(const std::string &name,const bool joinable) 
		: threadName(name),alive(false),complete(false),joinable(joinable) { m_hThread = NULL; };

	/**
	* \brief 析构函数，用于销毁一个对象，回收对象空间
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
	* \brief 使当前线程睡眠指定的时间，秒
	*
	*
	* \param sec 指定的时间，秒
	*/
	void Thread::sleep(const long sec)
	{
		::Sleep(1000 * sec);
	}

	/**
	* \brief 使当前线程睡眠指定的时间，毫秒
	*
	*
	* \param msec 指定的时间，毫秒
	*/
	void Thread::msleep(const long msec)
	{
		::Sleep(msec);
	}

	/**
	* \brief 使当前线程睡眠指定的时间，微秒
	*
	*
	* \param usec 指定的时间，微秒
	*/
	void Thread::usleep(const long usec)
	{
		::Sleep(usec / 1000);
	}

	/**
	* \brief 线程是否是joinable的
	*
	*
	* \return joinable
	*/
	const bool Thread::isJoinable() const
	{
		return joinable;
	}

	/**
	* \brief 检查线程是否在运行状态
	*
	* \return 线程是否在运行状态
	*/
	const bool Thread::isAlive() const
	{
		return alive;
	}

	/**
	* \brief 主动结束线程
	*
	* 其实只是设置标记，那么线程的run主回调循环回检查这个标记，如果这个标记已经设置，就退出循环
	*
	*/
	void Thread::final()
	{
		complete = true;
	}
	 
	/**
	* \brief 判断线程是否继续运行下去
	*
	* 主要用在run()函数循环中，判断循环是否继续执行下去
	*
	* \return 线程主回调是否继续执行
	*/
	const bool Thread::isFinal() const 
	{
		return complete;
	}

	/**
	* \brief 返回线程名称
	*
	* \return 线程名称
	*/
	const std::string &Thread::getThreadName() const
	{
		return threadName;
	}
	
	/**
	 * \brief 线程函数
	 *
	 * 在函数体里面会调用线程类对象实现的回调函数
	 *
	 * \param arg 传入线程的参数
	 * \return 返回线程结束信息
	 */
	unsigned long WINAPI Thread::threadFunc(void *arg)
	{
		Thread *thread = (Thread *)arg;

		thread->mlock.lock();
		thread->alive = true;
		thread->mlock.unlock();

		//运行线程的主回调函数
		thread->run();

		thread->mlock.lock();
		thread->alive = false;
		thread->mlock.unlock();

		//如果不是joinable,需要回收线程资源
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
	 * \brief 创建线程,启动线程
	 *
	 * \return 创建线程是否成功
	 */
	bool Thread::start()
	{
		unsigned long dwThread;

		//线程已经创建运行,直接返回
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
	 * \brief 等待一个线程结束
	 *
	 */
	void Thread::join()
	{
		WaitForSingleObject(m_hThread,INFINITE);
	}

#else
	// 实现linux 和 mac 版本
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
	* \brief 构造函数，创建一个对象
	* \param name 线程名称
	* \param joinable 标明这个线程退出的时候是否保存状态，如果为true表示线程退出保存状态，否则将不保存退出状态
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
	* \brief 使当前线程睡眠指定的时间，秒
	* \param sec 指定的时间，秒
	*/
	void Thread::sleep(const long sec)
	{
		::sleep(1000 * sec);
	}

	/**
	* \brief 使当前线程睡眠指定的时间，毫秒
	* \param msec 指定的时间，毫秒
	*/
	void Thread::msleep(const long msec)
	{
		::sleep(msec);
	}

	/**
	* \brief 使当前线程睡眠指定的时间，微秒
	* \param usec 指定的时间，微秒
	*/
	void Thread::usleep(const long usec)
	{
		::sleep(usec / 1000);
	}

	/**
	* \brief 线程是否是joinable的
	* \return joinable
	*/
	const bool Thread::isJoinable() const
	{
		return joinable;
	}

	/**
	* \brief 检查线程是否在运行状态
	* \return 线程是否在运行状态
	*/
	const bool Thread::isAlive() const
	{
		return alive;
	}
	/**
	* \brief 主动结束线程
	* 其实只是设置标记，那么线程的run主回调循环回检查这个标记，如果这个标记已经设置，就退出循环
	*/
	void Thread::final()
	{
		complete = true;
	}

	/**
	* \brief 判断线程是否继续运行下去
	* 主要用在run()函数循环中，判断循环是否继续执行下去
	* \return 线程主回调是否继续执行
	*/
	const bool Thread::isFinal() const 
	{
		return complete;
	}
	/**
	* \brief 判断两个线程是否是同一个线程
	* \param other 待比较的线程
	* \return 是否是同一个线程
	*/
	/*
	const bool Thread::operator==(const Thread& other) const
	{
		return pthread_equal(m_hThread,other.m_hThread) != 0;
	}
	*/
	/**
	* \brief 返回线程名称
	*
	* \return 线程名称
	*/
	const std::string &Thread::getThreadName() const
	{
		return threadName;
	}
	/**
	 * \brief 线程函数
	 * 在函数体里面会调用线程类对象实现的回调函数
	 */
	void* Thread::threadFunc(void *arg)
	{
		Thread *thread = (Thread *)arg;
		
		thread->mlock.lock();
		thread->alive = true;
		thread->cond.broadcast();
		thread->mlock.unlock();

		//设置线程信号处理句柄
		sigset_t sig_mask;
		sigfillset(&sig_mask);
		pthread_sigmask(SIG_SETMASK, &sig_mask, NULL);

		//mysql_thread_init();

		//运行线程的主回调函数
		thread->run();

		//mysql_thread_end();

		thread->mlock.lock();
		thread->alive = false;
		thread->cond.broadcast();
		thread->mlock.unlock();

		//如果不是joinable,需要回收线程资源
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
	 * \brief 创建线程,启动线程
	 * \return 创建线程是否成功
	 */
	bool Thread::start()
	{
		//线程已经创建运行,直接返回
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
	 * \brief 等待一个线程结束
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
	 *对锁进行unlock操作
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
	 * \brief 添加一个线程到分组中
	 * \param thread 待添加的线程
	 */
	void ThreadGroup::add(Thread *thread)
	{
		RWLock_scope_wrlock scope_wrlock(rwlock);
		Container::iterator it = std::find(vts.begin(),vts.end(),thread);
		if (it == vts.end())
			vts.push_back(thread);
	}

	/**
	 * \brief 按照index下标获取线程
	 * \param index 下标编号
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
	 * \brief 重载[]运算符,按照index下标获取线程
	 * \param index 下标编号
	 * \return 线程
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
	 * \brief 等待分组中的所有线程结束
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
	 * \brief 对容器中的所有元素调用回调函数
	 * \param cb 回调函数实例
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

