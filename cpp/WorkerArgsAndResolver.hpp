#pragma once

template<typename T>
class WorkerArgsAndResolver {
public:
	v8::Global<v8::Promise::Resolver> resolver;
	static void staticDoWork(uv_work_t* req) {
		//printf("%u staticDoWork\n", GetTickCount());
		static_cast<T *>(req->data)->doWork(req);
		//printf("%u staticDoWork done\n", GetTickCount());
	}

	static void staticAfterWork(uv_work_t* req, int status) {
		//printf("%u staticAfterWork\n", GetTickCount());
		static_cast<T *>(req->data)->afterWork(req, status);
		v8::Isolate::GetCurrent()->RunMicrotasks();
		delete static_cast<T *>(req->data);
		delete req;
		//printf("%u staticAfterWork done\n", GetTickCount());
	}

	WorkerArgsAndResolver(const v8::FunctionCallbackInfo<v8::Value>& info) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		auto LocalResolver = v8::Promise::Resolver::New(info.GetIsolate());
		info.GetReturnValue().Set(LocalResolver->GetPromise());
		resolver.Reset(isolate, LocalResolver);
	}

	void queue() {
		uv_work_t *work = new uv_work_t();
		work->data = this;
		uv_queue_work(uv_default_loop(), work, staticDoWork, staticAfterWork);
	}

	static void create(const v8::FunctionCallbackInfo<v8::Value>& info) {
		new T(info);
	}
};
