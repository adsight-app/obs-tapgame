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
	void StartTimer(DelayAgentContext *ctx);
	void StopTimer(DelayAgentContext *ctx);

private slots:
	void TimerDecrement();
};
