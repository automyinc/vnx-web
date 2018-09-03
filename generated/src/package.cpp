
// AUTO GENERATED by vnxcppcodegen

#include <vnx/Topic.h>

#include <vnx/web/BinaryData.hxx>
#include <vnx/web/CacheBase.hxx>
#include <vnx/web/Content.hxx>
#include <vnx/web/DefaultView.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/DynamicFile.hxx>
#include <vnx/web/DynamicView.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/File.hxx>
#include <vnx/web/FileInfo.hxx>
#include <vnx/web/FileSystemBase.hxx>
#include <vnx/web/FrontendBase.hxx>
#include <vnx/web/Generic.hxx>
#include <vnx/web/HttpParserBase.hxx>
#include <vnx/web/HttpProcessorBase.hxx>
#include <vnx/web/HttpRendererBase.hxx>
#include <vnx/web/HttpRequest.hxx>
#include <vnx/web/HttpResponse.hxx>
#include <vnx/web/Notification.hxx>
#include <vnx/web/PageView.hxx>
#include <vnx/web/Parameter.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/Request.hxx>
#include <vnx/web/Response.hxx>
#include <vnx/web/StreamEventArray.hxx>
#include <vnx/web/StreamRead.hxx>
#include <vnx/web/StreamWrite.hxx>
#include <vnx/web/View.hxx>
#include <vnx/web/ViewProcessorBase.hxx>
#include <vnx/web/cache_entry_t.hxx>
#include <vnx/web/request_type_e.hxx>
#include <vnx/web/stream_event_t.hxx>



namespace vnx {
namespace web {


static void register_all_types() {
	vnx::register_type_code(vnx::web::BinaryData::create_type_code());
	vnx::register_type_code(vnx::web::CacheBase::create_type_code());
	vnx::register_type_code(vnx::web::Content::create_type_code());
	vnx::register_type_code(vnx::web::DefaultView::create_type_code());
	vnx::register_type_code(vnx::web::Directory::create_type_code());
	vnx::register_type_code(vnx::web::DynamicFile::create_type_code());
	vnx::register_type_code(vnx::web::DynamicView::create_type_code());
	vnx::register_type_code(vnx::web::ErrorCode::create_type_code());
	vnx::register_type_code(vnx::web::File::create_type_code());
	vnx::register_type_code(vnx::web::FileInfo::create_type_code());
	vnx::register_type_code(vnx::web::FileSystemBase::create_type_code());
	vnx::register_type_code(vnx::web::FrontendBase::create_type_code());
	vnx::register_type_code(vnx::web::Generic::create_type_code());
	vnx::register_type_code(vnx::web::HttpParserBase::create_type_code());
	vnx::register_type_code(vnx::web::HttpProcessorBase::create_type_code());
	vnx::register_type_code(vnx::web::HttpRendererBase::create_type_code());
	vnx::register_type_code(vnx::web::HttpRequest::create_type_code());
	vnx::register_type_code(vnx::web::HttpResponse::create_type_code());
	vnx::register_type_code(vnx::web::Notification::create_type_code());
	vnx::register_type_code(vnx::web::PageView::create_type_code());
	vnx::register_type_code(vnx::web::Parameter::create_type_code());
	vnx::register_type_code(vnx::web::Provider::create_type_code());
	vnx::register_type_code(vnx::web::Request::create_type_code());
	vnx::register_type_code(vnx::web::Response::create_type_code());
	vnx::register_type_code(vnx::web::StreamEventArray::create_type_code());
	vnx::register_type_code(vnx::web::StreamRead::create_type_code());
	vnx::register_type_code(vnx::web::StreamWrite::create_type_code());
	vnx::register_type_code(vnx::web::View::create_type_code());
	vnx::register_type_code(vnx::web::ViewProcessorBase::create_type_code());
	vnx::register_type_code(vnx::web::cache_entry_t::create_type_code());
	vnx::register_type_code(vnx::web::request_type_e::create_type_code());
	vnx::register_type_code(vnx::web::stream_event_t::create_type_code());
}

static struct vnx_static_init {
	vnx_static_init() {
		register_all_types();
	}
} vnx_static_init_;

} // namespace vnx
} // namespace web
