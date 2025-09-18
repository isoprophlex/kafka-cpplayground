#include <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "./src/kafka_producer.h"
#include "./src/kafka_utils.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Kafka Producer UI");

    QVBoxLayout* layout = new QVBoxLayout();
    QComboBox* topicCombo = new QComboBox();
    QTextEdit* messageEdit = new QTextEdit();
    messageEdit->setPlaceholderText("Escribí tu mensaje...");
    QPushButton* sendButton = new QPushButton("Enviar");
    QTextEdit* log = new QTextEdit();
    log->setReadOnly(true);

    layout->addWidget(topicCombo);
    layout->addWidget(messageEdit);
    layout->addWidget(sendButton);
    layout->addWidget(log);
    window.setLayout(layout);
    window.resize(400, 300);
    window.show();

    const std::string brokers = "localhost:9092,localhost:9094,localhost:9095";

    // Llenar ComboBox con topics dinámicamente
    std::vector<std::string> topics = KafkaUtils::get_topics(brokers);
    for(const auto& t : topics) {
        topicCombo->addItem(QString::fromStdString(t));
    }

    KafkaProducer producer(brokers);

    QObject::connect(sendButton, &QPushButton::clicked, [&](){
        std::string topic = topicCombo->currentText().toStdString();
        std::string msg = messageEdit->toPlainText().toStdString();
        if(producer.produce(topic, msg)) {
            log->append(QString::fromStdString("[OK] Mensaje enviado a " + topic + ": " + msg));
        } else {
            log->append("[ERROR] No se pudo enviar mensaje");
        }
        messageEdit->clear();
    });

    return app.exec();
}
