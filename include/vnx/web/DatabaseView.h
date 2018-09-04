
#ifndef INCLUDE_VNX_WEB_DATABASEVIEW_H
#define INCLUDE_VNX_WEB_DATABASEVIEW_H

#include <vnx/web/DatabaseViewBase.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/database/DatabaseClient.hxx>


namespace vnx {
namespace web {

class DatabaseView : public DatabaseViewBase {
public:
	DatabaseView(const std::string& _vnx_name);

protected:
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override;
	
	void update();
	
private:
	void render_overview(std::ostream& out);
	
	void render_table_index(std::ostream& out, const std::string& table, const Object& parameter);
	
	void render_table(std::ostream& out, const std::vector<Object>& result, bool hide_id);
	
	void render_table(std::ostream& out, const std::vector<Object>& result, const std::vector<std::string>& fields);
	
private:
	std::shared_ptr<Provider> provider;
	std::shared_ptr<database::DatabaseClient> client;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_DATABASEVIEW_H
