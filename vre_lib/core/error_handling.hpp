#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <qlogging.h>

// remove to disable error checks
#define VRE_DEBUG

#ifdef VRE_DEBUG
// remove a single line to remove single checks
#define VRE_NULL_PTR_CHECKS
#define VRE_NULL_CHECKS
#define VRE_BOOL_CHECKS
#define VRE_VULKAN_RESULT_CHECKS
#define VRE_GENERAL_ERRORS
#endif


namespace vre
{
	void log(const std::string& msg);
    void err(const std::string& error);
	void err(const std::string& title, const std::string& error);
	void checkVk(VkResult result, const std::string& error);
	void messageBox(const std::string& title,const std::string& text);

	template<typename T>
	void checkPtr(T& ptr, const std::string& error)
	{
#ifdef VRE_NULL_PTR_CHECKS
		if (ptr == nullptr) {
			err("Nullpointer!", error);
		}
#endif
	}
	
	/// @brief Returns an error if the boolean equals false.
	/// @param b - The boolean.
	/// @param error - The error message.
	void check(bool b, const std::string& error);

	/// @brief Returns the error msg if the value == NULL.
	/// @tparam T - Type of the value to check. Must be comparable to NULL/0.
	/// @param value - The value to compare.
	/// @param error - The message forwarded in case value == NULL.
	template<typename T>
	void check(T& value, const std::string& error)
	{
#ifdef VRE_NULL_CHECKS
		if (value == NULL) {
			err("Check failed!", error);
		}
#endif
	}

	namespace Logging
	{
		/// @brief Logfile holds the pointer to a log file opened by ActivateLogging. All qt debug messages will be added to this file.
		/// This file isn't closed on purpose, to ensure all messages are catched. (Especially those errors created on object destruction)
		inline std::ofstream* Logfile{};
		void ActivateLogging();
		void QtMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

		/// @brief A little timer class to calculate benchmarks. Usage: Create a timer object upon start and call one of the time functions
		/// in the desired precision to get the time since object creation.
		class Timer
		{
			using clock_t = std::chrono::high_resolution_clock;
		public:
			/// @brief Time since object creation in seconds.
			auto Seconds() { return std::chrono::duration_cast<std::chrono::seconds>(clock_t::now() - m_timestamp).count(); }

			/// @brief Time since object creation in milliseconds.
			auto Milliseconds() { return std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_timestamp).count(); }

			/// @brief Time since object creation in microseconds.
			auto Microseconds() { return std::chrono::duration_cast<std::chrono::microseconds>(clock_t::now() - m_timestamp).count(); }

			/// @brief Time since object creation in nanoseconds.
			auto Nanoseconds() { return std::chrono::duration_cast<std::chrono::nanoseconds>(clock_t::now() - m_timestamp).count(); }

			/// @brief Reset timestamp, for example to use object mutliple times.
			void Reset() { m_timestamp = clock_t::now(); }

		private:
			std::chrono::time_point<clock_t> m_timestamp{ clock_t::now() };
		};
	}
}