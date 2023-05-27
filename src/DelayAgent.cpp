#include <obs-module.h>

#include "plugin-macros.generated.h"

#include "DelayAgent.hpp"

DelayAgent::DelayAgent(QObject *parent) : QObject(parent) {
	ctx_ = new DelayAgentContext();
	ctx_->timer = new QTimer();

	blog(LOG_INFO, "tapgame :: connect time");
	QObject::connect(ctx_->timer, SIGNAL(timeout()), SLOT(TimerDecrement()));

	blog(LOG_INFO, "tapgame :: Starting the timer");
	StartTimer(ctx_);
}

DelayAgent::~DelayAgent() {
	StopTimer(ctx_);
	delete ctx_;
}

void DelayAgent::StartTimer(DelayAgentContext* ctx) {
	ctx->timer->start(5000);
}

void DelayAgent::StopTimer(DelayAgentContext* ctx) {
	ctx->timer->stop();
}

void DelayAgent::TimerDecrement() {
	blog(LOG_INFO, "tapgame :: on timer");
}
