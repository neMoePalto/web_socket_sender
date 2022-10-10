#include "widget.h"

#include <QByteArray>
#include <QFile>
#include <QGridLayout>
#include <QHeaderView>
#include <QProcess>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QWebSocket>
//#include <QTimer>


Widget::Widget(QWidget *parent)
  : QWidget(parent) {
  // --------------------- GUI --------------------
  pb_connect_ = new QPushButton("&Соединение");
  pb_connect_->setPalette(red0_);
  pb_connect_->setMinimumSize(100, 60);

  auto* grid = new QGridLayout(this);
  grid->addWidget(pb_connect_,          0, 0,   1, 1);  // (obj, row, colmn,    rowSpan, colmnSpan)

  te_data_for_sending_ = new QTextEdit();
  //    _teStatistics->setMinimumSize(500, 150);
  te_data_for_sending_->setFixedWidth(500);
  grid->addWidget(te_data_for_sending_, 1, 0,   2, 2);

  QFont   arial_12B("Monospace", 12, QFont::Bold);

  // Описание таблицы:
  table_compress_stat_ = new QTableWidget(0, 4, this);
  table_compress_stat_->setFont(arial_12B);
  QStringList headers{tr("Размер до\nсжатия"),
                      tr("Размер после\nсжатия"),
                      tr("Коэффициент\nсжатия"),
                      tr("Средний коэффициент\nсжатия")};
  table_compress_stat_->setHorizontalHeaderLabels(headers);
  table_compress_stat_->setColumnWidth(0, 150);
  table_compress_stat_->setColumnWidth(1, 150);
  table_compress_stat_->setColumnWidth(2, 200);
  table_compress_stat_->setColumnWidth(3, 200);
  table_compress_stat_->setFixedWidth(700);
  QHeaderView* header = table_compress_stat_->horizontalHeader();
  header->setFixedHeight(46);
  grid->addWidget(table_compress_stat_,  1, 15,   13, 3);
  // ---------------------------------------

  setLayout(grid);
//  setFixedWidth(1315);
  setMinimumHeight(800);


  //---------------------------------------------


  //---------------------------------------------


  connect(pb_connect_, &QPushButton::clicked, this, &Widget::on_button_clicked);
}


Widget::~Widget() {
}


void Widget::connect_signals(const QWebSocket& sock) {
  connect(&sock, &QWebSocket::connected,    this, &Widget::on_connected);
  connect(&sock, &QWebSocket::disconnected, this, &Widget::on_disconnected);
}


void Widget::on_button_clicked() {
  const QString file_name = "data.txt";
  const QString gz_name = file_name + ".gz";

  auto* tgz_process = new QProcess(this);
//  tgz_process->setWorkingDirectory("/home/yamb");
//  tgz_process->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  connect(tgz_process, &QProcess::finished, this, &Widget::on_compressing_finished);


//  tgz_process->exitStatus();

//  connect(tgz_process, &QProcess::readyReadStandardError, [&tgz_process]() {
//    qDebug() << tgz_process->errorString();
//  });
  connect(tgz_process, &QProcess::readyReadStandardOutput, [&tgz_process]() {
    qDebug() << tgz_process->readAllStandardOutput();
  });

//  tgz_process->start("tar", {"-cfz", tgz_name, file_name});
  tgz_process->start("/bin/sh", {"-c",
                                 "mkdir temp && "
                                 "cp data.txt temp && "
//                                 "cd temp && "
                                 "gzip temp/data.txt"},
                     QProcess::OpenModeFlag::ReadWrite);

//  QFile file(file_name);
//  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//      return;
//  while (!file.atEnd()) {
//      QByteArray line = file.readLine();
//      process_line(line);
  //  }
}


void Widget::on_compressing_finished() {
  if (ws_) {
    ws_->deleteLater();
  }
  ws_ = new QWebSocket(); // Params here! +-
  connect_signals(*ws_);
  const QUrl url = {"ws://localhost:1234"};
  ws_->open(QUrl(url));
}


void Widget::on_connected() {
  pb_connect_->setPalette(green0_);
  connection_status_ = true;

  QString gz_name = "data.txt";
  qDebug() << "QProcess finished!";

  QFile file("temp/" + gz_name);
  if (file.exists()) {
    if (QByteArray ba = file.readAll(); !ba.isEmpty()) {
      ws_->sendBinaryMessage(ba);
      qDebug() << "send message to server";
    }
  }


  connect(ws_, &QWebSocket::textMessageReceived,
          this, &Widget::on_text_message_received);
//  auto str = QStringLiteral("Hello, world!");
//  auto len = ws_->sendTextMessage(str);
//  if (len == str.size()) {
//    data_sending_status_ = true;
//  }
}


void Widget::on_disconnected() {
  if (connection_status_) {
    qDebug() << "WebSocket connected";
    connection_status_ = false;
    if (data_sending_status_) {
      qDebug() << "Данные отправлены на сервер";
      data_sending_status_ = false;
    } else {
      qDebug() << "Не удалось отправить данные на сервер";
    }
    qDebug() << "Соединение завершено";
  } else {
    qDebug() << "Не удалось установить соединение с сервером";
  }
//  pb_connect_->setPalette(*red0_);
}


void Widget::on_text_message_received(QString message) {
//  if (m_debug)
  qDebug() << "Message received:" << message;
//  ws_->close();
}
