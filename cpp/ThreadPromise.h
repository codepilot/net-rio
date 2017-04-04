#pragma once

#include "node_object_wrap.h"

class CreateFile_iaar : public IOCPArgsAndResolver<CreateFile_iaar> {
public:
	std::wstring wsFileName;
	uint32_t dwDesiredAccess{ GENERIC_READ };
	uint32_t dwShareMode{ FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE };
	LPSECURITY_ATTRIBUTES lpSecurityAttributes{ nullptr };
	uint32_t dwCreationDisposition{ OPEN_EXISTING };
	uint32_t dwFlagsAndAttributes{ FILE_ATTRIBUTE_NORMAL };
	HANDLE hTemplateFile{ nullptr };
	HANDLE ret{ INVALID_HANDLE_VALUE };
	void doWork() {
		ret = CreateFileW(wsFileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		CreateIoCompletionPort(ret, uv_default_loop()->iocp, 0, 0);
	}

	void afterWork(uv_async_t* req) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		resolver.Get(isolate)->Resolve(v8::Number::New(isolate, static_cast<double_t>(reinterpret_cast<int64_t>(ret))));
	}

	CreateFile_iaar(const v8::FunctionCallbackInfo<v8::Value>& info) : IOCPArgsAndResolver(info) {
		if (info.Length() > 0) {
			v8::String::Value strVal{ info[0] };
			wsFileName = { reinterpret_cast<wchar_t *>(*strVal), static_cast<uint64_t>(strVal.length()) };
			if (info.Length() > 1) {
				dwDesiredAccess = info[1]->Uint32Value();
				if (info.Length() > 2) {
					dwShareMode = info[2]->Uint32Value();
					if (info.Length() > 3) {
						/*lpSecurityAttributes = info[3]->Uint32Value(); */
						if (info.Length() > 4) {
							dwCreationDisposition = info[4]->Uint32Value();
							if (info.Length() > 5) {
								dwFlagsAndAttributes = info[5]->Uint32Value();
								if (info.Length() > 6) {
									/*dwFlagsAndAttributes = info[6]->Uint32Value();*/
								}
							}
						}
					}
				}
			}
		}
	}
};

class WriteFileEx_iaar : public IOCPArgsAndResolver<WriteFileEx_iaar, false> {
public:
	HANDLE hFile{ nullptr };
	v8::Global<v8::Value> buffer;
	int64_t fileOffset{ 0 };
	LPCVOID lpBuffer{ nullptr };
	DWORD nNumberOfBytesToWrite{ 0 };
	BOOL ret{ FALSE };
	DWORD written{ 0 };

	void doWork() {
		uv_async_t *async = new uv_async_t();
		uv_async_init(uv_default_loop(), async, staticAfterWork);
		async->data = this;
		SetLastError(ERROR_SUCCESS);
		(&async->async_req)->u.io.overlapped.Pointer = reinterpret_cast<PVOID>(fileOffset);
		ret = WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, &written, &(&async->async_req)->u.io.overlapped);
		const auto lastError = GetLastError();
		switch (lastError) {
		case ERROR_SUCCESS:
		case ERROR_IO_PENDING:
			return;
		default:
			return;
		}
	}

	void afterWork(uv_async_t* req) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		resolver.Get(isolate)->Resolve(v8::Number::New(isolate, static_cast<double_t>((&req->async_req)->u.io.overlapped.InternalHigh)));
	}

	WriteFileEx_iaar(const v8::FunctionCallbackInfo<v8::Value>& info) : IOCPArgsAndResolver(info) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		if (info.Length() > 0 && info[0]->IsNumber()) {
			hFile = reinterpret_cast<HANDLE>(static_cast<int64_t>(info[0]->NumberValue())) ;
			if (info.Length() > 1 && node::Buffer::HasInstance(info[1])) {
				buffer.Reset(isolate, info[1]);
				lpBuffer = node::Buffer::Data(info[1]);
				nNumberOfBytesToWrite = SafeInt<DWORD>(node::Buffer::Length(info[1]));
				if (info.Length() > 2 && info[2]->IsNumber()) {
					fileOffset = static_cast<int64_t>(info[2]->NumberValue());
				}
			}
		}
	}
};

class ReadFileEx_iaar : public IOCPArgsAndResolver<ReadFileEx_iaar, false> {
public:
	HANDLE hFile{ nullptr };
	v8::Global<v8::Value> buffer;
	int64_t fileOffset{ 0 };
	LPVOID lpBuffer{ nullptr };
	DWORD nNumberOfBytesToRead{ 0 };
	BOOL ret{ FALSE };
	DWORD bytesRead{ 0 };

