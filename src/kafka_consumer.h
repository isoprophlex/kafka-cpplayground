#pragma once
#include <string>
#include <vector>
#include <librdkafka/rdkafkacpp.h>

class KafkaConsumer {
public:
    KafkaConsumer(const std::string& brokers, const std::string& group_id, const std::string& topic);
    ~KafkaConsumer();

    // Devuelve un mensaje recibido o nullptr si no hay
    RdKafka::Message* consume_once(int timeout_ms = 1000);

private:
    RdKafka::KafkaConsumer* consumer;
    std::vector<std::string> topics;
};
