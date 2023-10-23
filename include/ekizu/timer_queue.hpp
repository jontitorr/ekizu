#ifndef EKIZU_TIMER_QUEUE_HPP
#define EKIZU_TIMER_QUEUE_HPP

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace ekizu
{
/**
 * @brief A timer queue. Enqueue tasks to be run at a later time.
 */
struct TimerQueue {
	TimerQueue() = default;
	TimerQueue(const TimerQueue &) = delete;
	TimerQueue &operator=(const TimerQueue &) = delete;
	TimerQueue(TimerQueue &&) = delete;
	TimerQueue &operator=(TimerQueue &&) = delete;
	~TimerQueue()
	{
		stop();
	}

	/**
     * @brief Toggle the repeat mode. If enabled, tasks will be run repeatedly
     * at a given interval.
     *
     * @param repeat_mode Whether to enable repeat mode.
     */
	void set_repeat_mode(bool repeat_mode)
	{
		std::unique_lock lk{ m_mtx };
		m_repeat_mode = repeat_mode;
	}

	/**
     * @brief Start the timer queue. If the queue is already running, this will
     * do nothing.
     */
	void start()
	{
		std::unique_lock lk{ m_mtx };

		if (!m_stop) {
			return;
		}

		m_stop = false;
		m_thread = std::thread{ &TimerQueue::run, this };
	}

	/**
     * @brief Stop the timer queue. If the queue is already stopped, this will
     * do nothing.
     */
	void stop()
	{
		std::unique_lock lk{ m_mtx };

		if (m_stop) {
			return;
		}

		m_stop = true;
		lk.unlock();
		m_cv.notify_all();

		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	/**
     * @brief Add a task to the queue. The task will be run after the given
     * delay.
     *
     * @param task The task to run.
     * @param delay The delay before running the task.
     * @return size_t The id of the task.
     */
	size_t add_task(const std::function<void()> &task,
			const std::chrono::milliseconds &delay = {})
	{
		std::unique_lock lk{ m_mtx };
		const auto id = ++m_task_counter == 0 ? ++m_task_counter :
							m_task_counter;

		m_tasks.push({ id, delay,
			       std::chrono::steady_clock::now() + delay,
			       task });
		lk.unlock();
		m_cv.notify_one();

		return id;
	}

	/**
     * @brief Cancels a task from the queue, if it exists. This will not remove
     * the task from the queue, but will prevent it from being executed. Use
     * remove_task() to remove the task from the queue.
     *
     * @param id The id of the task to cancel.
     */
	void cancel_task(size_t id)
	{
		std::unique_lock lk{ m_mtx };
		auto &tasks = m_tasks.get_container();
		auto task_it = std::find_if(tasks.begin(), tasks.end(),
					    [id](const auto &task) {
						    return task.id == id;
					    });

		if (task_it != tasks.end()) {
			task_it->id = 0;
			task_it->time = std::chrono::steady_clock::now();
		}
	}

	/**
     * @brief Removes a task from the queue, if it exists.
     *
     * @param id The id of the task to remove.
     */
	void remove_task(size_t id)
	{
		std::unique_lock lk{ m_mtx };
		auto &tasks = m_tasks.get_container();
		auto task_it = std::find_if(tasks.begin(), tasks.end(),
					    [id](const auto &task) {
						    return task.id == id;
					    });

		if (task_it != tasks.end()) {
			tasks.erase(task_it);
		}
	}

    private:
	struct Task {
		bool operator>(const Task &other) const
		{
			return time > other.time;
		}

		size_t id;
		std::chrono::milliseconds delay;
		std::chrono::steady_clock::time_point time;
		std::function<void()> func;
	};

	void run()
	{
		std::unique_lock lk{ m_mtx };

		while (!m_stop) {
			if (m_tasks.empty()) {
				m_cv.wait(lk, [this] {
					return !m_tasks.empty() || m_stop;
				});
			} else {
				if (const auto time = m_tasks.top().time;
				    time > std::chrono::steady_clock::now()) {
					m_cv.wait_until(lk, time, [this, time] {
						return time <= std::chrono::steady_clock::
								       now() ||
						       m_stop;
					});
				} else {
					auto task = m_tasks.top_and_pop();

					lk.unlock();

					if (task.id != 0) {
						task.func();
					}

					lk.lock();

					if (m_repeat_mode && task.id != 0) {
						task.time =
							std::chrono::steady_clock::
								now() +
							task.delay;
						m_tasks.push(std::move(task));
					}
				}
			}
		}
	}

	struct Queue
		: std::priority_queue<Task, std::vector<Task>, std::greater<> > {
		std::vector<Task> &get_container()
		{
			return c;
		}

		Task top_and_pop()
		{
			std::pop_heap(c.begin(), c.end(), comp);
			auto t = std::move(c.back());
			c.pop_back();
			return t;
		}
	};

	Queue m_tasks;
	std::thread m_thread;
	std::mutex m_mtx;
	std::condition_variable m_cv;
	bool m_stop{ true };
	bool m_repeat_mode{};
	size_t m_task_counter{};
};
} // namespace ekizu

#endif // EKIZU_TIMER_QUEUE_HPP
