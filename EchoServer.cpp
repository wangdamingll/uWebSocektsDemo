#include "App.h"

/* ws->getUserData returns one of these */
struct PerSocketData {
	int a = 0;
};

int main() {
	auto app = uWS::App();
	auto behavior = uWS::Behavior();
	behavior.compression= uWS::DISABLED;
	behavior.maxPayloadLength = 16 * 1024;
	behavior.idleTimeout = 10;
	behavior.open = [](auto *ws, auto *req) {
		auto socketData = (struct PerSocketData*)ws->getUserData();
		socketData->a = 1;
		auto remoteIp = ws->getRemoteAddress();
		std::cout << "open::remoteIp=[" <<remoteIp.data()<<"]"<< std::endl;
	};
	behavior.message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
		auto socketData = (struct PerSocketData*)ws->getUserData();
		int size = ws->getBufferedAmount();
		bool sendRet = ws->send(message, opCode);
		std::cout << "message=[" << message<<"] size=["<< size << "] sendRet=["<<sendRet<<"]" << std::endl;
	};
	behavior.drain = [](auto *ws) {
		int size = ws->getBufferedAmount();
		std::cout << "drain::size=["<< size<<"]" <<std::endl;
	};
	behavior.ping = [](auto *ws) {
		std::cout << "ping" << std::endl;
	};
	behavior.pong = [](auto *ws) {
		std::cout << "pong" << std::endl;
	};
	behavior.close = [](auto *ws, int code, std::string_view message) {
		std::cout << "close::code=["<<code<<"] message=["<< message << "]"<<std::endl;
	};

	app.ws<PerSocketData>("/*", (uWS::Behavior&&)behavior).listen(9001, [](auto *token) {
			if (token) {
				std::cout << "Listening on port " << 9001 << std::endl;
			}
	}).run();

}
