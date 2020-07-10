#pragma once
#ifndef WATCHDOGTIMER_H
#define WATCHDOGTIMER_H
#include <chrono>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <functional>


template<typename Clock>
class BaseWatchdogTimer
{
private:
	typedef std::chrono::milliseconds Millseconds;

private:
	std::mutex mutex;
	std::condition_variable sleep_condition;
	std::atomic_bool exit_flag;
	std::thread thread;
	typename Clock::time_point clock_counter;

private:
	BaseWatchdogTimer(const BaseWatchdogTimer<Clock>&) = delete;
	BaseWatchdogTimer(BaseWatchdogTimer<Clock>&&) = delete;
	BaseWatchdogTimer<Clock>& operator=(const BaseWatchdogTimer<Clock>&) = delete;
	BaseWatchdogTimer<Clock>& operator=(BaseWatchdogTimer<Clock>&&) = delete;

private:
	void forever(Millseconds timeout, bool loop)
	{
		std::unique_lock<std::mutex> lock(this->mutex);

		do
		{
			do
			{
				// 'std::condition_variable::wait_for' may return no_timeout although time exceeds timeout
				// So, must use another variable like 'exit_flag'
				if (this->sleep_condition.wait_for(lock, timeout, [=]() {return this->exit_flag.load(); }))
				{
					if (this->exit_flag.load())
						goto out;
				}
			} while (std::chrono::duration_cast<Millseconds>((Clock::now() - this->clock_counter)) < timeout);
			this->timeout();
		} while (loop);

	out:
		return;
	}

private:
	virtual void timeout()
	{

	}

public:
	BaseWatchdogTimer()
	{

	}

	virtual ~BaseWatchdogTimer()
	{
		stop();
	}

public:
	void kick(unsigned int timeout_milliseconds, bool loop = false)
	{
		kick(Millseconds(timeout_milliseconds), loop);
	}

	void kick(std::chrono::milliseconds timeout, bool loop = false)
	{
		stop();
		thread = std::thread(std::bind(&BaseWatchdogTimer<Clock>::forever, this, timeout, loop));
	}

	void clear()
	{
		this->clock_counter = Clock::now();
	}

	void stop()
	{
		exit_flag.store(true);
		sleep_condition.notify_all();

		if (thread.joinable())
		{
			thread.join();
		}

		exit_flag.store(false);
	}
};

typedef std::chrono::high_resolution_clock HighResolutionClock;
typedef std::chrono::system_clock SystemClock;
typedef HighResolutionClock DefaultClock;


typedef BaseWatchdogTimer<DefaultClock> WatchdogTimer;
typedef BaseWatchdogTimer<HighResolutionClock> HighResolutionWatchdogTimer;
typedef BaseWatchdogTimer<SystemClock> SystemClockWatchdogTimer;
#endif
