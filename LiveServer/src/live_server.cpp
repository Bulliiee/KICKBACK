//
// Created by SSAFY on 2024-04-29.
// 라이브 서버
//

#include <boost/asio.hpp>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

#include "model/connection_info_udp.cpp"
#include "util/thread_safe_queue.h"

using boost::asio::ip::udp;

// 상수
enum {
    max_length = 1024
};

// thread-safe message queue
ThreadSafeQueue<std::pair<udp::endpoint, std::string>> message_queue;


//class LiveServer {
//public:
//    LiveServer(boost::asio::io_context &io_context, short port)
//        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
//    }
//
//    // 수신 스레드가 참조
//    void startReceive() {
//        // 비동기적으로 데이터 수신 위해 socket_객체에 호출
//        // 데이터 수신 할 때까지 현재 스레드 블록x
//        socket_.async_receive_from(
//                // 수신된 데이터를 저장할 버퍼, recv_buffer_는 실제 데이터가 저장될 메모리 영역임
//                // remote_endpoint_는 데이터 보낸 엔드포인트 주소와 포트 저장할 객체
//                boost::asio::buffer(recv_buffer), remote_endpoint,
//                // async_receive_from()의 콟백함수로, 비동기 작업이 완료(데이터 수신시)되면 실행
//                // [this]: 람다 캡처 리스트로, 현재 객체를 람다 내부에서 사용하게 함
//                // 이는 클래스 멤버변수나 함수에 접근 시 사용
//                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
//                    if (!ec && bytes_recvd > 0) {
//                        std::string received_message(recv_buffer, bytes_recvd);
//                        // mutex lock 후 message_queue에 넣기
//                        message_queue.push(std::make_pair(remote_endpoint, received_message));
//                        startReceive();
//                    }
//                });
//    }
//
//    // 작업 스레드가 참조
//    void startWork(boost::asio::io_context &io_context) {
//        std::thread([&]() {
//            while (true) {
//                // TODO: 그냥 lock, unique_lock 차이 확인
//                std::pair<udp::endpoint, std::string> recv_data;
//                message_queue.wait_and_pop(recv_data);
//                std::cout << recv_data.first << " " << recv_data.second << std::endl;
//
//                udp::endpoint sender = recv_data.first;
//                std::string message = recv_data.second;
//
//                io_context.post([&io_context, sender, message, this] {
//                    return process_message(io_context, sender, message);
//                });
//            }
//        }).detach();
//    }
//
//
//private:
//    void process_message(boost::asio::io_context &io_context, const udp::endpoint &sender, const std::string &message) {
//        // TODO: 실제 메시지 처리 로직
//        std::cout << "Processing message(" << sender << "): " << message << std::endl;
//
//        // Example message format: "CREATE:a", "JOIN:a", "MSG:a:Hello"
//        auto command_delimiter_pos = message.find(':');
//        auto room_delimiter_pos = message.find(':', command_delimiter_pos + 1);
//        std::string command = message.substr(0, command_delimiter_pos);
//        std::string room_name = message.substr(command_delimiter_pos + 1,
//                                               room_delimiter_pos - command_delimiter_pos - 1);
//        std::string content = message.substr(room_delimiter_pos + 1);
//        std::cout << command << ", " << room_name << ", " << content << std::endl;
//
//        if (command == "CREATE") {
//            rooms_[room_name].insert(sender);
//        } else if (command == "JOIN") {
//            rooms_[room_name].insert(sender);
//        } else if (command == "MSG") {
//            for (const auto &participant: rooms_[room_name]) {
//                if (participant != sender) {
//                    socket_.send_to(boost::asio::buffer(content), participant);
//                }
//            }
//        } else {
//            socket_.send_to(boost::asio::buffer("Incorrect message"), sender);
//        }
//    }
//
//    udp::socket socket_;
//    std::unordered_map<std::string, std::set<udp::endpoint>> rooms_;
//
//    udp::endpoint remote_endpoint;
//    char recv_buffer[max_length];
//};

