#pragma once
#include "platfrom.h"
#if PLATFORM == PLATFORM_WIN32
	#define MY_HANDLE void*
	#define MY_API  __stdcall
	#define MY_MUTEX_HANDLE void*
	#define MY_COND_HANDLE void* 
	#define MY_RW_LOCK_HANDLE void*
#else 
	#define MY_HANDLE pthread_t // 当非windows 是将 HANDLE设置为 int
	#define MY_API
	#define MY_MUTEX_HANDLE pthread_mutex_t
	#define MY_COND_HANDLE pthread_cond_t
	#define MY_RW_LOCK_HANDLE pthread_rwlock_t 
#endif
#include <list>
#include <vector>
#include <string>
/**
 * 声明类集合
 **/
namespace thread{
	class Con;
	class Mutex;
	class RWLock;
	class RWLock_scope_rdlock;
	class Thread;
	class ThreadGroup;
};

namespace thread{
	class Mutex
	{
	public:
		/**
		* \brief 构造函数，构造一个互斥体对象
		*
		*/
		Mutex();

		/**
		* \brief 析构函数，销毁一个互斥体对象
		*
		*/
		~Mutex();

		/**
		* \brief 加锁一个互斥体
		*
		*/
		inline void lock();

		/**
		* \brief 解锁一个互斥体
		*
		*/
		inline void unlock();

		MY_MUTEX_HANDLE	 m_hMutex;    /**< 系统互斥体 */
	};

	/**
	* \brief Wrapper
	* 方便在复杂函数中锁的使用
	*/
	class Mutex_scope_lock
	{
	public:
		/**
		* \brief 构造函数
		* 对锁进行lock操作
		* \param m 锁的引用
		*/
		Mutex_scope_lock(Mutex &m);
		/**
		* \brief 析购函数
		* 对锁进行unlock操作
		*/
		~Mutex_scope_lock();
	private:

		/**
		* \brief 锁的引用
		*/
		Mutex &mlock;
	};
	/**
	* \brief 封装了系统读写锁，使用上要简单，省去了手工初始化和销毁系统读写锁的工作，这些工作都可以由构造函数和析构函数来自动完成
	*
	*/
	class RWLock
	{

	public:
		/**
		* \brief 构造函数，用于创建一个读写锁
		*
		*/
		RWLock();
		/**
		* \brief 析构函数，用于销毁一个读写锁
		*
		*/
		~RWLock();
		/**
		* \brief 对读写锁进行读加锁操作
		*
		*/
		inline void rdlock();
		/**
		* \brief 对读写锁进行写加锁操作
		*
		*/
		inline void wrlock();
		/**
		* \brief 对读写锁进行解锁操作
		*
		*/
		inline void unlock();
	private:
		MY_RW_LOCK_HANDLE m_hMutex;    /**< 系统读写锁 */
		//读写计数测试
		unsigned int rd_count;
		unsigned int wr_count;
	};
	/**
	* \brief rdlock Wrapper
	* 方便在复杂函数中读写锁的使用
	*/
	class RWLock_scope_rdlock
	{

	public:

		/**
		* \brief 构造函数
		* 对锁进行rdlock操作
		* \param m 锁的引用
		*/
		RWLock_scope_rdlock(RWLock &m);
		/**
		* \brief 析购函数
		* 对锁进行unlock操作
		*/
		~RWLock_scope_rdlock();
	private:

		/**
		* \brief 锁的引用
		*/
		RWLock &rwlock;

	};

	/**
	* \brief wrlock Wrapper
	* 方便在复杂函数中读写锁的使用
	*/
	class RWLock_scope_wrlock
	{

	public:

		/**
		* \brief 构造函数
		* 对锁进行wrlock操作
		* \param m 锁的引用
		*/
		RWLock_scope_wrlock(RWLock &m);

		/**
		* \brief 析购函数
		* 对锁进行unlock操作
		*/
		~RWLock_scope_wrlock();
	private:

		/**
		* \brief 锁的引用
		*/
		RWLock &rwlock;

	};
	/**
	* \brief 封装了系统条件变量，使用上要简单，省去了手工初始化和销毁系统条件变量的工作，这些工作都可以由构造函数和析构函数来自动完成
	*
	*/
	class Cond 
	{
	public:
		/**
		* \brief 构造函数，用于创建一个条件变量
		*
		*/
		Cond();
		/**
		* \brief 析构函数，用于销毁一个条件变量
		*
		*/
		~Cond();

