
#include <vnx/web/stream_event_t.hxx>


namespace vnx {
namespace web {

stream_event_t stream_event_t::create(const ::vnx::Hash128& stream, const ::int32_t& event) {
	stream_event_t sample;
	sample.stream = stream;
	sample.event = event;
	return sample;
}

stream_event_t stream_event_t::create_with_value(const ::vnx::Hash128& stream, const ::int32_t& event, const ::int32_t& value) {
	stream_event_t sample;
	sample.stream = stream;
	sample.event = event;
	sample.value = value;
	return sample;
}


} // web
} // vnx
