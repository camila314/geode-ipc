#include <geode-ipc.hpp>
#include <matjson.hpp>
#include <iostream>

int main() {
	auto out = GeodeIPC::send("geode.loader", "ipc-test", {});

	if (out.isErr()) {
		std::cout << "Error: " << out.unwrapErr() << std::endl;
		return 1;
	}

	for (auto& mod : out.unwrap().asArray().unwrap()) {
		auto name = mod["name"].asString().unwrap();
		auto id = mod["id"].asString().unwrap();
		auto version = mod["version"].asString().unwrap();

		std::cout << name << " (" << id << ") version " << version << std::endl;
	}

	return 0;
}
