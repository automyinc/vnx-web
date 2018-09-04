
#ifndef INCLUDE_VNX_WEB_DATABASEVIEW_H
#define INCLUDE_VNX_WEB_DATABASEVIEW_H

#include <vnx/web/DatabaseViewBase.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/DatabaseClient.hxx>


namespace vnx {
namespace web {

class DatabaseView : public DatabaseViewBase {
public:
	DatabaseView(const std::string& _vnx_name);

protected:
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override;
	
	void update();
	
	void maintain();
	
private:
	std::shared_ptr<Content> render(std::shared_ptr<const Request> request,
									std::shared_ptr<const Object> parameter) const;
	
	std::shared_ptr<Content> login(std::shared_ptr<const Request> request,
									std::shared_ptr<const Object> parameter) const;
	
	void render_overview(std::ostream& out) const;
	
	void render_table_index(std::ostream& out, const std::string& table, const Object& parameter) const;
	
	void render_table(std::ostream& out, const std::vector<Object>& result,
				const std::vector<std::string>& fields, const std::map<std::string, std::string>& field_header) const;
	
private:
	std::shared_ptr<Provider> provider;
	std::shared_ptr<DatabaseClient> client;
	
	struct session_t {
		bool is_valid = false;
		int64_t create_time = 0;
	};
	
	std::string ssid_key;
	std::unordered_map<std::string, session_t> sessions;
	session_t* current = 0;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_DATABASEVIEW_H
