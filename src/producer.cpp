#include <iostream>
#include <string>
#include <thread>
#include <librdkafka/rdkafkacpp.h>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>   
using namespace std;

int main() {
    srand(time(0));

    string brokers = "localhost:9092";
    string topic = "system-logs";
    string errstr;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);

    if (!producer) { cerr << "Producer creation failed: " << errstr << endl; return 1; }

    cout << "--- PRODUCER STARTED (RANDOM MODE) ---" << endl;
    int i = 0;
    
    while (true) {
        string msg = "Log_Entry_" + to_string(i);
       
        int chance = rand() % 100;

        if (chance < 15) {
            msg += " [ERROR] Database Connection Failed";
        } else {
            msg += " [INFO] Success";
        }

        // Send to Kafka
        producer->produce(
            topic, 
            RdKafka::Topic::PARTITION_UA, 
            RdKafka::Producer::RK_MSG_COPY, 
            const_cast<char *>(msg.c_str()), msg.size(), 
            NULL, 0, 0, NULL
        );
        
        producer->poll(0);
        
        if (msg.find("ERROR") != string::npos) {
            cout << "Sent: " << msg << " (Spike Generated!)" << endl;
        } else {
            cout << "Sent: " << msg << endl;
        }

        // Speed: 500ms (2 logs per second)
        this_thread::sleep_for(chrono::milliseconds(500)); 
        i++;
    }
    return 0;
}