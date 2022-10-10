#pragma once

#include <QPalette>
#include <QWidget>


class QPushButton;
class QTableWidget;
class QTextEdit;
class QWebSocket;
class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  void connect_signals(const QWebSocket& sock);

  void on_button_clicked();
  void on_compressing_finished();
  void on_connected();
  void on_disconnected();
  void on_text_message_received(QString message);

private:
  QWebSocket* ws_                  = nullptr;
  bool        connection_status_   = false;
  bool        data_sending_status_ = false;

private:
  QPushButton*   pb_connect_;
  QTextEdit*     te_data_for_sending_;
  QTableWidget*  table_compress_stat_;

  const QPalette green0_{Qt::green};
  const QPalette red0_{Qt::red};
  const QPalette green1_{QColor(110,140,60)};
  const QPalette red1_{QColor(200,110,70)};
  const QPalette gray0_{Qt::gray};
  const QPalette yellow0_{Qt::yellow};
};