	void doWork() {
		uv_async_t *async = new uv_async_t();
		uv_async_init(uv_default_loop(), async, staticAfterWork);
		async->data = this;
		SetLastError(ERROR_SUCCESS);
		(&async->async_req)->u.io.overlapped.Pointer = reinterpret_cast<PVOID>(fileOffset);
		ret = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &bytesRead, &(&async->async_req)->u.io.overlapped);
		const auto lastError = GetLastError();
		switch (lastError) {
		case ERROR_SUCCESS:
		case ERROR_IO_PENDING:
			return;
		default:
			return;
		}
	}

	void afterWork(uv_async_t* req) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);
		resolver.Get(isolate)->Resolve(v8::Number::New(isolate, static_cast<double_t>((&req->async_req)->u.io.overlapped.InternalHigh)));
	}

	ReadFileEx_iaar(const v8::FunctionCallbackInfo<v8::Value>& info) : IOCPArgsAndResolver(info) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		if (info.Length() > 0 && info[0]->IsNumber()) {
			hFile = reinterpret_cast<HANDLE>(static_cast<int64_t>(info[0]->NumberValue()));
			if (info.Length() > 1 && node::Buffer::HasInstance(info[1])) {
				buffer.Reset(isolate, info[1]);
				lpBuffer = node::Buffer::Data(info[1]);
				nNumberOfBytesToRead = SafeInt<DWORD>(node::Buffer::Length(info[1]));
				if (info.Length() > 2 && info[2]->IsNumber()) {
					fileOffset = static_cast<int64_t>(info[2]->NumberValue());
				}
			}
		}
	}
};


class SleepEx_iaar : public IOCPArgsAndResolver<SleepEx_iaar> {
public:
	uint32_t milliseconds{ 0 };
	uint32_t ret{ 0 };
	void doWork() {
		auto beginTicks = GetTickCount();
		Sleep(milliseconds);
		auto finishTicks = GetTickCount();
		ret = finishTicks - beginTicks;
	}

	void afterWork(uv_async_t* req) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		resolver.Get(isolate)->Resolve(v8::Integer::NewFromUnsigned(isolate, ret));
	}

	SleepEx_iaar(const v8::FunctionCallbackInfo<v8::Value>& info) : IOCPArgsAndResolver(info) {
		milliseconds = info.Length()>0 ? info[0]->Uint32Value() : 0;
	}

};

class Fence_iaar : public IOCPArgsAndResolver<Fence_iaar> {
public:
	void doWork() {
	}

	void afterWork(uv_async_t* req) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		resolver.Get(isolate)->Resolve(v8::Undefined(isolate));
	}

	Fence_iaar(const v8::FunctionCallbackInfo<v8::Value>& info) : IOCPArgsAndResolver(info) {
	}

};

#if 0
class IOCP_Dispatcher : public node::ObjectWrap {
public:
	HANDLE iocp{ nullptr };

	static void New(const v8::FunctionCallbackInfo<v8::Value>& info) {
		Isolate* isolate = info.GetIsolate();

		if (info.IsConstructCall()) {
			(new ThreadPromise(info))->Wrap(info.This());
			info.GetReturnValue().Set(info.This());
		}
		else {
			std::vector<Local<Value>> argv{ SafeInt<size_t>(info.Length()) };
			for (decltype(info.Length()) i{ 0 }; i < info.Length(); i++) {
				argv[i] = info[i];
			}
			info.GetReturnValue().Set(constructor.Get(isolate)->NewInstance(isolate->GetCurrentContext(), SafeInt<int>(argv.size()), argv.data()).ToLocalChecked());
		}
	}
	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& info) {
		Isolate* isolate = info.GetIsolate();

		std::vector<Local<Value>> argv{ SafeInt<size_t>(info.Length()) };
		for (decltype(info.Length()) i{ 0 }; i < info.Length(); i++) {
			argv[i] = info[i];
		}
		v8::Local<v8::Object> instance =
			constructor.Get(isolate)->NewInstance(isolate->GetCurrentContext(), SafeInt<int>(argv.size()), argv.data()).ToLocalChecked();

		// Prototype
		setMethod(instance, L"_SleepEx", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			SleepEx_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_Fence", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			Fence_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_CreateFile", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			CreateFile_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_WriteFileEx", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			WriteFileEx_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_ReadFileEx", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			ReadFileEx_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});



		info.GetReturnValue().Set(instance);
	}
	static void Init(v8::Local<v8::Object> exports) {
		v8::Isolate* isolate = exports->GetIsolate();

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
		tpl->SetClassName(v8::String::NewFromUtf8(isolate, "ThreadPromise"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		constructor.Set(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "ThreadPromise"),
			tpl->GetFunction());
	}
	static v8::Eternal<v8::Function> constructor;

private:

	DWORD threadProc() {
		if (iocp) {
			for (;;) {
				SleepEx(INFINITE, TRUE);
			}
		}
		else {
			for (;;) {
				std::array<OVERLAPPED_ENTRY, 1> oe;
				ULONG numRemoved{ 0 };
				GetQueuedCompletionStatusEx(iocp, oe.data(), SafeInt<ULONG>(oe.size()), &numRemoved, INFINITE, TRUE);
				for (decltype(numRemoved) i{ 0 }; i < numRemoved; i++) {
					reinterpret_cast<PAPCFUNC>(oe[i].lpCompletionKey)(reinterpret_cast<ULONG_PTR>(oe[i].lpOverlapped));
				}
			}
		}
		return 0;
	}

	static DWORD static_threadProc(LPVOID lpThreadParameter) {
		return reinterpret_cast<ThreadPromise *>(lpThreadParameter)->threadProc();
	}

	ThreadPromise(const v8::FunctionCallbackInfo<v8::Value>& info) :
		iocp{ (info.Length()>0 && info[0]->IsNumber()) ? reinterpret_cast<HANDLE>(static_cast<int64_t>(info[0]->NumberValue())) : nullptr },
		threadHandle{ CreateThread(nullptr, 0, static_threadProc, this, 0, 0) } {
	}

	~ThreadPromise() {
		QueueUserAPC([](ULONG_PTR dwParam) {
			TerminateThread(GetCurrentThread(), 0);
		}, threadHandle, reinterpret_cast<ULONG_PTR>(this));
		WaitForSingleObject(threadHandle, INFINITE);
		CloseHandle(threadHandle);
	}

};
#endif

