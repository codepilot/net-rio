#pragma once

template<typename T, bool explicitSend = true>
class Promise_LibUV_Worker {
public:
	v8::Global<v8::Promise::Resolver> resolver;

	static void staticDoWork(uv_work_t* req) {
		static_cast<T *>(req->data)->doWork(req);
	}

	static void staticAfterWork(uv_work_t* req, int status) {
		static_cast<T *>(req->data)->afterWork(req, status);
		v8::Isolate::GetCurrent()->RunMicrotasks();
		delete static_cast<T *>(req->data);
		delete req;
	}

	Promise_LibUV_Worker(const v8::FunctionCallbackInfo<v8::Value>& info) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		auto LocalResolver = v8::Promise::Resolver::New(info.GetIsolate());
		info.GetReturnValue().Set(LocalResolver->GetPromise());
		resolver.Reset(isolate, LocalResolver);
	}

	void queue() {
		uv_work_t *work = new uv_work_t();
		work->data = this;

		uv_queue_work(
			uv_default_loop(),
			work,
			[](uv_work_t* req) {static_cast<T *>(req->data)->doWork(req);},
			[](uv_work_t* req, int status) {
				static_cast<T *>(req->data)->afterWork(req, status);
				v8::Isolate::GetCurrent()->RunMicrotasks();
				delete static_cast<T *>(req->data);
				delete req;});

	}

	static auto create(const v8::FunctionCallbackInfo<v8::Value>& info) {
		return new T(info);
	}
};


class SocketBind_luvw: public Promise_LibUV_Worker<SocketBind_luvw> {
public:
	SOCKET sock{ INVALID_SOCKET };
	LPSOCKADDR_STORAGE sockStorage{ nullptr };
	size_t sockAddrLen{ 0 };
	int ret{ 0 };
	v8::Global<v8::Value> sockAddrBuffer;

	void doWork(uv_work_t* req) {
		ret = bind(sock, reinterpret_cast<const sockaddr *>(sockStorage), sockAddrLen);
	}

	void afterWork(uv_work_t* req, int status) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		resolver.Get(isolate)->Resolve(v8::Integer::New(isolate, ret));
	}

	SocketBind_luvw(const v8::FunctionCallbackInfo<v8::Value>& info) : Promise_LibUV_Worker(info) {
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope scope(isolate);

		puts("socket.bindIocp");
	//SocketBind_luvw::create(info)->queue();
		if (info.Length() < 1) { return; }
		if (!node::Buffer::HasInstance(info[0])) { return; }
		sockAddrBuffer.Reset(isolate, info[0]);
		sockStorage =  reinterpret_cast<LPSOCKADDR_STORAGE>(node::Buffer::Data(info[0]));
		sockAddrLen = node::Buffer::Length(info[0]);
		sock = info.This()->Get(EternalStrings::es_socket.Get(isolate))->Int32Value();
	}

};

class UdpSocket : public node::ObjectWrap {
public:
	static void New(const v8::FunctionCallbackInfo<v8::Value>& info) {
		if (!info.IsConstructCall()) { return; }
		if (info.Length() != 4) { return; }
		if (!(info[0]->IsInt32() && info[1]->IsInt32() && info[2]->IsInt32() && info[3]->IsUint32())) { return; }


		Isolate* isolate = info.GetIsolate();
		(new UdpSocket(info))->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	}

	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& info) {
		__debugbreak();
		Isolate* isolate = info.GetIsolate();

		std::vector<Local<Value>> argv{ SafeInt<size_t>(info.Length()) };
		for (decltype(info.Length()) i{ 0 }; i < info.Length(); i++) {
			argv[i] = info[i];
		}
		v8::Local<v8::Object> instance =
			constructor.Get(isolate)->NewInstance(isolate->GetCurrentContext(), SafeInt<int>(argv.size()), argv.data()).ToLocalChecked();

		info.GetReturnValue().Set(instance);
	}

	void method_bindSync(const v8::FunctionCallbackInfo<v8::Value>& info) {
		puts("socket.bindSync");
		if (info.Length() < 1) { return; }
		if (!node::Buffer::HasInstance(info[0])) { return; }
		sizeof(SOCKADDR_STORAGE);
		LPSOCKADDR_STORAGE sockStorage{ reinterpret_cast<LPSOCKADDR_STORAGE>(node::Buffer::Data(info[0])) };
		PSOCKADDR_IN sockAddrIn{ reinterpret_cast<PSOCKADDR_IN>(sockStorage) };

		size_t sockAddrLen{ node::Buffer::Length(info[0]) };
		info.GetReturnValue().Set(bind(sock, reinterpret_cast<const sockaddr *>(sockStorage), sockAddrLen));
	}
	

	void method_bindIocp(const v8::FunctionCallbackInfo<v8::Value>& info) {
		puts("socket.bindIocp");
		SocketBind_luvw::create(info)->queue();
#if 0
		if (info.Length() < 1) { return; }
		if (!node::Buffer::HasInstance(info[0])) { return; }
		sizeof(SOCKADDR_STORAGE);
		LPSOCKADDR_STORAGE sockStorage{ reinterpret_cast<LPSOCKADDR_STORAGE>(node::Buffer::Data(info[0])) };
		PSOCKADDR_IN sockAddrIn{ reinterpret_cast<PSOCKADDR_IN>(sockStorage) };

		size_t sockAddrLen{ node::Buffer::Length(info[0]) };
		info.GetReturnValue().Set(bind(sock, reinterpret_cast<const sockaddr *>(sockStorage), sockAddrLen));
#endif
	}

	void method_bindAsync(const v8::FunctionCallbackInfo<v8::Value>& info) {
		puts("socket.bindAsync");
		if (info.Length() < 1) { return; }
		if (!node::Buffer::HasInstance(info[0])) { return; }
		sizeof(SOCKADDR_STORAGE);
		LPSOCKADDR_STORAGE sockStorage{ reinterpret_cast<LPSOCKADDR_STORAGE>(node::Buffer::Data(info[0])) };
		PSOCKADDR_IN sockAddrIn{ reinterpret_cast<PSOCKADDR_IN>(sockStorage) };

		size_t sockAddrLen{ node::Buffer::Length(info[0]) };
		info.GetReturnValue().Set(bind(sock, reinterpret_cast<const sockaddr *>(sockStorage), sockAddrLen));
	}



	static void Init(v8::Local<v8::Object> exports) {
		v8::Isolate* isolate = exports->GetIsolate();

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
		// Prototype

		setPrototypeThisMethod(UdpSocket, tpl, bindSync, method_bindSync);
		setPrototypeThisMethod(UdpSocket, tpl, bindIocp, method_bindIocp);
		setPrototypeThisMethod(UdpSocket, tpl, bindAsync, method_bindAsync);
		setPrototypeMethod(tpl, EternalStrings::es_addMembership, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.addMembership");
			//socket.addMembership(multicastAddress[, multicastInterface])
		});

		setPrototypeMethod(tpl, EternalStrings::es_address, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.address");
			//socket.address()
		});

