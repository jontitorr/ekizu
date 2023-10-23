#ifndef EKIZU_THREAD_POOL_HPP
#define EKIZU_THREAD_POOL_HPP

#include <condition_variable>
#include <ekizu/util.hpp>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ekizu
{
/**
 * @brief A thread pool. Spin up a number of threads and enqueue tasks to be run
 * on them.
 */
struct ThreadPool {
	explicit ThreadPool(size_t threads = std::thread::hardware_concurrency())
	{
		for (size_t i{}; i < threads; ++i) {
			m_workers.emplace_back([this] {
				while (true) {
					std::function<void()> task;

					{
						std::unique_lock lk{ m_mtx };
						m_cv.wait(lk, [this] {
							return m_stop ||
							       !m_tasks.empty();
						});

						if (m_stop && m_tasks.empty()) {
							return;
						}

						task = std::move(
							m_tasks.front());
						m_tasks.pop();
					}

					task();
				}
			});
		}
	}

	ThreadPool(const ThreadPool &) = delete;
	ThreadPool &operator=(const ThreadPool &) = delete;
	ThreadPool(ThreadPool &&) = delete;
	ThreadPool &operator=(ThreadPool &&) = delete;

	~ThreadPool()
	{
		{
			std::scoped_lock lk{ m_mtx };
			m_stop = true;
		}

		m_cv.notify_all();

		for (auto &worker : m_workers) {
			if (worker.joinable()) {
				worker.join();
			}
		}
	}

	/**
     * @brief Enqueue a task to be run on a thread in the pool.
     *
     * @tparam F The type of the function to be run.
     * @tparam Args The types of the arguments to be passed to the function.
     * @param f The function to be run.
     * @param args The arguments to be passed to the function.
     * @return Result<typename std::future<std::invoke_result_t<F, Args...>>> A
     * future which can be used to get the return value of the function, or an
     * error if the thread pool is stopped.
     */
	template <typename F, typename... Args>
	Result<typename std::future<std::invoke_result_t<F, Args...> > >
	enqueue(F &&f, Args &&...args)
	{
		using ReturnType = std::invoke_result_t<F, Args...>;

		auto task = std::make_shared<std::packaged_task<ReturnType()> >(
			std::bind(std::forward<F>(f),
				  std::forward<Args>(args)...));

		auto res = task->get_future();

		{
			std::scoped_lock lk{ m_mtx };

			if (m_stop) {
				return tl::make_unexpected(std::make_error_code(
					std::errc::operation_canceled));
			}

			m_tasks.emplace([task] { (*task)(); });
		}

		m_cv.notify_one();
		return res;
	}

    private:
	std::mutex m_mtx;
	std::condition_variable m_cv;
	bool m_stop{};
	std::queue<std::function<void()> > m_tasks;
	std::vector<std::thread> m_workers;
};
} // namespace ekizu

#endif // EKIZU_THREAD_POOL_HPP
