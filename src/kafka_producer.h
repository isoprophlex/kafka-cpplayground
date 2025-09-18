#pragma once
#include <string>
#include <librdkafka/rdkafkacpp.h>

class KafkaProducer {
public:
    KafkaProducer(const std::string& brokers);
    ~KafkaProducer();

    bool produce(const std::string& topic_name, const std::string& message, const std::string& key="");

private:
    RdKafka::Producer* producer;
};
