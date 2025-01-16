#include <string>
#include <matjson.hpp>

namespace GeodeIPC {
	/**
	 * Send raw string and accept raw value. Not recommended,
	 * for regular use as Geode IPC uses json for communication.
	 */
	geode::Result<std::string> sendRaw(std::string const& message, double timeout = 5);

	/**
	 * Send JSON message and get a JSON response. Not recommended
	 * if you need to send messages to a specific mod.
	 */
	geode::Result<matjson::Value> sendRawJSON(matjson::Value const& message, double timeout = 5);

	/**
	 * Send message to specific mod and get JSON response.
	 */
	geode::Result<matjson::Value> send(
		std::string const& modName,
		std::string const& messageName,
		matjson::Value const& messageData,
		double timeout = 5
	);
}