// 아래는 테스트용 클래스임
class receiver {
public:
    void run() {
        do_receive();
    }

private:
    void do_receive() {
        ConnectionInfoUDP::instance().socket().async_receive_from(
                boost::asio::buffer(receive_buffer_), remote_endpoint,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                    if (!ec && bytes_recvd > 0) {
                        std::string received_message(receive_buffer_, bytes_recvd);
                        // mutex lock 후 message_queue에 넣기
                        message_queue.push(std::make_pair(remote_endpoint, received_message));
                        do_receive();
                    }
                });
    }

    udp::endpoint remote_endpoint;
    char receive_buffer_[max_length]{};
};

class worker {
public:
    void run() {
        do_work();
    }

private:
    void do_work() {
        std::thread([&]() {
            while (true) {
                // TODO: 그냥 lock, unique_lock 차이 확인
                std::pair<udp::endpoint, std::string> recv_data;
                message_queue.wait_and_pop(recv_data);
                std::cout << recv_data.first << " " << recv_data.second << std::endl;

                udp::endpoint sender = recv_data.first;
                std::string message = recv_data.second;

                process_message(sender, message);
            }
        }).detach();
    }

    void process_message(const udp::endpoint &sender, const std::string &message) {
        // TODO: 실제 메시지 처리 로직
        std::cout << "Processing message(" << sender << "): " << message << std::endl;

        // Example message format: "CREATE:a", "JOIN:a", "MSG:a:Hello"
        auto command_delimiter_pos = message.find(':');
        auto room_delimiter_pos = message.find(':', command_delimiter_pos + 1);
        std::string command = message.substr(0, command_delimiter_pos);
        std::string room_name = message.substr(command_delimiter_pos + 1,
                                               room_delimiter_pos - command_delimiter_pos - 1);
        std::string content = message.substr(room_delimiter_pos + 1);
        std::cout << command << ", " << room_name << ", " << content << std::endl;

        if (command == "CREATE") {
            rooms_[room_name].insert(sender);
        } else if (command == "JOIN") {
            rooms_[room_name].insert(sender);
        } else if (command == "MSG") {
            for (const auto &participant: rooms_[room_name]) {
                if (participant != sender) {
                    ConnectionInfoUDP::instance().socket().send_to(boost::asio::buffer(content), participant);
                }
            }
        } else {
            ConnectionInfoUDP::instance().socket().send_to(boost::asio::buffer("Incorrect message"), sender);
        }
    }

    std::unordered_map<std::string, std::set<udp::endpoint>> rooms_;
    udp::endpoint remote_endpoint_;
};


int main() {
    try {
//                boost::asio::io_context io_context;
//
//                LiveServer live_server(io_context, 12345);
//                std::size_t thread_pool_size = (std::thread::hardware_concurrency() * 2) + 1;
//                std::vector<std::thread> threads;
//                // 수신 스레드
//                live_server.startReceive();
//                threads.emplace_back([&io_context] { io_context.run(); });
//
//                // 처리 스레드
//                live_server.startWork(io_context);
//                for (std::size_t i = 1; i < thread_pool_size; i++) {
//                    threads.emplace_back([&io_context] { io_context.run(); });
//                }
//                for (auto &t: threads) {
//                    t.join();
//                }

        // IO 컨텍스트 객체 생성 후 소켓 설정 초기화
        boost::asio::io_context io_context;
        ConnectionInfoUDP::instance().init(io_context, 12345);

        // 수신 및 처리 객체
        receiver receiver;
        worker worker;

        // receiver용 단일 스레드 시작
        std::thread receiver_thread([&receiver] {
            receiver.run();
        });

        // worker용 스레드 풀 생성 및 실행
        std::size_t thread_pool_size = std::thread::hardware_concurrency() * 2;
        std::vector<std::thread> worker_threads;
        worker.run();
        for (std::size_t i = 1; i < thread_pool_size; i++) {
            worker_threads.emplace_back([&io_context] { io_context.run(); });
        }

        // 모든 스레드 대기
        receiver_thread.join();
        for (auto &t: worker_threads) {
            t.join();
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}