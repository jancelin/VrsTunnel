#ifndef ASYNCHRONOUS_INPUT_OUTPUT_HPP_
#define ASYNCHRONOUS_INPUT_OUTPUT_HPP_

#include <aio.h>
#include <memory>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>


namespace VrsTunnel::Ntrip
{
	/**
	 * Input/output operation status
	 */
	enum class io_status { InProgress, Error, Success };

	/**
	 * Asyncronous input/output operations based on file descriptor.
	 * Copy and move operations are disabled.
	 */
	class async_io
	{
		async_io(const async_io&)               = delete;
		async_io& operator=(const async_io&)    = delete;
		async_io& operator=(async_io&&)         = delete;

		/**
		 * Control block of asyncronous operation
		 * (pragma disables zero size array warning in struct aiocb)
		 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
		struct aiocb m_read_cb;
#pragma GCC diagnostic pop

		std::unique_ptr<char[]> m_data{}; /**< Buffer for transmission */

	public:
		async_io(async_io&&)                    = default;

		/**
		 * Init asyncronous operation fields
		 */
		explicit async_io(int sockfd) noexcept;

		/**
		 * @return current status of the transmission
		 */
		[[nodiscard]] io_status check() noexcept;

		/**
		 * Assyncronous write operation
		 * @param data buffer to be transmitted
		 * @param size buffer size
		 * @return status of async request
		 */
		[[nodiscard]] io_status write(const char* data, std::size_t size);

		/**
		 * @return amount of received bytes
		 */
		int available() noexcept;

		/**
		 * Method to read available bytes
		 * @param size number of bytes to read
		 * @return buffer of received bytes
		 */
		std::unique_ptr<char[]> read(std::size_t size);

		/**
		 * Completes asyncronous operation
		 * @return status associated with AIOCBP.
		 * It should be called after status is not
		 * 'InProgress' any more.
		 */
		[[nodiscard]] ssize_t end() noexcept;

		void close() noexcept;
	};
}

#endif /* ASYNCHRONOUS_INPUT_OUTPUT_HPP_ */
