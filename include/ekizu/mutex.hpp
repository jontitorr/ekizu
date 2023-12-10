#ifndef EKIZU_MUTEX_HPP
#define EKIZU_MUTEX_HPP

#include <mutex>

namespace ekizu {
template <class T>
struct MutexGuard {
	MutexGuard(T &inner, std::mutex &mtx) : m_inner{inner}, m_mtx{mtx} {
		mtx.lock();
	}

	operator T &() & { return m_inner; }

	~MutexGuard() { m_mtx.unlock(); }

   private:
	T &m_inner;
	std::mutex &m_mtx;
};

template <class T>
struct Mutex {
	Mutex() = default;

	Mutex(T &&inner) : m_inner{std::move(inner)} {}

	Mutex(const T &) = delete;

	MutexGuard<T> lock() { return MutexGuard<T>(m_inner, m_mtx); }

   private:
	std::mutex m_mtx;
	T m_inner;
};
}  // namespace ekizu

#endif	// EKIZU_MUTEX_HPP
