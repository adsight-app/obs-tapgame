#include <obs-module.h>

#include "plugin-macros.generated.h"

#include "DelayAgent.hpp"

#define DELAY_PUSH_PERIOD_SEC 1000

bool next_out(void *, obs_output_t *output) {
	blog(LOG_INFO, "tapgame :: next");
	// not needed yet: DelayAgent::DelayAgentContext *ctx = static_cast<DelayAgent::DelayAgentContext *>(param);
	uint32_t delay = obs_output_get_active_delay(output);
	blog(LOG_INFO, "tapgame :: active delay %d", delay);
	return true;
}

DelayAgent::DelayAgent(QObject *parent) : QObject(parent) {
	ctx_ = new DelayAgentContext();
	ctx_->timer = new QTimer();

	QObject::connect(ctx_->timer, SIGNAL(timeout()), SLOT(TimerDecrement()));

	StartTimer();
}

DelayAgent::~DelayAgent() {
	StopTimer();
	delete ctx_;
}

void DelayAgent::StartTimer() {
	blog(LOG_INFO, "tapgame :: Timer starting (%d)", DELAY_PUSH_PERIOD_SEC);
	ctx_->timer->start(DELAY_PUSH_PERIOD_SEC);
}

void DelayAgent::StopTimer() {
	ctx_->timer->stop();
}

void DelayAgent::TimerDecrement() {
	blog(LOG_INFO, "tapgame :: on timer");
	obs_enum_outputs(next_out, ctx_);
}
