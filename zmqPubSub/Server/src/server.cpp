#include <vector>
#include <iostream>
#include <fstream>
#include <zmq.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <stdlib.h>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define PRESSKEY() (!kbhit())
#else
#include <curses.h>
#define PRESSKEY() (getch() == ERR)
#endif // _WIN32


struct Student {
    std::string id;
    std::string name;
    std::string surname;
    std::string date;
};

void json_serializer(boost::property_tree::ptree& pt, std::vector<Student>& st_vec) {
    /* Сериализуем данные из файла в  Json, в котором ключи имеют вид "ФамилияИмяДата".
    Так сразу отсеятся дублирующие записи и сортировка сведётся к тривиальной (по ключам). */
    using namespace std;

    for (auto& st : st_vec) {
        string key = st.surname + st.name + st.date;
        std::replace(key.begin(), key.end(), '.', '_');
        if (!pt.count(key)) {
            pt.put(key + ".id", st.id);
            pt.put(key + ".name", st.name);
            pt.put(key + ".surname", st.surname);
            pt.put(key + ".date", st.date);
        }
    }
}


void file_read(std::vector<Student>& st, std::string file_Name) {
    std::string buffer;
    std::vector<std::string> pv;

    std::ifstream fin(file_Name);
    if (!fin.is_open()) {
        std::cout << "Не удалось найти файл: " << file_Name << std::endl;
        return;
    }
    while (std::getline(fin, buffer)) {
        boost::split(pv, buffer, boost::is_any_of(" "));
        try {
            st.push_back({ pv.at(0), pv.at(1), pv.at(2), pv.at(3) });
        }
        catch (std::out_of_range) {
            continue;
        }
    }
    fin.close();
}

void file_write(std::vector<Student>& st, std::string file_name) {
    std::ofstream myfile;
    myfile.open(file_name);
    for (auto& s : st) {
        myfile << s.id << " " << s.name << " " << s.surname << " " << s.date << '\n';
    }
    myfile.close();
}


class ServerSub
{
public:
    explicit ServerSub(std::string port)
        : port(port)
    {}

    ~ServerSub() {
        socket.close();
        context.close();
    }

    void startServer(std::string& json_str) {
        zmq::context_t context(1);
        try {
            socket = { context, zmq::socket_type::pub }; // ZMQ_PUB
            socket.bind("tcp://*:" + port);
        }
        catch (zmq::error_t) {
            std::cout << "Что-то пошло не так. Ошибка при связывании сокета..." << std::endl;
        }
        handle(json_str);
    }

    void serverStop() {
        socket.close();
        context.close();
    }

    void filePrinter() {
    }

    void handle(std::string& json_str) {
        std::cout << "Working... Для остановки сервера нажмите любую клавишу\n";
        while (PRESSKEY()) {
            //if (GetKeyState(VK_RETURN)) break;

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            zmq::message_t topic(static_cast<std::string>("students"));

            std::string s = json_str;
            zmq::message_t reply(s); 
            try {
                socket.send(topic, zmq::send_flags::sndmore);
                socket.send(reply, zmq::send_flags::none);
            }
            catch (zmq::error_t) {
                std::cout << "Ошибка при отправке сообщений" << std::endl;
            }
        }
        serverStop();
    }

private:
    zmq::socket_t socket;
    std::string port;
    zmq::context_t context;
};


void some_logic() {
    ServerSub ser("5555");

    std::string json_str;
    // Читаем данные из файла
    {
        std::stringstream json;
        std::vector<Student> st;
        file_read(st, "file_student.txt");
        file_read(st, "file_student_2.txt");

        boost::property_tree::ptree data;
        json_serializer(data, st);

        try {
            boost::property_tree::json_parser::write_json(json, data);
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        file_write(st, "outfile/file_student_3.txt");
        json_str = json.str();
    }
    ser.startServer(json_str);
}


int main()
{
    setlocale(LC_ALL, "Russian");
    some_logic();
    return 0;
}
