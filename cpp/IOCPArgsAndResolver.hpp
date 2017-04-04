#pragma once

template<typename T, bool explicitSend = true>
class IOCPArgsAndResolver {
public:
	v8::Global<v8::Promise::Resolver> resolver;
	static void staticDoWork(T * that) {
		that->doWork();
		if (explicitSend) {
			that->send();
		}
	}

	static void staticAfterWork(uv_async_t* handle) {
		static_cast<T *>(handle->data)->afterWork(handle);
		v8::Isolate::GetCurrent()->RunMicrotasks();
		delete static_cast<T *>(handle->data);
		delete handle;
	}

	IOCPArgsAndResolver(const v8::FunctionCallbackInfo<v8::Value>& info) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		auto LocalResolver = v8::Promise::Resolver::New(info.GetIsolate());
		info.GetReturnValue().Set(LocalResolver->GetPromise());
		resolver.Reset(isolate, LocalResolver);
	}

	void send() {
		uv_async_t *async = new uv_async_t();
		uv_async_init(uv_default_loop(), async, staticAfterWork);
		async->data = this;
		PostQueuedCompletionStatus(async->loop->iocp, 0, 0, &(&async->async_req)->u.io.overlapped);
	}

	void queue(HANDLE threadHandle) {
		QueueUserAPC(reinterpret_cast<PAPCFUNC>(staticDoWork), threadHandle, reinterpret_cast<ULONG_PTR>(this));
	}

	static auto create(const v8::FunctionCallbackInfo<v8::Value>& info) {
		return new T(info);
	}
};