class ThreadPromise : public node::ObjectWrap {
public:
	HANDLE threadHandle{ nullptr };
	HANDLE iocp{ nullptr };

	static void New(const v8::FunctionCallbackInfo<v8::Value>& info) {
		Isolate* isolate = info.GetIsolate();

		if (info.IsConstructCall()) {
			(new ThreadPromise(info))->Wrap(info.This());
			info.GetReturnValue().Set(info.This());
		}
		else {
			std::vector<Local<Value>> argv{ SafeInt<size_t>(info.Length()) };
			for (decltype(info.Length()) i{ 0 }; i < info.Length(); i++) {
				argv[i] = info[i];
			}
			info.GetReturnValue().Set(constructor.Get(isolate)->NewInstance(isolate->GetCurrentContext(), SafeInt<int>(argv.size()), argv.data()).ToLocalChecked());
		}
	}

	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& info) {
		Isolate* isolate = info.GetIsolate();

		std::vector<Local<Value>> argv{ SafeInt<size_t>(info.Length()) };
		for (decltype(info.Length()) i{ 0 }; i < info.Length(); i++) {
			argv[i] = info[i];
		}
		v8::Local<v8::Object> instance =
			constructor.Get(isolate)->NewInstance(isolate->GetCurrentContext(), SafeInt<int>(argv.size()), argv.data()).ToLocalChecked();

		// Prototype
		setMethod(instance, L"_SleepEx", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			SleepEx_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_Fence", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			Fence_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_CreateFile", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			CreateFile_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_WriteFileEx", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			WriteFileEx_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});

		setMethod(instance, L"_ReadFileEx", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			ReadFileEx_iaar::create(info)->queue(ObjectWrap::Unwrap<ThreadPromise>(info.Holder())->threadHandle);
		});



		info.GetReturnValue().Set(instance);
	}

	static void Init(v8::Local<v8::Object> exports) {
		v8::Isolate* isolate = exports->GetIsolate();

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
		tpl->SetClassName(v8::String::NewFromUtf8(isolate, "ThreadPromise"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		constructor.Set(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "ThreadPromise"),
			tpl->GetFunction());
	}
	static v8::Eternal<v8::Function> constructor;

private:

	DWORD threadProc() {
		if (iocp) {
			for (;;) {
				SleepEx(INFINITE, TRUE);
			}
		}
		else {
			for (;;) {
				std::array<OVERLAPPED_ENTRY, 1> oe;
				ULONG numRemoved{ 0 };
				GetQueuedCompletionStatusEx(iocp, oe.data(), SafeInt<ULONG>(oe.size()), &numRemoved, INFINITE, TRUE);
				for (decltype(numRemoved) i{ 0 }; i < numRemoved; i++) {
					reinterpret_cast<PAPCFUNC>(oe[i].lpCompletionKey)(reinterpret_cast<ULONG_PTR>(oe[i].lpOverlapped));
				}
			}
		}
		return 0;
	}

	static DWORD static_threadProc(LPVOID lpThreadParameter) {
		return reinterpret_cast<ThreadPromise *>(lpThreadParameter)->threadProc();
	}

	ThreadPromise(const v8::FunctionCallbackInfo<v8::Value>& info) :
		iocp{ (info.Length()>0 && info[0]->IsNumber()) ? reinterpret_cast<HANDLE>(static_cast<int64_t>(info[0]->NumberValue())) : nullptr },
		threadHandle{ CreateThread(nullptr, 0, static_threadProc, this, 0, 0) } {
	}

	~ThreadPromise() {
		QueueUserAPC([](ULONG_PTR dwParam) {
			TerminateThread(GetCurrentThread(), 0);
		}, threadHandle, reinterpret_cast<ULONG_PTR>(this));
		WaitForSingleObject(threadHandle, INFINITE);
		CloseHandle(threadHandle);
	}

};