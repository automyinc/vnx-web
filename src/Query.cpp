
#include <vnx/query/package.h>


namespace vnx {
namespace query {

Variant Expression::execute(const Object& object) const {
	return Variant();
}

Variant Field::execute(const Object& object) const {
	return object[name];
}

Variant Value::execute(const Object& object) const {
	return data;
}

Variant Equal::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return A->execute(object) == B->execute(object);
}

Variant NotEqual::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return A->execute(object) != B->execute(object);
}

Variant Less::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return A->execute(object) < B->execute(object);
}

Variant LessEqual::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return A->execute(object) <= B->execute(object);
}

Variant Greater::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return A->execute(object) > B->execute(object);
}

Variant GreaterEqual::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return A->execute(object) >= B->execute(object);
}

Variant And::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return bool(A->execute(object)) && bool(B->execute(object));
}

Variant Or::execute(const Object& object) const {
	if(!A || !B) {
		throw std::logic_error("execute(): !A || !B");
	}
	return bool(A->execute(object)) || bool(B->execute(object));
}

Variant NotNull::execute(const Object& object) const {
	if(!A) {
		throw std::logic_error("execute(): !A");
	}
	const Variant result = A->execute(object);
	return !(result.empty() || result.is_null());
}


void Aggregate::update(const Object& object) {
	// nothing
}

void Min::update(const Object& object) {
	if(!A) {
		throw std::logic_error("update(): !A");
	}
	const Variant tmp = A->execute(object);
	if(result.empty() || tmp < result) {
		result = tmp;
	}
}

void Max::update(const Object& object) {
	if(!A) {
		throw std::logic_error("update(): !A");
	}
	const Variant tmp = A->execute(object);
	if(result.empty() || tmp > result) {
		result = tmp;
	}
}

void Sum::update(const Object& object) {
	if(!A) {
		throw std::logic_error("update(): !A");
	}
	result += A->execute(object).to<float64_t>();
}

void Count::update(const Object& object) {
	result++;
}


Variant Aggregate::get_result() const {
	return Variant();
}

Variant Min::get_result() const {
	return result;
}

Variant Max::get_result() const {
	return result;
}

Variant Sum::get_result() const {
	return result;
}

Variant Count::get_result() const {
	return result;
}

	
} // query
} // vnx
