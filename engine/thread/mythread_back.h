#pragma once
#include "platfrom.h"
#if PLATFORM == PLATFORM_WIN32
	#define MY_HANDLE void*
	#define MY_API  __stdcall
	#define MY_MUTEX_HANDLE void*
	#define MY_COND_HANDLE void* 
	#define MY_RW_LOCK_HANDLE void*
#else 
	#define MY_HANDLE pthread_t // ����windows �ǽ� HANDLE����Ϊ int
	#define MY_API
	#define MY_MUTEX_HANDLE pthread_mutex_t
	#define MY_COND_HANDLE pthread_cond_t
	#define MY_RW_LOCK_HANDLE pthread_rwlock_t 
#endif
#include <list>
#include <vector>
#include <string>
/**
 * �����༯��
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
		* \brief ���캯��������һ�����������
		*
		*/
		Mutex();

		/**
		* \brief ��������������һ�����������
		*
		*/
		~Mutex();

		/**
		* \brief ����һ��������
		*
		*/
		inline void lock();

		/**
		* \brief ����һ��������
		*
		*/
		inline void unlock();

		MY_MUTEX_HANDLE	 m_hMutex;    /**< ϵͳ������ */
	};

	/**
	* \brief Wrapper
	* �����ڸ��Ӻ���������ʹ��
	*/
	class Mutex_scope_lock
	{
	public:
		/**
		* \brief ���캯��
		* ��������lock����
		* \param m ��������
		*/
		Mutex_scope_lock(Mutex &m);
		/**
		* \brief ��������
		* ��������unlock����
		*/
		~Mutex_scope_lock();
	private:

		/**
		* \brief ��������
		*/
		Mutex &mlock;
	};
	/**
	* \brief ��װ��ϵͳ��д����ʹ����Ҫ�򵥣�ʡȥ���ֹ���ʼ��������ϵͳ��д���Ĺ�������Щ�����������ɹ��캯���������������Զ����
	*
	*/
	class RWLock
	{

	public:
		/**
		* \brief ���캯�������ڴ���һ����д��
		*
		*/
		RWLock();
		/**
		* \brief ������������������һ����д��
		*
		*/
		~RWLock();
		/**
		* \brief �Զ�д�����ж���������
		*
		*/
		inline void rdlock();
		/**
		* \brief �Զ�д������д��������
		*
		*/
		inline void wrlock();
		/**
		* \brief �Զ�д�����н�������
		*
		*/
		inline void unlock();
	private:
		MY_RW_LOCK_HANDLE m_hMutex;    /**< ϵͳ��д�� */
		//��д��������
		unsigned int rd_count;
		unsigned int wr_count;
	};
	/**
	* \brief rdlock Wrapper
	* �����ڸ��Ӻ����ж�д����ʹ��
	*/
	class RWLock_scope_rdlock
	{

	public:

		/**
		* \brief ���캯��
		* ��������rdlock����
		* \param m ��������
		*/
		RWLock_scope_rdlock(RWLock &m);
		/**
		* \brief ��������
		* ��������unlock����
		*/
		~RWLock_scope_rdlock();
	private:

		/**
		* \brief ��������
		*/
		RWLock &rwlock;

	};

	/**
	* \brief wrlock Wrapper
	* �����ڸ��Ӻ����ж�д����ʹ��
	*/
	class RWLock_scope_wrlock
	{

	public:

		/**
		* \brief ���캯��
		* ��������wrlock����
		* \param m ��������
		*/
		RWLock_scope_wrlock(RWLock &m);

		/**
		* \brief ��������
		* ��������unlock����
		*/
		~RWLock_scope_wrlock();
	private:

		/**
		* \brief ��������
		*/
		RWLock &rwlock;

	};
	/**
	* \brief ��װ��ϵͳ����������ʹ����Ҫ�򵥣�ʡȥ���ֹ���ʼ��������ϵͳ���������Ĺ�������Щ�����������ɹ��캯���������������Զ����
	*
	*/
	class Cond 
	{
	public:
		/**
		* \brief ���캯�������ڴ���һ����������
		*
		*/
		Cond();
		/**
		* \brief ������������������һ����������
		*
		*/
		~Cond();

		/**
		* \brief �����еȴ���������������̹߳㲥�����źţ�ʹ��Щ�߳��ܹ���������ִ��
		*
		*/
		void broadcast();

		/**
		* \brief �����еȴ���������������̷߳����źţ�ʹ��Щ�߳��ܹ���������ִ��
		*
		*/
		void signal();

		/**
		* \brief �ȴ��ض���������������
		*
		*
		* \param m_hMutex ��Ҫ�ȴ��Ļ�����
		*/
		void wait(Mutex &mutex);
	private:
		MY_COND_HANDLE m_hEvent;    /**< ϵͳ�������� */
	};

	/**
	* \brief ��װ���̲߳���������ʹ���̵߳Ļ���
	*
	*/
	class Thread
	{
	public:

		/**
		* \brief ���캯��������һ������
		*
		* \param name �߳�����
		* \param joinable ��������߳��˳���ʱ���Ƿ񱣴�״̬�����Ϊtrue��ʾ�߳��˳�����״̬�����򽫲������˳�״̬
		*/
		Thread(const std::string &name = std::string("zThread"),const bool joinable = true);

		/**
		* \brief ������������������һ�����󣬻��ն���ռ�
		*
		*/
		virtual ~Thread();

		/**
		* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬��
		*
		*
		* \param sec ָ����ʱ�䣬��
		*/
		static void sleep(const long sec);

		/**
		* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬����
		*
		*
		* \param msec ָ����ʱ�䣬����
		*/
		static void msleep(const long msec);

		/**
		* \brief ʹ��ǰ�߳�˯��ָ����ʱ�䣬΢��
		*
		*
		* \param usec ָ����ʱ�䣬΢��
		*/
		static void usleep(const long usec);

		/**
		* \brief �߳��Ƿ���joinable��
		*
		*
		* \return joinable
		*/
		const bool isJoinable() const;

		/**
		* \brief ����߳��Ƿ�������״̬
		*
		* \return �߳��Ƿ�������״̬
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
		* \brief ���������߳�
		*
		* ��ʵֻ�����ñ�ǣ���ô�̵߳�run���ص�ѭ���ؼ�������ǣ�����������Ѿ����ã����˳�ѭ��
		*
		*/
		void final();
		/**
		* \brief �ж��߳��Ƿ����������ȥ
		*
		* ��Ҫ����run()����ѭ���У��ж�ѭ���Ƿ����ִ����ȥ
		*
		* \return �߳����ص��Ƿ����ִ��
		*/
		const bool isFinal() const;
		/**
		* \brief ���鹹�������߳����ص�������ÿ����Ҫʵ��������������Ҫ�����������
		*
		* ���������ѭ����Ҫ��ÿ��ѭ������߳��˳����isFinal()�������ܹ���֤�̰߳�ȫ�˳�
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
		* \brief �����߳�����
		*
		* \return �߳�����
		*/
		const std::string &getThreadName() const;

	public:
		std::string threadName;      /**< �߳����� */
		Mutex mlock;          /**< ������ */
		volatile bool alive;      /**< �߳��Ƿ������� */
		volatile bool complete;
		MY_HANDLE m_hThread;        /**< �̱߳�� */
		bool joinable;          /**< �߳����ԣ��Ƿ�����joinable��� */
		Cond cond;
	}; 

	/**
	* \brief ���߳̽��з���������
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

		typedef std::vector<Thread *> Container;  /**< �������� */

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

		Container vts;                /**< �߳����� */
		RWLock rwlock;                /**< ��д�� */
	};
};
