#ifndef _CryThread_pthreads_h_
#define _CryThread_pthreads_h_ 1

#include <ISystem.h>
#include <ILog.h>

#if defined(PS3)
	#include <time.h>
	#include <sys/ppu_thread.h>
	#include <sys/synchronization.h>
	#include <sys/time_util.h>
	#include <sys/sys_time.h>
	#define PTHREAD_MUTEX_FAST_NP PTHREAD_MUTEX_NORMAL
#endif


#if defined(WIN32)
	#include <pthreads_win32.h>
#else
	#include <sys/types.h>
	#include <sys/time.h>
	#include <pthread.h>
	#include <sched.h>
#endif

template<class LockClass> class _PthreadCond;
template<int PthreadMutexType> class _PthreadLockBase;

template<int PthreadMutexType> class _PthreadLockAttr
{
	friend class _PthreadLockBase<PthreadMutexType>;

	_PthreadLockAttr(int dummy)
	{
		pthread_mutexattr_init(&m_Attr);
		pthread_mutexattr_settype(&m_Attr, PthreadMutexType);
	}
	~_PthreadLockAttr()
	{
		pthread_mutexattr_destroy(&m_Attr);
	}

protected:
  pthread_mutexattr_t m_Attr;
};

template<int PthreadMutexType> class _PthreadLockBase
{
	static _PthreadLockAttr<PthreadMutexType> m_Attr;

protected:
	static pthread_mutexattr_t &GetAttr() { return m_Attr.m_Attr; }
};

template<class LockClass, int PthreadMutexType> class _PthreadLock
  : public _PthreadLockBase<PthreadMutexType>
{
  friend class _PthreadCond<LockClass>;

//#if defined(_DEBUG)
public:
//#endif
  pthread_mutex_t m_Lock;

public:
  _PthreadLock()
	{
		pthread_mutex_init(
				&m_Lock,
				&_PthreadLockBase<PthreadMutexType>::GetAttr());
	}
  ~_PthreadLock() { pthread_mutex_destroy(&m_Lock); }

  void Lock() { pthread_mutex_lock(&m_Lock); }
	bool TryLock() { return pthread_mutex_trylock(&m_Lock) != EBUSY; }
  void Unlock() { pthread_mutex_unlock(&m_Lock); }

	// Get the POSIX pthread_mutex_t.
	// Warning:
	// This method will not be available in the Win32 port of CryThread.
	pthread_mutex_t &Get_pthread_mutex_t() { return m_Lock; }

#ifndef NDEBUG
	bool IsLocked()
	{
#if defined(WIN32)
		return pthread_mutex_islocked_np(&m_Lock);
#elif defined(LINUX)
		// FIXME implement
		return true;
#else
		return true;
#endif
	}
#endif
};

#if !defined(PS3)
	#if defined(_DEBUG) && defined(PTHREAD_MUTEX_ERRORCHECK_NP)
	template<> class CryLock<CRYLOCK_FAST>
		: public _PthreadLock<CryLock<CRYLOCK_FAST>, PTHREAD_MUTEX_ERRORCHECK_NP>
	#else
	template<> class CryLock<CRYLOCK_FAST>
		: public _PthreadLock<CryLock<CRYLOCK_FAST>, PTHREAD_MUTEX_FAST_NP>
	#endif
	{
		CryLock(const CryLock<CRYLOCK_FAST>&);
		void operator = (const CryLock<CRYLOCK_FAST>&);

	public:
		CryLock() { }
	};
#endif//PS3

template<> class CryLock<CRYLOCK_RECURSIVE>
  : public _PthreadLock<CryLock<CRYLOCK_RECURSIVE>, PTHREAD_MUTEX_RECURSIVE>
{
	CryLock(const CryLock<CRYLOCK_RECURSIVE>&);
	void operator = (const CryLock<CRYLOCK_RECURSIVE>&);

public:
	CryLock() { }
};

#if !defined(PS3)
	template<> class CryCondLock<CRYLOCK_FAST> : public CryLock<CRYLOCK_FAST> {};
#endif
template<> class CryCondLock<CRYLOCK_RECURSIVE> : public CryLock<CRYLOCK_RECURSIVE> {};

