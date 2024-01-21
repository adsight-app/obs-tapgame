#pragma once

#include <QObject>
#include <QTimer>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

struct DelayAgentContext {
	bool found_stream;
	bool stream_active;
	uint32_t delay;

	bool found_streamer_key;
	QString streamer_key;
};

class DelayAgent : public QObject {
	Q_OBJECT
public:
	explicit DelayAgent(QObject *parent);
	~DelayAgent();

	QTimer *timer;
	QNetworkAccessManager qnam;
	QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;

	DelayAgentContext *ctx_;

private:
	void StartTimer();
	void StopTimer();
	void ReportConnection();

private slots:
	void TimerDecrement();
	void HttpFinished();
	void sslErrors(const QList<QSslError> &errors);
};
