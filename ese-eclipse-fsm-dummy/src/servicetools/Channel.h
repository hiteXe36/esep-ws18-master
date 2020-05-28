#pragma once

#include "Message.h"
#include "Semaphore.h"

#include <queue>
#include <iostream>
#include <thread>
#include <cstddef>

class Channel
{
public:
	Channel(const size_t max_size)
			: sem_free_spaces_
			{ max_size }, sem_size_
			{ }, queue_
			{ }, mtx_
			{ }
	{
	}

	virtual ~Channel()
	{
	}

	auto size(void) const
	{
		return sem_size_.get_value();
	}

	auto max_size(void) const
	{
		return sem_free_spaces_.get_value() + sem_size_.get_value();
	}

	inline Message dequeue(void)
	{
		sem_size_.wait();
		mtx_.lock();
		auto return__ = queue_.front();
		queue_.pop();
		mtx_.unlock();
		sem_free_spaces_.post();
		return return__;
	}

	inline void enqueue(const Message element)
	{
		sem_free_spaces_.wait();
		mtx_.lock();
		queue_.push(element);
		mtx_.unlock();
		sem_size_.post();
	}
	void operator<<(const Message element)
	{
		enqueue(element);
	}

	Message friend operator<<(Message& target, Channel& source)
	{
		return target = source.dequeue();
	}

	void friend operator<<(Channel& target, Channel& source)
	{
		target.enqueue(source.dequeue());
	}

	void destroy()
	{
		sem_free_spaces_.destroy();
		sem_size_.destroy();
	}

private:
	Semaphore sem_free_spaces_, sem_size_;
	std::queue<Message> queue_;
	std::mutex mtx_;
};
