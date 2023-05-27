#pragma once

#include <QObject>
#include <QTimer>

class DelayAgent : public QObject {
	Q_OBJECT
public:
	explicit DelayAgent(QObject *parent);
	~DelayAgent();

	struct DelayAgentContext {
		QTimer *timer;
	};

	DelayAgentContext *ctx_;

private:
	void StartTimer();
	void StopTimer();

private slots:
	void TimerDecrement();
};
