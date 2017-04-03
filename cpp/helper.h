#pragma once

template<class T>
void setMethod(v8::Handle<v8::Object>& classObj, std::wstring&& funcName, const T&& callback) {
	v8::Isolate* isolate = Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	auto funcTemplate = FunctionTemplate::New(isolate, static_cast<FunctionCallback>(callback));
	Eternal<String> es_temp;//keep string forever
	es_temp.Set(isolate, String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t *>(funcName.data()), String::kInternalizedString, SafeInt<int>(funcName.size())));
	funcTemplate->SetClassName(es_temp.Get(isolate));
	auto func = funcTemplate->GetFunction();
	func->SetName(es_temp.Get(isolate));
	classObj->Set(es_temp.Get(isolate), func);
}


template <typename T> double ptr_to_double(T ptr) { return static_cast<double>(reinterpret_cast<int64_t>(ptr)); }
template <typename T> T double_to_ptr(double dbl) { return reinterpret_cast<T>(static_cast<int64_t>(dbl)); }
template <typename SrcType, typename DstType> DstType ptr_to_ptr(SrcType srcPtr) { return reinterpret_cast<DstType>(srcPtr); }

#define setIndexValue(dst, index, val) { dst->Set(index, val); };
#define setIndexInt32(dst, index, val) { setIndexValue(dst, index, Int32::New(isolate, val)); };
#define setIndexUint32(dst, index, val) { setIndexValue(dst, index, Uint32::New(isolate, val)); };
#define setIndexFloat64(dst, index, val) { setIndexValue(dst, index, Number::New(isolate, val)); };
#define setIndexPtr(dst, index, val) { setIndexValue(dst, index, Number::New(isolate, ptr_to_double(val))); };

#define getEternalLit(lit) EternalStrings::es_##lit.Get(isolate)
#define throwEternalLit(lit) { isolate->ThrowException(v8::Exception::TypeError(getEternalLit(lit))); return; }
#define throwMinArgCount(count) \
if (args.Length() < count) { \
	throwEternalLit(exceptNumArgs); \
}

#define throwWrongArguments() throwEternalLit(exceptWrongArguments);
#define throwArgs_s() \
	const auto isolate{ v8::Isolate::GetCurrent() }; \
	{ \
		throwMinArgCount(1); \
		if (!args[0]->IsString()) { \
			throwWrongArguments(); \
		} \
	}\
	v8::HandleScope handle_scope{ isolate };


#define throwArgs_n() \
	const auto isolate{ v8::Isolate::GetCurrent() }; \
	{ \
		throwMinArgCount(1); \
		if (!args[0]->IsNumber()) { \
			throwWrongArguments(); \
		} \
	} \
	v8::HandleScope handle_scope{ isolate };


#define throwArgs_s()  \
	const auto isolate{ v8::Isolate::GetCurrent() }; \
	{ \
		throwMinArgCount(1); \
		if (!args[0]->IsString()) { \
			throwWrongArguments(); \
		} \
	} \
	v8::HandleScope handle_scope{ isolate };

#define throwArgs_nsnn() \
	const auto isolate{ v8::Isolate::GetCurrent() }; \
	{ \
		throwMinArgCount(4); \
		if (!args[0]->IsNumber() || !args[1]->IsString() || !args[2]->IsNumber() || !args[3]->IsNumber()) { \
			throwWrongArguments(); \
		} \
	} \
	v8::HandleScope handle_scope{ isolate };

#define throwArgs_ns()  \
	const auto isolate{ v8::Isolate::GetCurrent() }; \
	{ \
		throwMinArgCount(2); \
		if (!args[0]->IsNumber() || !args[1]->IsString()) { \
			throwWrongArguments(); \
		} \
	} \
	v8::HandleScope handle_scope{ isolate };

#define getELitValue(src, eLit) src->Get(getEternalLit(eLit))
#define getELitValueFromObject(src, eLit) src->ToObject()->Get(getEternalLit(eLit))
#define getELitValueFromArgN(argN, eLit) args[argN]->ToObject()->Get(getEternalLit(eLit))
#define getELitObjectFromArgN(argN, eLit) args[argN]->ToObject()->Get(getEternalLit(eLit))->ToObject()

