#include <obs-module.h>

#include <QString>
#include <QRegularExpression>
#include <QUrl>


#include "plugin-macros.generated.h"

#include "DelayAgent.hpp"

#define ENDPOINT "https://4mtbvr6meium22qtmt3thewsfy0jrbpa.lambda-url.eu-west-2.on.aws/"

#define DELAY_PUSH_PERIOD_SEC 10000
#define REQUIRED_STREAM "simple_stream"
#define REQUIRED_BROWSER_SOURCE "tapgame"
#define URL_PROPERTY_NAME "url"

bool next_output(void * param, obs_output_t *output) {
	DelayAgentContext *ctx = static_cast<DelayAgentContext *>(param);
	const char * name = obs_output_get_name(output);

	if (strncmp(REQUIRED_STREAM, name, 14) == 0) {
		uint32_t delay = obs_output_get_active_delay(output);
		ctx->delay = delay;
		ctx->found_delay = true;
		return false;
	}

	return true;
}

bool next_source(void*param, obs_source_t* source) {
	DelayAgentContext *ctx = static_cast<DelayAgentContext *>(param);
	const char *name = obs_source_get_name(source);
	bool rc = true;

	if (strncmp(REQUIRED_BROWSER_SOURCE, name, 8) == 0) {
		obs_data_t *data = obs_source_get_settings(source);
		const char * value = obs_data_get_string(data, URL_PROPERTY_NAME);

		if (value == NULL) {
			blog(LOG_WARNING,
			     "No URL value found for 'tapgame' source");
		} else {
			QRegularExpression re("StreamerKey=([^=&]+)");
			QRegularExpressionMatch match = re.match(value);

			if (match.hasMatch()) {
				QString key = match.captured(1);
				ctx->streamer_key = key;

				blog(LOG_INFO, "Matched Key %s",
				     key.toStdString().c_str());

				ctx->found_streamer_key = true;
				rc = false;
			} else {
				blog(LOG_WARNING,
				     "No StreamerKey found in the 'tapgame' source");		
			}
		}

		obs_data_release(data);
	}

	return rc;
}

DelayAgent::DelayAgent(QObject *parent) : QObject(parent) {
	ctx_ = new DelayAgentContext();
	timer = new QTimer();

	QObject::connect(timer, SIGNAL(timeout()), SLOT(TimerDecrement()));

	StartTimer();
}

DelayAgent::~DelayAgent() {
	StopTimer();
	delete ctx_;
}

void DelayAgent::StartTimer() {
	blog(LOG_INFO, "Timer starting (%d)", DELAY_PUSH_PERIOD_SEC);
	timer->start(DELAY_PUSH_PERIOD_SEC);
}

void DelayAgent::StopTimer() {
	timer->stop();
}

void DelayAgent::TimerDecrement() {
	blog(LOG_INFO, "Timer Elapsed");
	ctx_->found_delay = false;
	ctx_->found_streamer_key = false;
	obs_enum_sources(next_source, ctx_);
	obs_enum_outputs(next_output, ctx_);

	if (!ctx_->found_streamer_key) {
		blog(LOG_WARNING, "Plugin setup incorrect");
		// TODO: Report this problem to the platform
		return;
	}

	if (!ctx_->found_delay) {
		blog(LOG_INFO, "Streaming not started");
		// TODO: Consider reporting to the platform
		return;
	}

	QString url = QString(ENDPOINT) + "?StreamerKey=" + ctx_->streamer_key;

	blog(LOG_INFO, "Requesting %s", url.toStdString().c_str());
	reply.reset(qnam.get(QNetworkRequest(url)));
	connect(reply.get(), &QNetworkReply::finished, this, &DelayAgent::HttpFinished);
	blog(LOG_INFO, "Requested");
}

void DelayAgent::HttpFinished(){
	blog(LOG_INFO, "HTTP Finished");
	//QNetworkReply::NetworkError error = reply->error();
	const QString &errorString = reply->errorString();
	blog(LOG_WARNING, "HTTP Error %s", errorString.toStdString().c_str());
}
