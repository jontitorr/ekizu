#ifndef EKIZU_EVENT_EMITTER_HPP
#define EKIZU_EVENT_EMITTER_HPP

#include <functional>
#include <list>
#include <mutex>

namespace ekizu {
/**
 * @brief A simple event emitter that can be used to emit events to listeners.
 *
 * @tparam Args The types of the arguments that will be passed to the listeners.
 */
template <typename... Args>
struct EventEmitter {
	/**
	 * @brief Add a listener to the emitter. The listener will be called when
	 * the emitter emits an event.
	 *
	 * @tparam Callback The type of the callback.
	 * @param callback The callback to add.
	 */
	template <typename Callback>
	void add_listener(Callback &&callback) {
		std::scoped_lock lk{m_mtx};
		m_callbacks.emplace_back(std::forward<Callback>(callback));
	}

	/**
	 * @brief Remove a listener from the emitter. The listener will no longer be
	 * called when the emitter emits an event.
	 */
	void pop() {
		std::scoped_lock lk{m_mtx};
		m_callbacks.pop_back();
	}

	/**
	 * @brief Remove all listeners from the emitter.
	 */
	void clear() {
		std::scoped_lock lk{m_mtx};
		m_callbacks.clear();
	}

	/**
	 * @brief Emit an event to all listeners.
	 *
	 * @param args The arguments to be passed to the listeners.
	 */
	template <typename... ForwardArgs,
			  typename std::enable_if_t<std::conjunction_v<
				  std::is_convertible<ForwardArgs, Args>...> > * = nullptr>
	void emit(ForwardArgs &&...args) {
		std::list<std::function<void(Args...)> > callbacks;

		{
			std::scoped_lock lk{m_mtx};
			callbacks = m_callbacks;
		}

		for (auto &callback : callbacks) {
			callback(std::forward<ForwardArgs>(args)...);
		}
	}

   private:
	std::list<std::function<void(Args...)> > m_callbacks;
	std::mutex m_mtx;
};
}  // namespace ekizu

#endif	// EKIZU_EVENT_EMITTER_HPP