#define get_args_n_Elit_as_Array(argN, eLit) Local<Array>::Cast(args[argN]->ToObject()->Get(getEternalLit(eLit)))
#define get_args_n_Elit_as_Uint32(argN, eLit) args[argN]->ToObject()->Get(getEternalLit(eLit))->Uint32Value()

#define setELitValue(dst, eLit, val) { dst->Set(getEternalLit(eLit), val); };
#define setELitInt32(dst, eLit, val) { setELitValue(dst, eLit, Int32::New(isolate, val)); };
#define setELitUint32(dst, eLit, val) { setELitValue(dst, eLit, Uint32::New(isolate, val)); };
#define setELitFloat64(dst, eLit, val) { setELitValue(dst, eLit, Number::New(isolate, val)); };
#define setELitPtr(dst, eLit, val) { setELitValue(dst, eLit, Number::New(isolate, ptr_to_double(val))); };

namespace EternalStrings {
#define createELit(eLit) extern v8::Eternal<v8::String> es_##eLit;
#define createEStr(eVar, eStr) extern v8::Eternal<v8::String> es_##eVar;


#include "EternalStrings.h"
#undef createELit
#undef createEStr
}

template<class T>
void setIterator(v8::Handle<v8::Object>& classObj, const T&& callback) {
	Isolate* isolate = Isolate::GetCurrent(); HandleScope scope(isolate);
	auto funcTemplate = FunctionTemplate::New(isolate, static_cast<FunctionCallback>(callback));
	funcTemplate->SetClassName(getEternalLit(iterator));
	auto func = funcTemplate->GetFunction();
	func->SetName(getEternalLit(iterator));
	Local<Object> obj = Object::New(isolate);
	obj->Set(getEternalLit(next), func);
	classObj->Set(v8::Symbol::GetIterator(isolate), obj);
}


template<class T>
void setPrototypeMethod(v8::Handle<v8::FunctionTemplate>& recv, std::wstring&& funcName, const T&& callback) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent(); v8::HandleScope scope(isolate);
	v8::Local<v8::Signature> s = v8::Signature::New(isolate, recv);
	auto funcTemplate = v8::FunctionTemplate::New(isolate, static_cast<v8::FunctionCallback>(callback), v8::Local<v8::Value>(), s);
	v8::Eternal<v8::String> es_temp;//keep string forever
	es_temp.Set(isolate, v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t *>(funcName.data()), v8::String::kInternalizedString, msl::utilities::SafeInt<int>(funcName.size())));
	funcTemplate->SetClassName(es_temp.Get(isolate));
	auto func = funcTemplate->GetFunction();
	func->SetName(es_temp.Get(isolate));
	recv->PrototypeTemplate()->Set(es_temp.Get(isolate), funcTemplate);
}

template<class T>
void setPrototypeMethod(v8::Handle<v8::FunctionTemplate>& recv, std::string&& funcName, const T&& callback) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent(); v8::HandleScope scope(isolate);
	v8::Local<v8::Signature> s = v8::Signature::New(isolate, recv);
	auto funcTemplate = v8::FunctionTemplate::New(isolate, static_cast<v8::FunctionCallback>(callback), v8::Local<v8::Value>(), s);
	v8::Eternal<v8::String> es_temp;//keep string forever
	es_temp.Set(isolate, v8::String::NewFromUtf8(isolate, reinterpret_cast<const char*>(funcName.data()), v8::String::kInternalizedString, msl::utilities::SafeInt<int>(funcName.size())));
	funcTemplate->SetClassName(es_temp.Get(isolate));
	auto func = funcTemplate->GetFunction();
	func->SetName(es_temp.Get(isolate));
	recv->PrototypeTemplate()->Set(es_temp.Get(isolate), funcTemplate);
}

#define setPrototypeThisMethod(recv, funcName) \
		setPrototypeMethod(recv, #funcName, [](const v8::FunctionCallbackInfo<v8::Value>& args) { \
			ObjectWrap::Unwrap<Adapter>(args.Holder())->funcName(args); \
		});