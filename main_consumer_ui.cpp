#include <QApplication>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include "./src/kafka_consumer.h"
#include "./src/kafka_utils.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Kafka Consumer UI");

    QVBoxLayout* layout = new QVBoxLayout();
    QTextEdit* messageView = new QTextEdit();
    messageView->setReadOnly(true);
    layout->addWidget(messageView);
    window.setLayout(layout);
    window.resize(400, 300);
    window.show();

    const std::string brokers = "localhost:9092,localhost:9094,localhost:9095";

    std::vector<std::string> topics = KafkaUtils::get_topics(brokers);
    std::string topic_to_consume = topics.empty() ? "test-topic" : topics[0];

    KafkaConsumer consumer(brokers, "dynamic-group", topic_to_consume);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&](){
        RdKafka::Message* msg = consumer.consume_once(500);
        if(msg) {
            std::string payload_str(static_cast<const char*>(msg->payload()), msg->len());
            messageView->append(QString::fromStdString("[" + topic_to_consume + "] " + payload_str));
            delete msg;
        }
    });
    timer.start(500);

    return app.exec();
}