#if 0
		setPrototypeMethod(tpl, EternalStrings::es_bind, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.bind");
			bind()
			//socket.bind([port][, address][, callback])
			//socket.bind(options[, callback])
		});
#endif

		setPrototypeMethod(tpl, EternalStrings::es_close, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.close");
			//socket.close([callback])
		});

		setPrototypeMethod(tpl, EternalStrings::es_dropMembership, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.dropMembership");
			//socket.dropMembership(multicastAddress[, multicastInterface])
		});

		setPrototypeMethod(tpl, EternalStrings::es_ref, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.ref");
			//socket.ref()
		});

		setPrototypeMethod(tpl, EternalStrings::es_send, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.send");
			//socket.send(msg, [offset, length, ] port, address[, callback])
		});

		setPrototypeMethod(tpl, EternalStrings::es_setBroadcast, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.setBroadcast");
			//socket.setBroadcast(flag)
		});


		setPrototypeMethod(tpl, EternalStrings::es_setMulticastLoopback, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.setMulticastLoopback");
			//socket.setMulticastLoopback(flag)
		});


		setPrototypeMethod(tpl, EternalStrings::es_setMulticastTTL, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.setMulticastTTL");
			//socket.setMulticastTTL(ttl)
		});


		setPrototypeMethod(tpl, EternalStrings::es_setTTL, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.setTTL");
			//socket.setTTL(ttl)
		});


		setPrototypeMethod(tpl, EternalStrings::es_unref, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			puts("socket.unref");
			//socket.unref()
		});

		tpl->SetClassName(getEternalLit(UdpSocket));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		constructor.Set(isolate, tpl->GetFunction());

		exports->Set(getEternalLit(UdpSocket), tpl->GetFunction());
	}
	static v8::Eternal<v8::Function> constructor;

private:
	SOCKET sock;
	static Sockets::GuidMsTcpIp guidMsTcpIP;
	Sockets::MsSockFuncPtrs funcPtrs;

	UdpSocket(int af, int type, int protocol, DWORD dwFlags) : sock{ WSASocketW(af, type, protocol, 0, 0, dwFlags) }, funcPtrs{ sock, guidMsTcpIP } {

	}

	UdpSocket(const v8::FunctionCallbackInfo<v8::Value>& info): UdpSocket{ info[0]->Int32Value(), info[1]->Int32Value(), info[2]->Int32Value(), info[3]->Uint32Value() }//:
																														//iocp{ (info.Length()>0 && info[0]->IsNumber()) ? reinterpret_cast<HANDLE>(static_cast<int64_t>(info[0]->NumberValue())) : nullptr },
																														//threadHandle{ CreateThread(nullptr, 0, static_threadProc, this, 0, 0) }
	{
		Isolate* isolate = info.GetIsolate();
		info.This()->Set(getEternalLit(socket), v8::Number::New(isolate, static_cast<double>(sock)));
	}

	~UdpSocket() {
		closesocket(sock);
	}
};

