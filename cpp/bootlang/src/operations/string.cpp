#include "operations/string.hpp"

Value::Ptr string_eq_string(Value::Ptr a_base, Value::Ptr b_base) {
    std::shared_ptr<StringValue> a = Value::toDerived<StringValue>(a_base);
    std::shared_ptr<StringValue> b = Value::toDerived<StringValue>(b_base);
    return std::make_shared<BoolValue>(a->value.compare(b->value) == 0);
}

Value::Ptr string_add_string(Value::Ptr a_base, Value::Ptr b_base) {
    std::shared_ptr<StringValue> a = Value::toDerived<StringValue>(a_base);
    std::shared_ptr<StringValue> b = Value::toDerived<StringValue>(b_base);
    return std::make_shared<StringValue>(a->value + b->value);
}

Value::Ptr to_string(Value::CallableInfo& info) {
    return std::make_shared<StringValue>(info.args.at(0)->toString());
}

Value::Ptr string_join(Value::CallableInfo& info) {
    std::string join_str = Value::toDerived<StringValue>(info.self)->value;
    
    Value::Ptr iterable = info.args.at(0);

    auto iter = iterable->toIter();
    std::string result = std::string("");

    while (!iter->finished) {
        if (!result.empty()) {
            result += join_str;
        }

        Value::Ptr value = iterable->next(iter);

        if (value->type != Value::Type::STRING) {
            throw std::runtime_error("Join exptected str instance");
        }

        result += Value::toDerived<StringValue>(value)->value;
    }

    return std::make_shared<StringValue>(result);
}

Value::Ptr string_upper(Value::CallableInfo& info) {
    std::string s = Value::toDerived<StringValue>(info.self)->value;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    return std::make_shared<StringValue>(s);
}

Value::Ptr string_lower(Value::CallableInfo& info) {
    std::string s = Value::toDerived<StringValue>(info.self)->value;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    return std::make_shared<StringValue>(s);
}

Value::Ptr string_title(Value::CallableInfo& info) {
    std::string s = Value::toDerived<StringValue>(info.self)->value;
    std::string result = "";
    bool previous_was_letter = false;

    for (char c : s) {
        bool is_letter = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');

        if (!is_letter) {
            previous_was_letter = false;
            result += c;
            continue;
        }

        if (previous_was_letter) {
            result += ::tolower(c);
        } else {
            result += ::toupper(c);
        }
    }

    return std::make_shared<StringValue>(result);
}