		/**
		* \brief 对所有等待这个条件变量的线程广播发送信号，使这些线程能够继续往下执行
		*
		*/
		void broadcast();

		/**
		* \brief 对所有等待这个条件变量的线程发送信号，使这些线程能够继续往下执行
		*
		*/
		void signal();

		/**
		* \brief 等待特定的条件变量满足
		*
		*
		* \param m_hMutex 需要等待的互斥体
		*/
		void wait(Mutex &mutex);
	private:
		MY_COND_HANDLE m_hEvent;    /**< 系统条件变量 */
	};

	/**
	* \brief 封装了线程操作，所有使用线程的基类
	*
	*/
	class Thread
	{
	public:

		/**
		* \brief 构造函数，创建一个对象
		*
		* \param name 线程名称
		* \param joinable 标明这个线程退出的时候是否保存状态，如果为true表示线程退出保存状态，否则将不保存退出状态
		*/
		Thread(const std::string &name = std::string("zThread"),const bool joinable = true);

		/**
		* \brief 析构函数，用于销毁一个对象，回收对象空间
		*
		*/
		virtual ~Thread();

		/**
		* \brief 使当前线程睡眠指定的时间，秒
		*
		*
		* \param sec 指定的时间，秒
		*/
		static void sleep(const long sec);

		/**
		* \brief 使当前线程睡眠指定的时间，毫秒
		*
		*
		* \param msec 指定的时间，毫秒
		*/
		static void msleep(const long msec);

		/**
		* \brief 使当前线程睡眠指定的时间，微秒
		*
		*
		* \param usec 指定的时间，微秒
		*/
		static void usleep(const long usec);

		/**
		* \brief 线程是否是joinable的
		*
		*
		* \return joinable
		*/
		const bool isJoinable() const;

		/**
		* \brief 检查线程是否在运行状态
		*
		* \return 线程是否在运行状态
		*/
		const bool isAlive() const;
#if PLATFORM == PLATFORM_WIN32
		static unsigned long MY_API threadFunc(void *arg);
#else
		static void* threadFunc(void *arg);
#endif
		bool start();
		void join();

		/**
		* \brief 主动结束线程
		*
		* 其实只是设置标记，那么线程的run主回调循环回检查这个标记，如果这个标记已经设置，就退出循环
		*
		*/
		void final();
		/**
		* \brief 判断线程是否继续运行下去
		*
		* 主要用在run()函数循环中，判断循环是否继续执行下去
		*
		* \return 线程主回调是否继续执行
		*/
		const bool isFinal() const;
		/**
		* \brief 纯虚构函数，线程主回调函数，每个需要实例华的派生类需要重载这个函数
		*
		* 如果是无限循环需要在每个循环检查线程退出标记isFinal()，这样能够保证线程安全退出
		* <pre>
		*   while(!isFinal())
		*   {
		*     ...
		*   }
		*   </pre>
		*
		*/
		virtual void run() = 0;
		/**
		* \brief 返回线程名称
		*
		* \return 线程名称
		*/
		const std::string &getThreadName() const;

	public:
		std::string threadName;      /**< 线程名称 */
		Mutex mlock;          /**< 互斥锁 */
		volatile bool alive;      /**< 线程是否在运行 */
		volatile bool complete;
		MY_HANDLE m_hThread;        /**< 线程编号 */
		bool joinable;          /**< 线程属性，是否设置joinable标记 */
		Cond cond;
	}; 

	/**
	* \brief 对线程进行分组管理的类
	*
	*/
	class ThreadGroup
	{

	public:

		struct Callback
		{
			virtual void exec(Thread *e)=0;
			virtual ~Callback(){};
		};

		typedef std::vector<Thread *> Container;  /**< 容器类型 */

		ThreadGroup();
		~ThreadGroup();
		void add(Thread *thread);
		Thread *getByIndex(const Container::size_type index);
		Thread *operator[] (const Container::size_type index);
		void joinAll();
		void execAll(Callback &cb);

		const Container::size_type size()
		{
			RWLock_scope_rdlock scope_rdlock(rwlock);
			return vts.size();
		}

	private:

		Container vts;                /**< 线程向量 */
		RWLock rwlock;                /**< 读写锁 */
	};
};