template<class LockClass> class _PthreadCond
{
  pthread_cond_t m_Cond;

public:
  _PthreadCond() { pthread_cond_init(&m_Cond, NULL); }
  ~_PthreadCond() { pthread_cond_destroy(&m_Cond); }
  void Notify() { pthread_cond_broadcast(&m_Cond); }
  void NotifySingle() { pthread_cond_signal(&m_Cond); }
  void Wait(LockClass &Lock) { pthread_cond_wait(&m_Cond, &Lock.m_Lock); }
	bool TimedWait(LockClass &Lock, uint32 milliseconds)
	{
#if defined(WIN32)
		// Our pthreads_win32 library has an extension supporting relative timed
		// waits.  We'll use that.
		int err;
		
		while (true)
		{
			err = pthread_cond_timedwait_rel(&m_Cond, &Lock.m_Lock, milliseconds);
			// Note: EINTR can't occur for the pthreads_win32 implementation.
			if (err == ETIMEDOUT)
			{
				return false;
			}
			else
				assert(err == 0);
			break;
		}
		return true;
#else
		struct timeval now;
    struct timespec timeout;
    int err;

    gettimeofday(&now, NULL);
		while (true)
		{
			timeout.tv_sec = now.tv_sec + milliseconds / 1000;
			uint64 nsec = (uint64)now.tv_usec * 1000 + (uint64)milliseconds * 1000000;
			if (nsec >= 1000000000)
			{
				timeout.tv_sec += (long)(nsec / 1000000000);
				nsec %= 1000000000;
			}
			timeout.tv_nsec = (long)nsec;
			err = pthread_cond_timedwait(&m_Cond, &Lock.m_Lock, &timeout);
			if (err == EINTR)
			{
				// Interrupted by a signal.
				continue;
			}
			else if (err == ETIMEDOUT)
			{
				return false;
			}
			else
				assert(err == 0);
			break;
		}
		return true;
#endif
	}

	// Get the POSIX pthread_cont_t.
	// Warning:
	// This method will not be available in the Win32 port of CryThread.
	pthread_cond_t &Get_pthread_cond_t() { return m_Cond; }
};

template<class LockClass> class _CryCondWithLock
  : public _PthreadCond<LockClass>
{
  LockClass m_Lock;

public:
  void Wait() { _PthreadCond<LockClass>::Wait(m_Lock); }
	void Wait(LockClass &Lock) { _PthreadCond<LockClass>::Wait(Lock); }
	bool TimedWait(uint32 milliseconds)
	{
		return _PthreadCond<LockClass>::Wait(m_Lock, milliseconds);
	}
	bool TimedWait(LockClass &Lock, uint32 milliseconds)
	{
		return _PthreadCond<LockClass>::Wait(Lock, milliseconds);
	}
  void Lock() { m_Lock.Lock(); }
	bool TryLock() { return m_Lock.TryLock(); }
  void Unlock() { m_Lock.Unlock(); }
  LockClass &GetLock() { return m_Lock; }
};

template<class LockClass> class CryCond<LockClass, false>
  : public _PthreadCond<LockClass>
{
	CryCond(const CryCond<LockClass, false>&);
	void operator = (const CryCond<LockClass, false>&);

public:
	CryCond() { }
};

template<class LockClass> class CryCond<LockClass, true>
  : public _CryCondWithLock<LockClass>
{
	CryCond(const CryCond<LockClass, true>&);
	void operator = (const CryCond<LockClass, true>&);

public:
	CryCond() { }
};

class _PthreadRWLock
{
	pthread_rwlock_t m_Lock;

public:
	_PthreadRWLock() { pthread_rwlock_init(&m_Lock, NULL); }
	~_PthreadRWLock() { pthread_rwlock_destroy(&m_Lock); }
	void RLock() { pthread_rwlock_rdlock(&m_Lock); }
	bool TryRLock() { return pthread_rwlock_tryrdlock(&m_Lock) != EBUSY; }
	void WLock() { pthread_rwlock_wrlock(&m_Lock); }
	bool TryWLock() { return pthread_rwlock_trywrlock(&m_Lock) != EBUSY; }
	void Lock() { WLock(); }
	bool TryLock() { return TryWLock(); }
	void Unlock() { pthread_rwlock_unlock(&m_Lock); }
};

class CrySimpleThreadSelf
{
protected:
#if defined(LINUX)
	static THREADLOCAL CrySimpleThreadSelf *m_Self;
#endif
};

