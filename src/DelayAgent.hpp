#pragma once

#include <QObject>
#include <QTimer>
#include <QString>

struct DelayAgentContext {
	bool found_delay;
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

	DelayAgentContext *ctx_;

private:
	void StartTimer();
	void StopTimer();

private slots:
	void TimerDecrement();
};
