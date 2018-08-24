
#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/TcpEndpoint.hxx>

#include <sstream>

#include <unistd.h>
#include <sys/socket.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "number";
	options["k"] = "connections";
	options["number"] = "number of requests";
	options["connections"] = "number of connections";
	options["t"] = "target";
	options["target"] = "target server";
	options["u"] = "url";
	options["url"] = "target url";
	
	vnx::init("http_bench_tool", argc, argv, options);
	
	int N = 100;
	int C = 1;
	std::string target = "localhost:8080";
	std::string url = "/";
	vnx::read_config("number", N);
	vnx::read_config("connections", C);
	vnx::read_config("target", target);
	vnx::read_config("url", url);
	
	auto endpoint = vnx::Endpoint::from_url(target);
	
	std::vector<int> sockets(C);
	for(int i = 0; i < C; ++i) {
		const int sock = endpoint->open();
		endpoint->connect(sock);
		sockets[i] = sock;
	}
	
	std::ostringstream tmp;
	tmp << "GET " << url << " HTTP/1.1\r\n";
	tmp << "Connection: keep-alive\r\n";
	tmp << "\r\n";
	
	const std::string request = tmp.str();
	
	int64_t fail_counter = 0;
	
	const int64_t start_time = vnx::get_time_millis();
	for(int i = 0; i < N; ++i) {
		const int sock = sockets[i % C];
		const int res = ::write(sock, request.c_str(), request.size());
		if(res <= 0) {
			fail_counter++;
		}
		if(i >= C) {
			while(true) {
				char buf[16*1024];
				const int num_read = ::recv(sock, buf, sizeof(buf), 0);
				if(num_read != sizeof(buf)) {
					break;
				}
			}
		}
		std::cout << ".";
	}
	for(int i = 0; i < C; ++i) {
		const int sock = sockets[i];
		while(true) {
			char buf[16*1024];
			const int num_read = ::recv(sock, buf, sizeof(buf), MSG_DONTWAIT);
			if(num_read != sizeof(buf)) {
				break;
			}
		}
	}
	const int64_t end_time = vnx::get_time_millis();
	std::cout << std::endl;
	
	std::cout << (1000 * N / (end_time - start_time)) << " requests/s, " << fail_counter << " failed out of " << N << std::endl;
	
	for(int i = 0; i < C; ++i) {
		::close(sockets[i]);
	}
	
	vnx::close();
	
}
