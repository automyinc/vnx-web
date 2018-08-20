
#include <vnx/web/StreamEvent.hxx>


namespace vnx {
namespace web {

std::shared_ptr<const ::vnx::web::StreamEvent> StreamEvent::create(const ::vnx::Hash128& stream, const ::int32_t& event) {
	std::shared_ptr<::vnx::web::StreamEvent> sample = StreamEvent::create();
	sample->stream = stream;
	sample->event = event;
	return sample;
}

std::shared_ptr<const ::vnx::web::StreamEvent> StreamEvent::create_with_value(const ::vnx::Hash128& stream, const ::int32_t& event, const ::int32_t& value) {
	std::shared_ptr<::vnx::web::StreamEvent> sample = StreamEvent::create();
	sample->stream = stream;
	sample->event = event;
	sample->value = value;
	return sample;
}


} // web
} // vnx
