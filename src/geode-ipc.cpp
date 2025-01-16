#include <geode-ipc.hpp>
#include <geode-ipc.h>

using namespace geode;
using namespace matjson;

Result<std::string> GeodeIPC::sendRaw(std::string const& message, double timeout) {
	auto resp = geode_ipc_send_timeout(message.c_str(), timeout);

	if (resp == NULL)
		return Err("failed to send message");

	std::string out = resp;
	free(resp);

	return Ok(out);
}

geode::Result<matjson::Value> GeodeIPC::sendRawJSON(matjson::Value const& message, double timeout) {
	auto resp = sendRaw(message.dump(), timeout);

	if (resp.isErr())
		return Err("failed to send message");

	auto ret = parse(resp.unwrap());
	if (ret.isErr())
		return Err("failed to parse response");


	return Ok(ret.unwrap());
}

geode::Result<matjson::Value> GeodeIPC::send(
	std::string const& modName,
	std::string const& messageName,
	matjson::Value const& messageData,
	double timeout
) {
	auto message = makeObject({
		{"mod", modName},
		{"message", messageName},
		{"data", messageData}
	});

	return sendRawJSON(message, timeout);
}
