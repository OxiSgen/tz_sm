#include <zmq.hpp>
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iomanip>

using std::setw;
using std::cout;
using std::endl;
using std::cerr;


void json_deserializer(boost::property_tree::ptree& pt ,
                       zmq::message_t& message) {
    std::stringstream jsonStr(message.to_string());
    try {
        boost::property_tree::read_json(jsonStr, pt);
    }
    catch (std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

class Client {
public:
    Client(std::string port, std::string topic)
        : port(port), zmq_topic(topic)
    {}

    ~Client() {
        socket.close();
        context.close();
    }

    void start_client() {
        socket = { context, zmq::socket_type::sub };
        std::cout << "Connecting to server..." << std::endl;
        try {
            socket.connect("tcp://zmq-server-cpp:" + port);
        }
        catch (zmq::error_t er) {
            cout << "Что-то пошло не так. Ошибка при подключении к серверу..." << endl;
        };
        data_recieve(zmq_topic);
    }

    void data_recieve(std::string top) {
        {
            socket.setsockopt(ZMQ_SUBSCRIBE, top.data(), top.size());
        }
        zmq::message_t topic;
        zmq::message_t message;
        try {
            socket.recv(topic);
            socket.recv(message);
        }
        catch (zmq::error_t er) {
            cout << "Что-то пошло не так. Ошибка при получении сообщений..." << endl;
        }

        printer(message);
    }

    void printer(zmq::message_t& message) {
        boost::property_tree::ptree pt;
        json_deserializer(pt, message);
        pt.sort();

        cout << setw(10) << "id" << setw(12) << "surname" << setw(12) << "name" << setw(17) << "date" << endl;
        cout << "-------------------------------------------------------------" << endl;
        BOOST_FOREACH(boost::property_tree::ptree::value_type & framePair, pt) {
            cout << setw(10) << framePair.second.get<std::string>("id") << " |" <<
                    setw(12) << framePair.second.get<std::string>("surname") << " |" <<
                    setw(12) << framePair.second.get<std::string>("name") << " |" <<
                    setw(17) << framePair.second.get<std::string>("date") << endl;
            }
    }

private:
    std::string zmq_topic;
    zmq::socket_t socket;
    std::string port;
    zmq::context_t context;
};


int main()
{
    Client cl("5555", "students");
    cl.start_client();
    return 0;
}
