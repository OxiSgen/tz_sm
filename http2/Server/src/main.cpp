#include <algorithm>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <stdlib.h>
#include <map>
#include <chrono>
#include <thread>

using namespace Pistache;

struct Student {
    std::string id;
    std::string name;
    std::string surname;
    std::string date;
};

namespace Json 
{
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

    void file_read(std::vector<Student>& st, std::string FileName) {
        std::string buffer;
        std::vector<std::string> pv;

        std::ifstream fin(FileName);
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

}


class StatsEndpoint
{
public:
    explicit StatsEndpoint(Address port)
        : httpEndpoint(std::make_shared<Http::Endpoint>(port))
    { }

    void init(int thr = 1)
    {
        auto opts = Http::Endpoint::options().threads(thr);
        httpEndpoint->init(opts);
        setup_routes();
    }

    void start()
    {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
    void setup_routes()
    {
        using namespace Rest;

        Routes::Get(router, "/ready", Routes::bind(&StatsEndpoint::ready, this));
    }

    void ready(const Rest::Request& request, Http::ResponseWriter response)
    {
        std::string json_str;
        {
            std::stringstream json;
            std::vector<Student> st;
            Json::file_read(st, "file_student.txt");
            Json::file_read(st, "file_student_2.txt");

            boost::property_tree::ptree data;
            Json::json_serializer(data, st);

            try {
                boost::property_tree::json_parser::write_json(json, data);
            }
            catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }

            json_str = json.str();
        }
        response.send(Http::Code::Ok, json_str);
    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char* argv[])
{
    // docker build -t hello_cpp_http .
    // docker run -p 9080:9080 -t hello_cpp_http
    Port port(9080);
    Address sock(Ipv4::any(), port);

    StatsEndpoint stats(sock);
    stats.init();
    stats.start();
}