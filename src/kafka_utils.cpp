#include "kafka_utils.h"
#include <librdkafka/rdkafkacpp.h>
#include <iostream>

std::vector<std::string> KafkaUtils::get_topics(const std::string& brokers) {
    std::vector<std::string> topics_list;
    std::string errstr;

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
    if(!producer) {
        std::cerr << "Error creando producer temporal: " << errstr << std::endl;
        delete conf;
        return topics_list;
    }

    delete conf;

    RdKafka::Metadata* metadata;
    if(producer->metadata(true, nullptr, &metadata, 5000) == RdKafka::ERR_NO_ERROR) {
        const RdKafka::Metadata::TopicMetadataVector* topics = metadata->topics();
        for(auto t = topics->begin(); t != topics->end(); ++t) {
            topics_list.push_back((*t)->topic());
        }
    }
    delete metadata;
    delete producer;

    return topics_list;
}
