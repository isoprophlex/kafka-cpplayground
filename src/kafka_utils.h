#pragma once
#include <string>
#include <vector>
#include <librdkafka/rdkafkacpp.h>

class KafkaUtils {
public:
    static std::vector<std::string> get_topics(const std::string& brokers);
};

//// TODO: Cacuila size de mensaje y mandar ese msj
// Y ver msg/s
// 1 publisher -> 1 subscriber. Ver velocidad de Kafka.
//