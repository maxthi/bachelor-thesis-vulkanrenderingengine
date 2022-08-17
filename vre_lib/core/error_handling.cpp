#include "error_handling.hpp"
#include <qmessagebox.h>
#include <fstream>
#include <iostream>

namespace vre
{
	void log(const std::string& msg)
	{
		qDebug(msg.c_str());
	}

	void err(const std::string& error)
	{
#ifdef VRE_GENERAL_ERRORS
		err("Error", error);
#endif
	}

	void err(const std::string& title, const std::string& error)
	{
#ifdef VRE_GENERAL_ERRORS
		log(error);
		messageBox(title, error);
		throw std::runtime_error(error);
#endif
	}

	void checkVk(VkResult result, const std::string& error)
	{
#ifdef VRE_VULKAN_RESULT_CHECKS
		if (result != VK_SUCCESS) {
			err("Vulkan check failed!", error);
		}
#endif
	}

	/// @brief If conditon is false, print error.
	/// Think: check whether value is in the state its supposed to be. If its supposed to be true, check(value,""), if supposed to be false, check(!value,"").
	/// @param b - The condition.
	/// @param error - The error message.
	void check(bool b, const std::string& error)
	{
#ifdef VRE_BOOL_CHECKS
		if (!b) {
			err("Check failed!", error);
		}
#endif
	}


	void messageBox(const std::string& title, const std::string& text)
	{
		QMessageBox msgBox;
		msgBox.setFixedWidth(200);
		msgBox.setWindowTitle(title.c_str());
		msgBox.setIcon(QMessageBox::Icon::Critical);
		msgBox.setText(text.c_str());
		msgBox.exec();
	}
}

namespace vre::Logging
{
	void ActivateLogging()
	{
		Logfile = new std::ofstream("latest_error_log.txt");
		qInstallMessageHandler(QtMessageHandler);
	}

	void QtMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
	{
		QByteArray localMsg = msg.toLocal8Bit();
		*Logfile << localMsg.constData() << std::endl;
		std::cout << localMsg.constData() << std::endl;
		// extended message with file
		// *Logfile << context.file << ":" << context.line << ">" << localMsg.constData() << "(" << context.function << ")" << std::endl;
	}
}