template<class Runnable>
class CrySimpleThread
	: public CryRunnable,
		protected CrySimpleThreadSelf
{
public:
	typedef void (*ThreadFunction)(void *);

private:
#if !defined(NO_THREADINFO)
	CryThreadInfo m_Info;
#endif
  pthread_t m_ThreadID;
	unsigned m_CpuMask;
	Runnable *m_Runnable;
	struct
	{
		ThreadFunction m_ThreadFunction;
		void *m_ThreadParameter;
	} m_ThreadFunction;
	bool m_bIsStarted;
	bool m_bIsRunning;

protected:
	virtual void Terminate()
	{
		// This method must be empty.
		// Derived classes overriding Terminate() are not required to call this
		// method.
	}

private:
#if !defined(NO_THREADINFO)
	static void SetThreadInfo(CrySimpleThread<Runnable> *self)
	{
		pthread_t thread = pthread_self();
  #if defined(WIN32)
		// This is evil.  Would be better to extend the pthreads_win32 API for
		// getting the thread ID.  However, pthreads_win32 is a temorary solution
		// anyway, so we'll go with this hack until the native implementation is
		// done.
		struct ptw32_thread_struct
		{
			DWORD ID;
			HANDLE Handle;
		} *Thread = reinterpret_cast<ptw32_thread_struct *>(thread.p);
		self->m_Info.m_ID = Thread->ID;
		// Set the thread name.
		self->SetName(NULL);
  #else
		self->m_Info.m_ID = (uint32)thread;
  #endif
	}
#else
	static void SetThreadInfo(CrySimpleThread<Runnable> *self) { }
#endif

	static void *PthreadRunRunnable(void *thisPtr)
	{
		CrySimpleThread<Runnable> *const self = (CrySimpleThread<Runnable> *)thisPtr;
#if defined(LINUX)
		m_Self = self;
#endif
		self->m_bIsStarted = true;
		self->m_bIsRunning = true;
		SetThreadInfo(self);
		self->m_Runnable->Run();
		self->m_bIsRunning = false;
		self->Terminate();
#if defined(LINUX)
		m_Self = NULL;
#endif
		return NULL;
	}

	static void *PthreadRunThis(void *thisPtr)
	{
		CrySimpleThread<Runnable> *const self = (CrySimpleThread<Runnable> *)thisPtr;
#if defined(LINUX)
		m_Self = self;
#endif
		self->m_bIsStarted = true;
		self->m_bIsRunning = true;
		SetThreadInfo(self);
		self->Run();
		self->m_bIsRunning = false;
		self->Terminate();
#if defined(LINUX)
		m_Self = NULL;
#endif
		return NULL;
	}

	CrySimpleThread(const CrySimpleThread<Runnable>&);
	void operator = (const CrySimpleThread<Runnable>&);

public:
  CrySimpleThread()
		: m_CpuMask(0), m_bIsStarted(false), m_bIsRunning(false)
	{
#if !defined(NO_THREADINFO)
		m_Info.m_Name = "<Thread>";
		m_Info.m_ID = 0;
#endif
		memset(&m_ThreadID, 0, sizeof m_ThreadID);
		m_Runnable = NULL;
	}

  virtual ~CrySimpleThread()
	{
		if (IsStarted())
		{
			// Note: We don't want to cache a pointer to ISystem and/or ILog to
			// gain more freedom on when the threading classes are used (e.g.
			// threads may be started very early in the initialization).
			ISystem *pSystem = GetISystem();
			ILog *pLog = NULL;
			if (pSystem != NULL)
				pLog = pSystem->GetILog();
			if (pLog != NULL)
				pLog->LogError("Runaway thread %s", GetName());
			Cancel();
			Join();
		}
	}

#if !defined(NO_THREADINFO)
	CryThreadInfo &GetInfo() { return m_Info; }
	const char *GetName() { return m_Info.m_Name.c_str(); }

	// Set the name of the called thread.
	//
	// WIN32:
	// If the thread is started, then the VC debugger is informed about the new
	// thread name.  If the thread is not started, then the VC debugger will be
	// informed lated when the thread is started through one of the Start()
	// methods.
	//
	// If the parameter Name is NULL, then the name of the thread is kept
	// unchanged.  This may be used to sent the current thread name to the VC
	// debugger.
	void SetName(const char *Name)
	{
		if (Name != NULL)
		{
			m_Info.m_Name = Name;
		}
#if defined(WIN32)
		if (IsStarted())
		{
			// The VC debugger gets the information about a thread's name through
			// the exception 0x406D1388.
			struct
			{
				DWORD Type;
				const char *Name;
				DWORD ID;
				DWORD Flags;
			} Info = { 0x1000, NULL, 0, 0 };
			Info.ID = (DWORD)m_Info.m_ID;
			__try
			{
				RaiseException(
						0x406D1388, 0, sizeof Info / sizeof(DWORD), (ULONG_PTR *)&Info);
			}
			__except (EXCEPTION_CONTINUE_EXECUTION)
			{
			}
		}
#endif
	}
#else
	CryThreadInfo &GetInfo()
	{
		static CryThreadInfo dummyInfo = { "<dummy>", 0 };
		return dummyInfo;
	}
	const char *GetName() { return "<dummy>"; }
	void SetName(const char *Name) { }
#endif

  virtual void Run()
	{
		// This Run() implementation supports the void StartFunction() method.
		// However, code using this class (or derived classes) should eventually
		// be refactored to use one of the other Start() methods.  This code will
		// be removed some day and the default implementation of Run() will be
		// empty.
		if (m_ThreadFunction.m_ThreadFunction != NULL)
		{
			m_ThreadFunction.m_ThreadFunction(m_ThreadFunction.m_ThreadParameter);
		}
	}

	// Cancel the running thread.
	//
	// If the thread class is implemented as a derived class of CrySimpleThread,
	// then the derived class should provide an appropriate implementation for
	// this method.  Calling the base class implementation is _not_ required.
	//
	// If the thread was started by specifying a Runnable (template argument),
	// then the Cancel() call is passed on to the specified runnable.
	//
	// If the thread was started using the StartFunction() method, then the
	// caller must find other means to inform the thread about the cancellation
	// request.
	virtual void Cancel()
	{
		if (IsStarted() && m_Runnable != NULL)
			m_Runnable->Cancel();
	}

	virtual void Start(Runnable &runnable, unsigned cpuMask = 0)
  {
#if defined(LINUX)
    assert(m_ThreadID == 0);
#endif
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
		m_CpuMask = cpuMask;
#if defined(PTHREAD_NPTL)
		if (cpuMask != ~0 && cpuMask != 0)
		{
			cpu_set_t cpuSet;
			CPU_ZERO(&cpuSet);
			for (int cpu = 0; cpu < sizeof(cpuMask) * 8; ++cpu)
			{
				if (cpuMask & (1 << cpu))
				{
					CPU_SET(cpu, &cpuSet);
				}
			}
			pthread_attr_setaffinity_np(&threadAttr, sizeof cpuSet, &cpuSet);
		}
#endif
		m_Runnable = &runnable;
    int err = pthread_create(
				&m_ThreadID,
				&threadAttr,
				PthreadRunRunnable,
				this);
    assert(err == 0);
  }

	virtual void Start(unsigned cpuMask = 0)
	{
#if defined(LINUX)
    assert(m_ThreadID == 0);
#endif
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
		m_CpuMask = cpuMask;
#if defined(PTHREAD_NPTL)
		if (cpuMask != ~0 && cpuMask != 0)
		{
			cpu_set_t cpuSet;
			CPU_ZERO(&cpuSet);
			for (int cpu = 0; cpu < sizeof(cpuMask) * 8; ++cpu)
			{
				if (cpuMask & (1 << cpu))
				{
					CPU_SET(cpu, &cpuSet);
				}
			}
			pthread_attr_setaffinity_np(&threadAttr, sizeof cpuSet, &cpuSet);
		}
#endif
    int err = pthread_create(
				&m_ThreadID,
				&threadAttr,
				PthreadRunThis,
				this);
    assert(err == 0);
	}

	void StartFunction(
			ThreadFunction threadFunction,
			void *threadParameter = NULL,
			unsigned cpuMask = 0
			)
	{
		m_ThreadFunction.m_ThreadFunction = threadFunction;
		m_ThreadFunction.m_ThreadParameter = threadParameter;
		Start(cpuMask);
	}

#if defined(LINUX)
	static CrySimpleThread<Runnable> *Self()
	{
		return reinterpret_cast<CrySimpleThread<Runnable> *>(m_Self);
	}
#endif

  void Exit()
  {
    assert(m_ThreadID == pthread_self());
		m_bIsRunning = false;
		Terminate();
#if defined(LINUX)
		m_Self = NULL;
#endif
    pthread_exit(NULL);
  }

  void Join()
  {
    int err = pthread_join(m_ThreadID, NULL);
    assert(err == 0);
		m_bIsStarted = false;
		memset(&m_ThreadID, 0, sizeof m_ThreadID);
  }

	unsigned SetCpuMask(unsigned cpuMask)
	{
		int oldCpuMask = m_CpuMask;

		if (cpuMask == m_CpuMask)
			return oldCpuMask;
		m_CpuMask = cpuMask;
#if defined(PTHREAD_NPTL)
		cpu_set_t cpuSet;
		CPU_ZERO(&cpuSet);
		if (cpuMask != ~0 && cpuMask != 0)
		{
			for (int cpu = 0; cpu < sizeof(cpuMask) * 8; ++cpu)
			{
				if (cpuMask & (1 << cpu))
				{
					CPU_SET(cpu, &cpuSet);
				}
			}
		else
		{
			CPU_ZERO(&cpuSet);
			for (int cpu = 0; i < sizeof(cpuSet) * 8; ++cpu)
			{
				CPU_SET(cpu, &cpuSet);
			}
		}
		pthread_attr_setaffinity_np(&threadAttr, sizeof cpuSet, &cpuSet);
#endif
		return oldCpuMask;
	}

	unsigned GetCpuMask() { return m_CpuMask; }

	void Stop()
	{
		m_bIsStarted = false;
	}

	bool IsStarted() { return m_bIsStarted; }
	bool IsRunning() { return m_bIsRunning; }
};

#endif

// vim:ts=2

