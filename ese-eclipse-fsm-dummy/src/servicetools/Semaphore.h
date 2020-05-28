#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
	Semaphore(const size_t count = 0)
			: count_(count), condition_(), mtx_()
	{
	}

	virtual ~Semaphore()
	{
	}
	Semaphore(const Semaphore&) = delete;
	Semaphore& operator=(const Semaphore&) = delete;
	Semaphore(Semaphore&&) = delete;
	Semaphore& operator=(Semaphore&&) = delete;

	inline void post(void)
	{
		std::lock_guard < std::mutex > lock(mtx_);
		count_++;
		condition_.notify_one();
	}

	inline void wait(void)
	{
		std::unique_lock < std::mutex > lock(mtx_);
		condition_.wait(lock, [&]
		{	return count_>0;});
		count_--;
	}

	inline bool try_wait(void)
	{
		std::unique_lock < std::mutex > lock(mtx_);
		if (count_ > 0)
		{
			count_--;
			return true;
		}
		return false;
	}

	inline size_t get_value() const
	{
		return count_;
	}

	void destroy()
	{
		condition_.notify_all();
	}
private:
	size_t count_;
	std::condition_variable condition_;
	std::mutex mtx_;
};
