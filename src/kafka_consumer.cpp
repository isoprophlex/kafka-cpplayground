#include "kafka_consumer.h"
#include <librdkafka/rdkafkacpp.h>
#include <iostream>

KafkaConsumer::KafkaConsumer(const std::string& brokers, const std::string& group_id, const std::string& topic) {
    std::string errstr;

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", group_id, errstr);
    conf->set("enable.auto.commit", "true", errstr);

    consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    if (!consumer) {
        std::cerr << "Error creando Consumer: " << errstr << std::endl;
        exit(1);
    }

    delete conf;

    topics.push_back(topic);
    consumer->subscribe(topics);
}

KafkaConsumer::~KafkaConsumer() {
    if (consumer) {
        consumer->close();
        delete consumer;
    }
}

RdKafka::Message* KafkaConsumer::consume_once(int timeout_ms) {
    if (!consumer) return nullptr;
    RdKafka::Message* msg = consumer->consume(timeout_ms);

    if (msg->err() == RdKafka::ERR_NO_ERROR) {
        return msg;
    }

    delete msg;
    return nullptr;
}
