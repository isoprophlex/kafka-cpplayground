#include "kafka_producer.h"
#include <iostream>
#include <librdkafka/rdkafkacpp.h>


KafkaProducer::KafkaProducer(const std::string& brokers) {
    std::string errstr;

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);

    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Error creando Producer: " << errstr << std::endl;
        exit(1);
    }

    delete conf;
}

KafkaProducer::~KafkaProducer() {
    if (producer) {
        producer->flush(5000); // espera a que se envíen los mensajes
        delete producer;
    }
}

bool KafkaProducer::produce(const std::string& topic_name, const std::string& message, const std::string& key) {
    std::string errstr;

    RdKafka::Conf* tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    RdKafka::Topic* topic_obj = RdKafka::Topic::create(producer, topic_name, tconf, errstr);
    if (!topic_obj) {
        std::cerr << "Error creando topic: " << errstr << std::endl;
        delete tconf;
        return false;
    }

    RdKafka::ErrorCode resp = producer->produce(
        topic_obj,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char*>(message.c_str()),
        message.size(),
        key.empty() ? nullptr : &key,
        nullptr
    );

    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Error enviando mensaje: " << RdKafka::err2str(resp) << std::endl;
    }

    producer->poll(0); // procesa delivery reports
    delete topic_obj;
    delete tconf;

    return resp == RdKafka::ERR_NO_ERROR;
}
