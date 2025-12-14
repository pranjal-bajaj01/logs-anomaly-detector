#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>

using namespace std;

int main() {
    string brokers = "localhost:9092";
    string topic = "system-logs";
    string group = "sde-group";
    string errstr;

    // Kafka Setup
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", group, errstr);
    conf->set("auto.offset.reset", "latest", errstr);

    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    if (!consumer) { cerr << "Consumer failed: " << errstr << endl; return 1; }
    consumer->subscribe({topic});

    cout << "--- DETECTOR LISTENING ---" << endl;

    while (true) {
        RdKafka::Message *msg = consumer->consume(1000);
        if (msg->err() == RdKafka::ERR_NO_ERROR) {
            string payload = static_cast<const char *>(msg->payload());
            
            // The Logic
            if (payload.find("[ERROR]") != string::npos) {
                cout << "\033[1;31m[CRITICAL ALERT] Anomaly Found: " << payload << "\033[0m" << endl;
            } else {
                cout << "[LOG] " << payload << endl;
            }
        }
        delete msg;
    }
    return 0;
}