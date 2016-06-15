///////////////////////////////////////////
// http://progsch.net/wordpress/?p=81

#include <boost/thread.hpp>
#include <deque>

class ThreadPool;

// our worker thread objects
class Worker {
public:
	Worker(ThreadPool &s) : pool(s) { }
	void operator()();
private:
	ThreadPool &pool;
};

// the actual thread pool
class ThreadPool {
public:
	ThreadPool(size_t);
	
	template<class F>
	void enqueue(F f);

	template <class F, class C>
	void enqueue(F f, C* i);

	bool done() { return active == 0; }

	~ThreadPool();
private:
	friend class Worker;

	// need to keep track of threads so we can join them
	std::vector< boost::thread > workers;

	// the task queue
	std::deque< std::function<void()> > tasks;

	// synchronization
	boost::mutex queue_mutex;
	boost::condition_variable condition;
	bool stop;

	size_t active;
};

void Worker::operator()()
{
	std::function<void()> task;
	while(true)
	{
		{
			// acquire lock
			boost::unique_lock<boost::mutex> 
				lock(pool.queue_mutex);

			// look for a work item
			while(!pool.stop && pool.tasks.empty())
			{ // if there are none wait for notification
				pool.condition.wait(lock);
			}

			if(pool.stop) // exit if the pool is stopped
				return;

			// get the task from the queue
			task = pool.tasks.front();
			pool.tasks.pop_front();

		}   // release lock

		++pool.active;

		// execute the task
		task();

		--pool.active;
	}
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
	:   stop(false), active(0)
{
	for(size_t i = 0; i < threads;++i)
		workers.push_back(boost::thread(Worker(*this)));
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
	// stop all threads
	stop = true;
	condition.notify_all();

	// join them
	for(size_t i = 0;i<workers.size();++i)
		workers[i].join();
}

// add new work item to the pool
template<class F>
void ThreadPool::enqueue(F f)
{
	{ // acquire lock
		boost::unique_lock<boost::mutex> lock(queue_mutex);

		// add the task
		tasks.push_back(std::function<void()>(f));
	} // release lock

	// wake up one thread
	condition.notify_one();
}

// add new work item to the pool
template<class F, class C>
void ThreadPool::enqueue(F f, C* i)
{
	{ // acquire lock
		boost::unique_lock<boost::mutex> lock(queue_mutex);

		// add the task
		tasks.push_back(std::bind(f, i));
	} // release lock

	// wake up one thread
	condition.notify_one();
}
