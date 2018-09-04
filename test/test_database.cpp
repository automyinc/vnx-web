/*************************************************************************
* 
*  [2017] - [2018] Automy Inc. 
*  All Rights Reserved.
* 
* NOTICE:  All information contained herein is, and remains
* the property of Automy Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Automy Incorporated
* and its suppliers and may be covered by U.S. and Foreign Patents,
* patents in process, and are protected by trade secret or copyright law.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Automy Incorporated.
*/

#include <vnx/Process.h>
#include <vnx/query/package.h>
#include <vnx/web/Database.h>
#include <vnx/web/DatabaseClient.hxx>

#include <vnx/test/Test.h>
using namespace vnx::test;
using namespace vnx::web;
using namespace vnx;


int main(int argc, char** argv) {
	
	vnx::test::init("vnx.database");
	
	vnx::init("test_database", argc, argv);
	
	VNX_TEST_BEGIN("query")
		const Hash128 id = Hash128::rand();
		Object object;
		object["id"] = id;
		object["test"] = 123;
		object["test2"] = "asdasdasd";
		{
			auto query = query::equal(query::field("test"), query::value(123));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::not_equal(query::field("test"), query::value(123));
			expect(query->execute(object).to<bool>(), false);
		}
		{
			auto query = query::not_equal(query::field("test"), query::value(1234));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::less(query::field("test"), query::value(1234));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::less_equal(query::field("test"), query::value(123));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::greater(query::field("test"), query::value(1234));
			expect(query->execute(object).to<bool>(), false);
		}
		{
			auto query = query::greater_equal(query::field("test"), query::value(123));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::equal(query::field("id"), query::value(id));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::not_equal(query::field("id"), query::value(Hash128()));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::equal(query::field("test2"), query::value("asdasdasd"));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::equal(query::field("test2"), query::value("fghfghfgh"));
			expect(query->execute(object).to<bool>(), false);
		}
		{
			auto query = query::not_equal(query::field("test2"), query::value("fghfghfgh"));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::logical_and(query::less_equal(query::field("test"), query::value(1234)),
											query::greater_equal(query::field("test"), query::value(123)));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::logical_or(query::equal(query::field("test"), query::value(1234)),
											query::equal(query::field("test"), query::value(123)));
			expect(query->execute(object).to<bool>(), true);
		}
		{
			auto query = query::logical_or(query::equal(query::field("test"), query::value(1234)),
											query::not_equal(query::field("test"), query::value(123)));
			expect(query->execute(object).to<bool>(), false);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("aggregate")
		std::vector<Object> object;
		object.resize(3);
		object[0]["test"] = 123;
		object[1]["test"] = 1234;
		object[2]["test"] = 12345;
		{
			auto query = vnx::clone(query::min(query::field("test")));
			for(const auto& obj : object) {
				query->update(obj);
			}
			expect(query->get_result().to<int>(), 123);
		}
		{
			auto query = vnx::clone(query::max(query::field("test")));
			for(const auto& obj : object) {
				query->update(obj);
			}
			expect(query->get_result().to<int>(), 12345);
		}
		{
			auto query = vnx::clone(query::sum(query::field("test")));
			for(const auto& obj : object) {
				query->update(obj);
			}
			expect(query->get_result().to<int>(), 13702);
		}
		{
			auto query = vnx::clone(query::count());
			for(const auto& obj : object) {
				query->update(obj);
			}
			expect(query->get_result().to<int>(), 3);
		}
	VNX_TEST_END()
	
	{
		Handle<Database> module = new Database("Database");
		module.start_detached();
	}
	
	DatabaseClient client("Database");
	
	Object object;
	object["name"] = "sdfsdfsdfds";
	object["balance"] = 1234;
	
	const Hash128 id = Hash128::rand();
	
	VNX_TEST_BEGIN("truncate")
		client.truncate("test");
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("insert_one")
		client.insert_one("test", id, object);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("select_one")
		const Object obj = client.select_one("test", id);
		expect(obj["id"].to<Hash128>(), id);
		expect(obj["name"], object["name"]);
		expect(obj["balance"], object["balance"]);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("update_one")
		Object obj;
		obj["balance"] = 12345;
		client.update_one("test", id, obj);
		{
			const Object obj2 = client.select_one("test", id);
			expect(obj2["balance"], obj["balance"]);
		}
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("select_query_1")
		auto query = query::select("test", query::equal(query::field("name"), query::value("sdfsdfsdfds")));
		auto result = client.select(query);
		expect(result.size(), 1);
		expect(result[0]["id"].to<Hash128>(), id);
		expect(result[0]["name"].to_string_value(), "sdfsdfsdfds");
		expect(result[0]["balance"].to<int>(), 12345);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("insert_many")
		std::map<Hash128, Object> objs;
		for(int i = 0; i < 100; ++i) {
			Object obj;
			obj["name"] = "user_" + std::to_string(i);
			obj["balance"] = i;
			obj["group"] = i / 10;
			objs[Hash128::rand()] = obj;
		}
		client.insert_many("test", objs);
		auto result = client.select(query::select("test", query::equal(query::field("name"), query::value("user_10"))));
		expect(result.size(), 1);
		expect(result[0]["balance"].to<int>(), 10);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("write_new_block")
		client.write_new_block();
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("update_query_1")
		auto query = query::update("test", "balance", 1337.7331, query::equal(query::field("name"), query::value("sdfsdfsdfds")));
		client.update(query);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("select_query_2")
		auto query = query::select("test", 0, query::order_by("balance", -1), query::limit(1));
		query.fields["name_"] = query::field("name");
		query.fields["balance"] = query::field("balance");
		auto result = client.select(query);
		expect(result.size(), 1);
		expect(result[0]["name_"], object["name"]);
		expect(result[0]["balance"].to<double>(), 1337.7331);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("select_query_3")
		auto query = query::select("test", query::not_null(query::field("group")), query::group_by("group"), query::order_by("total", 1));
		query.aggregates["total"] = query::sum(query::field("balance"));
		auto result = client.select(query);
		expect(result.size(), 10);
		expect(result[0]["group"].to<int>(), 0);
		expect(result[0]["total"].to<int>(), 45);
	VNX_TEST_END()
	
	VNX_TEST_BEGIN("delete_from")
		auto query = query::delete_from("test", query::equal(query::field("name"), query::value("sdfsdfsdfds")));
		client.delete_from(query);
		auto result = client.get_table_info();
		expect(result.count("test"), 1);
		expect(result["test"]["row_count"].to<int>(), 0);
	VNX_TEST_END()
	
	vnx::close();
	
	vnx::test::exit();
	
}
