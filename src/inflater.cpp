#include <zlib.h>

#include <ekizu/inflater.hpp>

namespace {
template <typename T>
struct DeleterOf;

template <>
struct DeleterOf<z_stream> {
	void operator()(z_stream *stream) const {
		inflateEnd(stream);
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete stream;
	}
};

using UniqueZStream = std::unique_ptr<z_stream, DeleterOf<z_stream> >;
}  // namespace

namespace ekizu {
struct Inflater::Impl {
	explicit Impl(UniqueZStream stream_) : stream{std::move(stream_)} {}

	UniqueZStream stream;
};

[[nodiscard]] Result<Inflater> Inflater::create() {
	UniqueZStream stream{new z_stream{}};

#ifndef _WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
	if (inflateInit(stream.get()) != Z_OK) {
		return boost::system::errc::not_supported;
	}
#ifndef _WIN32
#pragma GCC diagnostic pop
#endif

	return Inflater{std::make_unique<Impl>(Impl{std::move(stream)})};
}

Inflater::Inflater(std::unique_ptr<Impl> impl) : m_impl{std::move(impl)} {}

Inflater::Inflater(Inflater &&) noexcept = default;
Inflater &Inflater::operator=(Inflater &&) noexcept = default;
Inflater::~Inflater() = default;

Result<std::string> Inflater::inflate(boost::span<const char> data) {
	m_impl->stream->avail_in = static_cast<uint32_t>(data.size());
	m_impl->stream->next_in =
		reinterpret_cast<uint8_t *>(const_cast<char *>(data.data()));

	std::string result;
	result.reserve(data.size() * 2);

	do {
		m_impl->stream->avail_out = static_cast<uint32_t>(m_buffer.size());
		m_impl->stream->next_out = reinterpret_cast<uint8_t *>(m_buffer.data());

		if (const auto res = ::inflate(m_impl->stream.get(), Z_NO_FLUSH);
			res != Z_OK) {
			inflateReset(m_impl->stream.get());
			return boost::system::errc::not_supported;
		}

		const auto have = m_buffer.size() - m_impl->stream->avail_out;
		result.insert(result.end(), m_buffer.begin(), m_buffer.begin() + have);
	} while (m_impl->stream->avail_out == 0);

	return result;
}
}  // namespace ekizu